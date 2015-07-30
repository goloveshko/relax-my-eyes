//! Objective-C++ class to enable Cocoa calls in C++

#pragma once

#include <QObject>

class SystemIdleTime
{
public:
	SystemIdleTime();
	~SystemIdleTime();

	double getIdleTime();
};
