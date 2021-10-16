/*****************************************************************************
*  Riechan's Basic Network base tool                                         *
*  @file     NetworkTool.h                                                   *
*  @brief    Network base tool API                                           *
*  @author   Riechan.ye                                                      *
*  @email    Riechan.ye@outlook.com                                          *
*  @version  1.0.0                                                           *
*  @date     2021.10.16 11:00:00                                             *
*  @license  GNU General Public License (GPL)                                *
*                                                                            *
*----------------------------------------------------------------------------*
*  Remark         : Description                                              *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2021/10/16 | 1.0.1     | Riechan.ye     | Create file                     *
*****************************************************************************/
#ifndef __NETWORKTOOL_H__
#define __NETWORKTOOL_H__

class NetworkTool{
public:
    static int GetLocalIp(const char *eth_inf, char *ip);



};




#endif 