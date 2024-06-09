#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <iostream>
#include "Packet.h"

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    512

using namespace std;

// ���� �Լ� ���� ��� �� ����
void err_quit(const char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    cerr << msg << " ����: " << static_cast<char*>(lpMsgBuf) << endl;
    LocalFree(lpMsgBuf);
    exit(1);
}

// ���� �Լ� ���� ���
void err_display(const char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    cerr << msg << " ����: " << static_cast<char*>(lpMsgBuf) << endl;
    LocalFree(lpMsgBuf);
}

int recvn(SOCKET s, char* buf, int len, int flags)
{
    int received;
    char* ptr = buf;
    int left = len;

    while (left > 0) {
        received = recv(s, ptr, left, flags);
        if (received == SOCKET_ERROR)
            return SOCKET_ERROR;
        else if (received == 0)
            break;
        left -= received;
        ptr += received;
    }

    return (len - left);
}

int main(int argc, char* argv[])
{
    int retval;

    // ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // socket()
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) err_quit("socket()");

    // connect()
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("connect()");

    // ������ ��ſ� ����� ����
    char buf[BUFSIZE + 1];
    int len;
    Packet sPK;
    Packet rPK;
    int count = 0;
    string serializedPacket;
    string receivedPacket;

    // ������ ������ ���
    while (1) {
        if (count == 0)
        {
            // ��Ŷ ����
            sPK = Packet(5, Header::req_con, "BB", 0xffff);
            serializedPacket = sPK.serialize();

            // ������ ������
            retval = send(sock, serializedPacket.c_str(), serializedPacket.size(), 0);
            if (retval == SOCKET_ERROR) {
                err_display("send()");
                break;
            }
            printf("[%s] %d����Ʈ�� ���½��ϴ�.\n", sPK.getData().c_str(), retval);

            // ������ �ޱ�
            retval = recvn(sock, buf, sPK.getFullSize(), 0);
            if (retval == SOCKET_ERROR) {
                err_display("recv()");
                break;
            }
            else if (retval == 0)
                break;

            receivedPacket = string(buf, retval);
            rPK = Packet::deserialize(receivedPacket);
            printf("%s\n", rPK.getData());
            //std::cout << "[" << rPK.getData() << "]" << endl;

            count++;
        }
        else
        {
            // ������ �Է�
            std::cout << "\n[���� ������] ";
            if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
                break;

            // '\n' ���� ����
            len = strlen(buf);
            if (buf[len - 1] == '\n')
                buf[len - 1] = '\0';
            if (strlen(buf) == 0)
                break;

            // ��Ŷ ���� �� ����
            sPK = Packet(len, Header::chat_string, buf, 0xffff);
            serializedPacket = sPK.serialize();

            retval = send(sock, serializedPacket.c_str(), serializedPacket.size(), 0);
            if (retval == SOCKET_ERROR) {
                err_display("send()");
                break;
            }
            std::cout << "[TCP Ŭ���̾�Ʈ] " << retval << "����Ʈ�� ���½��ϴ�.\n";

            // ������ �ޱ�
            retval = recvn(sock, buf, sPK.getFullSize(), 0);
            if (retval == SOCKET_ERROR) {
                err_display("recv()");
                break;
            }
            else if (retval == 0)
                break;

            receivedPacket = string(buf, retval);
            rPK = Packet::deserialize(receivedPacket);
            std::cout << rPK.getData() << endl;
        }
    }

    // closesocket()
    closesocket(sock);

    // ���� ����
    WSACleanup();
    return 0;
}
