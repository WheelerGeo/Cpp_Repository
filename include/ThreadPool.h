#ifndef __THREADPOLL_H__
#define __THREADPOLL_H__
#include <pthread.h>
#include <vector>
#include <functional>
#include <unistd.h>

using TASKCALLBACK = std::function<void(void*)>;


class ThreadPool{
public:
    ThreadPool(int max_thread, int min_thread, int max_queue);
    static void* threadWorkHandler(void *arg);
    static void* threadMangerHandler(void *arg);
    void threadExit(void);
    void addThreadPoolTask(void* usr_data, TASKCALLBACK task_call_back);
    int getBusyThreadNum(void);
    int getLiveThreadNum(void);
    void threadPoolDestroy(void);
private:
    struct thread_pool_task_{
        TASKCALLBACK task_call_back_ = NULL;
        void* usr_data_ = NULL;
    }thread_pool_task_;

    pthread_mutex_t thread_pool_lock_;                  // 同步锁
    pthread_mutex_t busy_task_lock_;                    // 保护变量锁
    pthread_cond_t queue_not_full_;                     // 任务队列是否满了
    pthread_cond_t queue_not_empty_;                    // 任务队列是否空了
    pthread_t *work_thread_;                            // 工作线程ID
    pthread_t manager_thread_;                          // 管理线程ID
    int min_thread_num_ = 0;
    int max_thread_num_ = 0;
    int live_thread_num_ = 0;
    int busy_thread_num_ = 0;
    int wait_exit_thread_num_ = 0;
    int max_queue_num_ = 0;
    bool shut_down_state_ = 0;                          // 是否要销毁线程池，销毁1，不销毁0
    std::vector<struct thread_pool_task_> thread_queue_;
};





#endif