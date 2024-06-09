#include "Socket.h"
#include <iostream>

using namespace std;

Socket::Socket(void)
{

}
Socket::~Socket(void)
{

}
void Socket::err_quit(const char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    cerr << msg << " 에러: " << static_cast<char*>(lpMsgBuf) << endl;
    LocalFree(lpMsgBuf);
    exit(1);
}
void Socket::err_display(const char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    cerr << msg << " 에러: " << static_cast<char*>(lpMsgBuf) << endl;
    LocalFree(lpMsgBuf);
}
