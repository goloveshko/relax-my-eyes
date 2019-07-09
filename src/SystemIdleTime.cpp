#include "SystemIdleTime.h"

SystemIdleTime::SystemIdleTime()
{
}

SystemIdleTime::~SystemIdleTime()
{
}

double SystemIdleTime::getIdleTime()
{
    double result = 0.0;
    LASTINPUTINFO last_input_info;
    last_input_info.cbSize = sizeof(LASTINPUTINFO);
    if (! GetLastInputInfo(&last_input_info)) {
        fprintf(stderr, "GetLastInputInfo() failed");
        return (unsigned long)-1;
    }

    result = GetTickCount64() - last_input_info.dwTime;
    return result;
}
