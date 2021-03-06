/*****************************************************************************
*  Riechan's Basic Timer library                                             *
*  @file     ThreadPool.h                                                    *
*  @brief    ThreadPool create API                                           *
*  @author   Riechan.ye                                                      *
*  @email    Riechan.ye@outlook.com                                          *
*  @version  1.0.0                                                           *
*  @date     2021.10.15 20:00:00                                             *
*  @license  GNU General Public License (GPL)                                *
*                                                                            *
*----------------------------------------------------------------------------*
*  Remark         : Description                                              *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2021/10/15 | 1.0.1     | Riechan.ye     | Create file                     *
*****************************************************************************/
#ifndef __THREADPOLL_H__
#define __THREADPOLL_H__
#include <pthread.h>
#include <vector>
#include <functional>
#include <unistd.h>
#include "ErrorCode.h"

using TASKCALLBACK = std::function<OPERATE_RET(void*)>;


class ThreadPool{
public:
    ThreadPool(void);
    static ThreadPool& getInstance(void);
    void threadPoolStart(int min_thread = 3, int max_thread = 10, int max_queue = 50);
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

    pthread_mutex_t thread_pool_lock_;                  // ?????????
    pthread_mutex_t busy_task_lock_;                    // ???????????????
    pthread_cond_t queue_not_full_;                     // ????????????????????????
    pthread_cond_t queue_not_empty_;                    // ????????????????????????
    pthread_t *work_thread_;                            // ????????????ID
    pthread_t manager_thread_;                          // ????????????ID
    int min_thread_num_ = 0;
    int max_thread_num_ = 0;
    int live_thread_num_ = 0;
    int busy_thread_num_ = 0;
    int wait_exit_thread_num_ = 0;
    int max_queue_num_ = 0;
    bool shut_down_state_ = 0;                          // ?????????????????????????????????1????????????0
    std::vector<struct thread_pool_task_> thread_queue_;
    bool is_start_ = false;
};





#endif