#ifndef __ERRORCODE_H__
#define __ERRORCODE_H__

using OPERATE_RET = int;

#define OPRT_OK                                             (-0x0000) // 0, 执行成功
#define OPRT_COM_ERROR                                      (-0x0001) //-1, 通用错误
#define OPRT_INVALID_PARM                                   (-0x0002) //-2, 无效的入参
#define OPRT_MALLOC_FAILED                                  (-0x0003) //-3, 内存分配失败
#define OPRT_NOT_SUPPORTED                                  (-0x0004) //-4, 不支持
#define OPRT_NETWORK_ERROR                                  (-0x0005) //-5, 网络错误
#define OPRT_NOT_FOUND                                      (-0x0006) //-6, 没有找到对象 
#define OPRT_SOCK_CREATE_ERROR                              (-0x0007) //-7, socket创建失败
#define OPRT_SOCK_BIND_ERROR                                (-0x0008) //-8, socket绑定失败
#define OPRT_SOCK_LISTEN_ERROR                              (-0x0009) //-9, socket监听失败
#define OPRT_SOCK_ACCEPT_ERROR                              (-0x000A) //-10, socket接受连接失败
#define OPRT_SOCK_CONNECT_ERROR                             (-0x000B) //-11, socket连接失败
#define OPRT_SOCK_RECV_ERROR                                (-0x000C) //-12, socket接受数据失败
#define OPRT_SOCK_SEND_ERROR                                (-0x000D) //-13, socket发送数据失败
#define OPRT_SOCK_CLOSE_ERROR                               (-0x000E) //-14, socket关闭连接失败
#define OPRT_SOCK_RECVFROM_ERROR                            (-0x000F) //-15, socketUDP接受数据失败 
#define OPRT_SOCK_SENDTO_ERROR                              (-0x0010) //-16, socketUDP发送数据失败 
#define OPRT_HTTP_METHOD_ERROR                              (-0x0011) //-17, 解析http请求方式错误
#define OPRT_HTTP_PROTO_ERROR                               (-0x0012) //-18, 解析http协议错误

#define OPRT_EPOLL_CREATE_ERROR                             (-0x0020) //-32, epoll创建失败
#define OPRT_EPOLL_CTL_ERROR                                (-0x0021) //-33, epoll注册事件失败
#define OPRT_EPOLL_ADDEVENT_ERROR                           (-0x0022) //-34, epoll添加事件失败
#define OPRT_EPOLL_WAIT_ERROR                               (-0x0023) //-35, epoll等待事件失败

#define OPRT_FILE_READ_ERROR                                (-0x0030) //-48, file读取失败


#define OPRT_IOCTL_ERROR                                    (-0x0040) //-64, ioctl读取失败


#define OPRT_TIME_GET_ERROR                                 (-0x0050) //-80, 获取时间失败

#define OPRT_LOG_INIT_ERROR                                 (-0x0060) //-96, logger初始化失败
#define OPRT_LOG_NOSHM_ERROR                                (-0x0061) //-97, logger缺失共享内存
#define OPRT_LOG_START_ERROR                                (-0x0062) //-98, logger启动失败



#endif