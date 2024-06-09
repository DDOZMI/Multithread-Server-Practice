#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

#define SERVERPORT 9000
#define BUFSIZE    512

class Server
{
private:
    WSADATA wsa;
    SOCKET listen_sock;
    SOCKET client_sock;
    SOCKADDR_IN serveraddr;
    SOCKADDR_IN clientaddr;
    int addrlen;
    int retval;
    HANDLE hThread;
    time_t timer;
    struct tm* curr_timer;

    static DWORD WINAPI ProcessClient(LPVOID arg);
    void HandleClient();
    void mCreate_Thread();
    DWORD WINAPI ThreadHandler(LPVOID lpParam);

    void err_quit(const char* msg);
    void err_display(const char* msg);

    int recvn(SOCKET s, char* buf, int len, int flags);

public:
    Server(void);
    ~Server(void);
    int Init();
    void mSocket();
    void mBind();
    void mListen();
    void mAccept();
    void mCloseSocket();
};
