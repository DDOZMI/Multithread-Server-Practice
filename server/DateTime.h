#pragma once
#include <ctime>

class DateTime {
private:
    char* dtBuffer;
    struct tm* curr_timer;
    void dtUpdate();
public:
    DateTime();
    ~DateTime();
    char* getDateTime();
};