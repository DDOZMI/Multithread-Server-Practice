#define _CRT_SECURE_NO_WARNINGS
#include "mPacket.h"

Packet::Packet(const char* input_data) : header(PK_DATA), end_marker(0xFFFF) {
    setData(input_data);
}

Packet::~Packet() {
    delete[] data;
}

void Packet::setData(const char* input_data) {
    int data_len = strlen(input_data) + 1;
    length = sizeof(length) + sizeof(header) + data_len + sizeof(end_marker);
    data = new char[data_len];
    strcpy(data, input_data);
}

char* Packet::toBuffer() {
    char* buffer = new char[length];
    char* ptr = buffer;
    memcpy(ptr, &length, sizeof(length));
    ptr += sizeof(length);
    memcpy(ptr, &header, sizeof(header));
    ptr += sizeof(header);
    memcpy(ptr, data, strlen(data) + 1);
    ptr += strlen(data) + 1;
    memcpy(ptr, &end_marker, sizeof(end_marker));
    return buffer;
}

Packet Packet::fromBuffer(const char* buffer) {
    Packet packet("");
    const char* ptr = buffer;
    memcpy(&packet.length, ptr, sizeof(packet.length));
    ptr += sizeof(packet.length);
    memcpy(&packet.header, ptr, sizeof(packet.header));
    ptr += sizeof(packet.header);

    int data_len = packet.length - sizeof(packet.length) - sizeof(packet.header) - sizeof(packet.end_marker);
    packet.data = new char[data_len];
    memcpy(packet.data, ptr, data_len);
    ptr += data_len;

    memcpy(&packet.end_marker, ptr, sizeof(packet.end_marker));
    return packet;
}