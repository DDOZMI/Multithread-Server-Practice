#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "mServer.h"
#include "mPacket.h"

using namespace std;

void mServer::err_quit(const char* msg)
{
    cerr << msg << " Error: " << WSAGetLastError() << endl;
    exit(1);
}
void mServer::err_display(const char* msg)
{
    cerr << msg << " Error: " << WSAGetLastError() << endl;
}

mServer::mServer(void) : listen_sock(INVALID_SOCKET), client_sock(INVALID_SOCKET), hThread(NULL) {
}

mServer::~mServer(void) 
{
    closesocket(listen_sock);
    WSACleanup();
}

int mServer::Init() 
{
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        err_quit("WSAStartup()");
        return 1;
    }
    return 0;
}

void mServer::mSocket()
{
    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET)
        err_quit("socket()");
}

void mServer::mBind()
{
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) 
        err_quit("bind()");
}

void mServer::mListen()
{
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) err_quit("listen()");
}

void mServer::mAccept()
{
    while (true) 
    {
        // accept()
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
            break;
        }

        // ������ Ŭ���̾�Ʈ ���� ���
        printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        // ������ ����
        mCreate_Thread();
    }
}

void mServer::mCloseSocket()
{
    closesocket(listen_sock);
}

DWORD WINAPI mServer::ProcessClient(LPVOID arg)
{
    mServer* This = static_cast<mServer*>(arg);
    This->HandleClient();
    return 0;
}

void mServer::HandleClient()
{
    int retval;
    SOCKADDR_IN clientaddr;
    int addrlen;
    char buf[BUFSIZE + 1];

    // Ŭ���̾�Ʈ ���� ���
    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

    while (1) {
        // ������ �ޱ�
        retval = recv(client_sock, buf, BUFSIZE, 0);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            break;
        }
        else if (retval == 0)
            break;

        // ���� ������ ���
        buf[retval] = '\0';
        printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
            ntohs(clientaddr.sin_port), buf);

        // ������ ������
        retval = send(client_sock, buf, retval, 0);
        if (retval == SOCKET_ERROR) {
            err_display("send()");
            break;
        }
    }

    // closesocket()
    closesocket(client_sock);
    printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
        inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
}

void mServer::mCreate_Thread()
{
    hThread = CreateThread(NULL, 0, ProcessClient,
        this, 0, NULL);
    if (hThread == NULL) 
    { 
        closesocket(client_sock); 
        err_display("CreateThread()");
    }
    else 
    {
        CloseHandle(hThread); 
    }
}

DWORD WINAPI mServer::ThreadHandler(LPVOID lpParam) {
    mServer* This = static_cast<mServer*>(lpParam);
    This->HandleClient();
    return 0;
}