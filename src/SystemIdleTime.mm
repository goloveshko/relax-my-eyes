#include "SystemIdleTime.h"
//#import <Foundation/Foundation.h>
//#import <Cocoa/Cocoa.h>
#include <ApplicationServices/ApplicationServices.h>

#import <Foundation/Foundation.h>
#import <IOKit/IOKitLib.h>

SystemIdleTime::SystemIdleTime()
{
}

SystemIdleTime::~SystemIdleTime()
{
}

//https://gist.github.com/darrarski/c586ce379d3e7e91a57d89ed557192ec
double SystemIdleTime::getIdleTime(void)
{
	double idleSeconds = -1;
	io_iterator_t iter = 0;
	if (IOServiceGetMatchingServices(kIOMasterPortDefault, IOServiceMatching("IOHIDSystem"), &iter) == KERN_SUCCESS) {
		io_registry_entry_t entry = IOIteratorNext(iter);
		if (entry) {
			CFMutableDictionaryRef dict = NULL;
			kern_return_t status;
			status = IORegistryEntryCreateCFProperties(entry, &dict, kCFAllocatorDefault, 0);
			if (status == KERN_SUCCESS) {
				CFNumberRef obj = (CFNumberRef)CFDictionaryGetValue(dict, CFSTR("HIDIdleTime"));
				if (obj) {
					int64_t nanoseconds = 0;
					if (CFNumberGetValue(obj, kCFNumberSInt64Type, &nanoseconds)) {
						idleSeconds = (double) nanoseconds / NSEC_PER_SEC;
					}
				}
				CFRelease(dict);
			}
			IOObjectRelease(entry);
		}
		IOObjectRelease(iter);
	}
	return idleSeconds;
}
