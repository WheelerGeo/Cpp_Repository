/*****************************************************************************
*  Riechan's Basic StdInput library                                          *
*  @file     StdInput.h                                                      *
*  @brief    StdInput API                                                    *
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
#ifndef __STDINPUT_H__
#define __STDINPUT_H__
#include "EventPoll.h"
#include "ErrorCode.h"

class StdInput{
public:
    StdInput(EventPoll* my_epoll);
    OPERATE_RET stdinInit(void);
    static OPERATE_RET receive(void* server, int fd);
    OPERATE_RET addUsrData(void* usr_data);
    virtual OPERATE_RET recvCallBack(void* usr_data, const std::string& buff);
private:
    EventPoll* epoll_ = NULL;
    std::string buff_ = "";
    void* usr_data_ = NULL;
};



#endif