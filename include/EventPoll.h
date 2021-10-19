/*****************************************************************************
*  Riechan's Basic EventPoll library                                         *
*  @file     EventPoll.h                                                     *
*  @brief    epoll loop API                                                  *
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
#ifndef __EVENTPOLL_H__
#define __EVENTPOLL_H__
#include <functional>
#include <sys/epoll.h>
#include <map>
#include <vector>
#include <sys/time.h>
#include "ErrorCode.h"

#define EVENTS_MAX      1024
#define EPOLL_TIMEOUT   1000

class EventPoll{
public:
    using CALLBACK = std::function<OPERATE_RET(void* usr_data, int fd)>;
    using TIMCALLBACK = std::function<OPERATE_RET(void* usr_data, const long int now_time_ms, int* time_duration)>;
    EventPoll(void);
    OPERATE_RET create(void);
    OPERATE_RET addEvent(void* usr_data, int fd, int flags, CALLBACK callback);
    void addTimer(void* usr_data, TIMCALLBACK tim_callback);
    OPERATE_RET loop();
private:
    int epoll_fd_ = -1;
    int events_cnt = 0;
    struct info {  
        CALLBACK callback_ = NULL;
        void* usr_data_ = NULL;
    };
    std::map<int, struct info> event_map_;
    TIMCALLBACK tim_callback_ = NULL;
    std::map<void*, TIMCALLBACK> timer_map_;
    epoll_event events_[EVENTS_MAX] ={0};
    int epoll_time_out_ = EPOLL_TIMEOUT;
    long int now_time_ms_ = 0;
};



#endif 