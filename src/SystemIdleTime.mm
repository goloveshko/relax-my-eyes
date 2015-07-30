#include "SystemIdleTime.h"
//#import <Foundation/Foundation.h>
//#import <Cocoa/Cocoa.h>
#include <ApplicationServices/ApplicationServices.h>

SystemIdleTime::SystemIdleTime()
{
}

SystemIdleTime::~SystemIdleTime()
{
}

double SystemIdleTime::getIdleTime()
{
	CFTimeInterval timeSinceLastEvent = CGEventSourceSecondsSinceLastEventType(kCGEventSourceStateHIDSystemState, kCGAnyInputEventType);
	return timeSinceLastEvent;
}
