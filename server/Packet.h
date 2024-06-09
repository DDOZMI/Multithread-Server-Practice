#pragma once

#include <iostream>
#include <string>

using namespace std;

enum class Header : uint8_t {
    req_con,
    ack_con,
    req_move,
    ack_move,
    chat_string,
};

class Packet {
private:
    uint16_t pSize;
    Header pHeader;
    string pData;
    uint16_t pEndMark;

public:
    Packet();
    Packet(uint16_t size, Header header, const string& data, uint16_t endMark);

    uint16_t getSize() const;
    void setSize(uint16_t size);

    Header getHeader() const;
    void setHeader(Header header);

    const string& getData() const;
    void setData(const string& data);

    uint16_t getEndMark() const;
    void setEndMark(uint16_t endMark);

    uint8_t getFullSize() const;

    string serialize() const;
    static Packet deserialize(const string& serialized);
};

