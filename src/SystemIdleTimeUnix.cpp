#include "SystemIdleTime.h"
#include <QDebug>

#include <X11/Xlib.h>
#include <X11/extensions/scrnsaver.h>

#include <chrono>

SystemIdleTime::SystemIdleTime()
{
}

SystemIdleTime::~SystemIdleTime()
{
}

double SystemIdleTime::getIdleTime()
{
    double result = 0.0;

    XScreenSaverInfo info;

    Display *display = XOpenDisplay((char*)0);

    if(! display)
    {
        qDebug() << "XOpenDisplay() failed";
        return (unsigned long)-1;
    }

    if (! XScreenSaverQueryInfo (display, DefaultRootWindow (display), &info)) {
        XCloseDisplay (display);
        "XOpenDisplay() failed""XScreenSaveQueryInfo() failed";
        return (unsigned long)-1;
    }

    XCloseDisplay (display); display = 0;

    std::chrono::milliseconds ms (info.idle);
    std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds> (ms);

    result = s.count();

    //qDebug() << "SystemIdleTime" << result;

    return result;
}
