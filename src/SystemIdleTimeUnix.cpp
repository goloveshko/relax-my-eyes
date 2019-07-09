#include "SystemIdleTime.h"
#include <QDebug>

#include <X11/Xlib.h>
#include <X11/extensions/scrnsaver.h>

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

    result = info.idle;

    //qDebug() << "SystemIdleTime" << result;

    return result;
}
