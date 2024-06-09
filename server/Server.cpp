#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "Server.h"
#include "Packet.h"
#include "DateTime.h"

using namespace std;

DateTime::DateTime() : dtBuffer(nullptr), curr_timer(nullptr)
{
    time_t timer = time(NULL);
    curr_timer = localtime(&timer);
    dtUpdate();
}

DateTime::~DateTime()
{
    delete[] dtBuffer;
}

void DateTime::dtUpdate()
{
    int buffer_size = snprintf(nullptr, 0, "%d�� %d�� %d�� %d�� %d�� %d��, ",
        curr_timer->tm_year + 1900, curr_timer->tm_mon + 1, curr_timer->tm_mday,
        curr_timer->tm_hour, curr_timer->tm_min, curr_timer->tm_sec) + 1;

    dtBuffer = new char[buffer_size];

    snprintf(dtBuffer, buffer_size, "%d�� %d�� %d�� %d�� %d�� %d��, ",
        curr_timer->tm_year + 1900, curr_timer->tm_mon + 1, curr_timer->tm_mday,
        curr_timer->tm_hour, curr_timer->tm_min, curr_timer->tm_sec);
}

char* DateTime::getDateTime()
{
    if (dtBuffer == nullptr)
        dtUpdate();

    char* result = new char[strlen(dtBuffer) + 1];
    strcpy(result, dtBuffer);

    return result;
}

void Server::err_quit(const char* msg)
{
    cerr << msg << " ����: " << WSAGetLastError() << endl;
    exit(1);
}

void Server::err_display(const char* msg)
{
    cerr << msg << " ����: " << WSAGetLastError() << endl;
}

Server::Server(void) : listen_sock(INVALID_SOCKET), client_sock(INVALID_SOCKET), hThread(NULL) { }

Server::~Server(void)
{
    closesocket(listen_sock);
    WSACleanup();
}

int Server::Init()
{
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        err_quit("WSAStartup()");
        return 1;
    }
    return 0;
}

void Server::mSocket()
{
    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET)
        err_quit("socket()");
}

void Server::mBind()
{
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR)
        err_quit("bind()");
}

void Server::mListen()
{
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) err_quit("listen()");
}

void Server::mAccept()
{
    while (true)
    {
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
            break;
        }

        mCreate_Thread();
    }
}

void Server::mCloseSocket()
{
    closesocket(listen_sock);
}

DWORD WINAPI Server::ProcessClient(LPVOID arg)
{
    Server* This = static_cast<Server*>(arg);
    This->HandleClient();
    return 0;
}

void Server::HandleClient()
{
    int retval;
    SOCKADDR_IN clientaddr;
    int addrlen;
    char buf[BUFSIZE + 1];
    Packet rPK;
    Packet sPK;
    DateTime dt;
    string showDateTime = dt.getDateTime();
    string retStr;
    uint16_t fullSize;
    string receivedPacket;
    string serializedPacket;

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

        receivedPacket = string(buf, retval);
        rPK = Packet::deserialize(receivedPacket);

        switch (rPK.getHeader())
        {
        case Header::req_con:
        
            printf("[%s(%s) ����]\n", rPK.getData().c_str(), inet_ntoa(clientaddr.sin_addr));

            retStr = showDateTime + rPK.getData().c_str() + " connected";
            fullSize = 4 + (uint16_t)retStr.size();

            sPK = Packet(fullSize, Header::ack_con, retStr, 0xffff);

            serializedPacket = sPK.serialize();
            retval = send(client_sock, serializedPacket.c_str(), serializedPacket.size(), 0);
            if (retval == SOCKET_ERROR) {
                err_display("send()");
                break;
            }
            break;
        

        case Header::chat_string:
            
            printf("[] %s\n", rPK.getData().c_str());

            retval = send(client_sock, serializedPacket.c_str(), serializedPacket.size(), 0);
            if (retval == SOCKET_ERROR) {
                err_display("send()");
                break;
            }
            break;

        default:
            break;
        }
    }

    closesocket(client_sock);
    printf("[%s ���� ����]", inet_ntoa(clientaddr.sin_addr));
    //cout << "[Ŭ���̾�Ʈ ����] IP: " << inet_ntoa(clientaddr.sin_addr) << ", ��Ʈ ��ȣ=" << ntohs(clientaddr.sin_port) << "\n";
}

void Server::mCreate_Thread()
{
    hThread = CreateThread(NULL, 0, ProcessClient, this, 0, NULL);
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

DWORD WINAPI Server::ThreadHandler(LPVOID lpParam) {
    Server* This = static_cast<Server*>(lpParam);
    This->HandleClient();
    return 0;
}
