#include "mSocket.h"
#include "mServer.h"

int main(int argc, char* argv[])
{
    mServer myServer;
    myServer.Init();
    myServer.mSocket();
    myServer.mBind();
    myServer.mListen();
    myServer.mAccept();
    myServer.mCloseSocket();

    return 0;
}