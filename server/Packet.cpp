#include "Packet.h"

using namespace std;

Packet::Packet() : pSize(0), pEndMark(0) {}

Packet::Packet(uint16_t size, Header header, const string& data, uint16_t endMark)
    : pSize(size), pHeader(header), pData(data), pEndMark(endMark) {}

uint16_t Packet::getSize() const {
    return pSize;
}

void Packet::setSize(uint16_t size) {
    pSize = size;
}

Header Packet::getHeader() const {
    return pHeader;
}

void Packet::setHeader(Header header) {
    pHeader = header;
}

const string& Packet::getData() const {
    return pData;
}

void Packet::setData(const string& data) {
    pData = data;
}

uint16_t Packet::getEndMark() const {
    return pEndMark;
}

void Packet::setEndMark(uint16_t endMark) {
    pEndMark = endMark;
}

uint8_t Packet::getFullSize() const {
    return sizeof(pSize) + sizeof(pHeader) + pData.size() + sizeof(pEndMark);
}

string Packet::serialize() const {
    string serialized;
    serialized.append(reinterpret_cast<const char*>(&pSize), sizeof(pSize));
    serialized.append(reinterpret_cast<const char*>(&pHeader), sizeof(pHeader));
    serialized.append(pData);
    serialized.append(reinterpret_cast<const char*>(&pEndMark), sizeof(pEndMark));
    return serialized;
}

Packet Packet::deserialize(const string& serialized) {
    Packet pk;
    size_t offset = 0;

    memcpy(&pk.pSize, serialized.data() + offset, sizeof(pk.pSize));
    offset += sizeof(pk.pSize);

    memcpy(&pk.pHeader, serialized.data() + offset, sizeof(pk.pHeader));
    offset += sizeof(pk.pHeader);

    size_t dataSize = serialized.size() - offset - sizeof(pk.pEndMark);
    pk.pData = serialized.substr(offset, dataSize);
    offset += dataSize;

    memcpy(&pk.pEndMark, serialized.data() + offset, sizeof(pk.pEndMark));

    return pk;
}
