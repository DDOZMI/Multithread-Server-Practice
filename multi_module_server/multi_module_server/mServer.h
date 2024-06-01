#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

#define SERVERPORT 9000
#define BUFSIZE    512

class mServer
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

    static DWORD WINAPI ProcessClient(LPVOID arg);
    void HandleClient();
    void mCreate_Thread();
    DWORD WINAPI mServer::ThreadHandler(LPVOID lpParam);

    void err_quit(const char* msg);
    void err_display(const char* msg);

public:
    mServer(void);
    ~mServer(void);
    int Init();
    void mSocket();
    void mBind();
    void mListen();
    void mAccept();
    void mCloseSocket();
};