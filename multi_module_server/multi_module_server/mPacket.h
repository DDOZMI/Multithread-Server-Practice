#pragma once

#include <iostream>
#include <WinSock2.h>

enum Header
{
	PK_DATA = 1
};

class Packet {
public:
    short length;
    char header;
    char* data;
    short end_marker;

    Packet(const char* input_data);
    ~Packet();
    char* toBuffer();
    static Packet fromBuffer(const char* buffer);

private:
    void setData(const char* input_data);
};