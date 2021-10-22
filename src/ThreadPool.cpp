#include "../include/ThreadPool.h"
#include "../include/Logger.h"

ThreadPool& ThreadPool::getInstance(void) {
    static ThreadPool instance;
    return instance;
}

void ThreadPool::threadPoolStart(int min_thread, int max_thread, int max_queue) {
    
    if (pthread_mutex_init(&thread_pool_lock_, NULL) != 0 || 
        pthread_mutex_init(&busy_task_lock_, NULL) != 0 ||
        pthread_cond_init(&queue_not_full_, NULL) != 0 || 
        pthread_cond_init(&queue_not_empty_, NULL) != 0) {
        LogError() << "ThreadPool:mutex and cond init error";
    }
    pthread_mutex_lock(&thread_pool_lock_);
    max_thread_num_ = max_thread;
    min_thread_num_ = min_thread;
    max_queue_num_ = max_queue;
    LogDebug() << max_queue_num_;
    busy_thread_num_ = 0;
    live_thread_num_ = min_thread_num_;
    wait_exit_thread_num_ = 0;
    work_thread_ = new pthread_t[max_thread_num_];
    // 重设任务队列容量
    // thread_queue_.resize(max_queue_num_);
    // 创建管理者线程
    pthread_create(&manager_thread_, NULL, threadMangerHandler, this);
    // 创建任务线程
    for(int i = 0; i < min_thread; ++i) {
        pthread_create(&work_thread_[i], NULL, threadWorkHandler, this);
    }
    pthread_mutex_unlock(&thread_pool_lock_);
}

void* ThreadPool::threadWorkHandler(void* arg) {
    LogInfo() << "work thread start!";
    ThreadPool* pool = (ThreadPool*)arg;
    while (1) {
        pthread_mutex_lock(&pool->thread_pool_lock_);
        
        while (pool->thread_queue_.size() == 0 && !pool->shut_down_state_) {
            pthread_cond_wait(&pool->queue_not_empty_, &pool->thread_pool_lock_);

            if (pool->wait_exit_thread_num_ > 0) {
                pool->wait_exit_thread_num_--;
                if (pool->live_thread_num_ > pool->min_thread_num_) {
                    pool->live_thread_num_--; 
                    pthread_mutex_unlock(&pool->thread_pool_lock_);
                    pool->threadExit();
                } 
            }
        }

        if (pool->shut_down_state_) {
            pthread_mutex_unlock(&pool->thread_pool_lock_);
            pool->threadExit();
        }

        auto it = pool->thread_queue_.begin();
        struct thread_pool_task_* task = new struct thread_pool_task_(*it);
        pool->thread_queue_.erase(it);
        pthread_cond_signal(&pool->queue_not_full_);
        pthread_mutex_unlock(&pool->thread_pool_lock_);

        LogInfo() << "tid:" << pthread_self() << "start working!";
        pthread_mutex_lock(&pool->busy_task_lock_);
        pool->busy_thread_num_++;
        pthread_mutex_unlock(&pool->busy_task_lock_);
        
        task->task_call_back_(task->usr_data_);
        free(task);
        task = NULL;
        
        LogInfo() << "tid:" << pthread_self() << "end working!";
        pthread_mutex_lock(&pool->busy_task_lock_);
        pool->busy_thread_num_--;
        pthread_mutex_unlock(&pool->busy_task_lock_);
    }
    return NULL;

}

void* ThreadPool::threadMangerHandler(void* arg) {
    LogInfo() << "manage thread start!";
    ThreadPool* pool = (ThreadPool*)arg;
    while (!pool->shut_down_state_) {
        sleep(3);
        pthread_mutex_lock(&pool->thread_pool_lock_);
        int queue_size = pool->thread_queue_.size();
        int live_thread_num = pool->live_thread_num_;
        int busy_thread_num = pool->busy_thread_num_;
        pthread_mutex_unlock(&pool->thread_pool_lock_);
        // 如果当前任务数量大于存活线程数量，并且存活线程数量没有大于最大线程数量，创建两个新线程
        if (queue_size > live_thread_num && live_thread_num < pool->max_thread_num_) {
            pthread_mutex_lock(&pool->thread_pool_lock_);
            // counter表示一次最多创建两个线程，通过遍历起始到最大线程ID，有空的线程ID就填入
            for (int i = 0, counter = 0; i < pool->max_thread_num_ && counter < 2 
                        && pool->live_thread_num_ < pool->max_thread_num_; ++i) {
                if (0 == pool->work_thread_[i]) {
                    pthread_create(&pool->work_thread_[i], NULL, threadMangerHandler, pool);
                    counter++;
                    pool->live_thread_num_++;
                }
            }
            pthread_mutex_unlock(&pool->thread_pool_lock_);
        }

        // 如果当前忙线程*2小于存活线程数量，并且存活线程数量大于最小线程数量，销毁两个线程
        if (busy_thread_num * 2 < live_thread_num && live_thread_num > pool->min_thread_num_) {
            pthread_mutex_lock(&pool->thread_pool_lock_);
            pool->wait_exit_thread_num_ = 2;
            pthread_mutex_unlock(&pool->thread_pool_lock_);
            for (int i = 0; i < 2; ++i) {
                pthread_cond_signal(&pool->queue_not_empty_);
            }
        }
    }
    return NULL;
}

// 二次封装pthread_exit，用于线程退出时将成员变量线程id数组对应元素清0
void ThreadPool::threadExit(void) {
    for (int i = 0; i < max_thread_num_; ++i) {
        if (work_thread_[i] == pthread_self()) {
            work_thread_[i] == 0;
            break;
        }
    }
    pthread_exit(NULL);
}

void ThreadPool::addThreadPoolTask(void* usr_data, TASKCALLBACK task_call_back) {
    pthread_mutex_lock(&thread_pool_lock_);
    LogDebug() << thread_queue_.size() << " " << max_queue_num_ << " " << shut_down_state_;
    while (thread_queue_.size() == max_queue_num_ && !shut_down_state_) {
        pthread_cond_wait(&queue_not_full_, &thread_pool_lock_);
    }
    LogDebug() << "bbbb";
    if (shut_down_state_) {
        pthread_mutex_unlock(&thread_pool_lock_);
        return;
    }

    thread_pool_task_.usr_data_ = usr_data;
    thread_pool_task_.task_call_back_ = task_call_back;
    thread_queue_.push_back(thread_pool_task_);
    pthread_mutex_unlock(&thread_pool_lock_);
}

int ThreadPool::getBusyThreadNum(void) {
    return busy_thread_num_;
}

int ThreadPool::getLiveThreadNum(void) {
    return live_thread_num_;
}

void ThreadPool::threadPoolDestroy(void) {
    shut_down_state_ = 1;

    // 回收管理者线程
    pthread_join(manager_thread_, NULL);

    // 唤醒消费者线程，让他在线程处理函数中自己主动退出
    for (int i = 0; i < live_thread_num_; ++i) {
        pthread_cond_signal(&queue_not_empty_);
    }

    // 释放容器内存
    std::vector<struct thread_pool_task_>().swap(thread_queue_);

    // 释放线程ID堆内存
    delete(work_thread_);

    pthread_mutex_destroy(&thread_pool_lock_);
    pthread_mutex_destroy(&busy_task_lock_);
    pthread_cond_destroy(&queue_not_full_);
    pthread_cond_destroy(&queue_not_empty_);

    delete(this);
}