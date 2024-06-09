#include "Socket.h"
#include "Server.h"

int main(int argc, char* argv[])
{
    Server myServer;
    myServer.Init();
    myServer.mSocket();
    myServer.mBind();
    myServer.mListen();
    myServer.mAccept();
    myServer.mCloseSocket();

    return 0;
}
