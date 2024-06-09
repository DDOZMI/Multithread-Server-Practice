#pragma once
#pragma comment(lib, "ws2_32")

#include <winsock2.h>
#include <stdlib.h>
#include <process.h>

#define SERVERPORT    9000
#define BUFSIZE        512

class Socket
{
protected:
    SOCKET listen_sock;
    SOCKET client_sock;
    SOCKADDR_IN serveraddr;
    SOCKADDR_IN clientaddr;
    HANDLE hThread;
    WSADATA wsa;

public:
    virtual int Init() = 0;
    virtual void mySocket() {};
    virtual void myBind() {};
    virtual void myListen() {};
    virtual void myCreate_Thread() {};
    virtual void myaccept() {};
    virtual void myCloseSocket() {};
    void err_quit(const char* msg);
    void err_display(const char* msg);
    Socket(void);
    ~Socket(void);
};
