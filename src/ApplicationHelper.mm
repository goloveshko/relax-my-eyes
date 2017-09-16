#include "ApplicationHelper.h"
#import <AppKit/AppKit.h>
#include <QDebug>

ApplicationHelper::ApplicationHelper()
{
}

ApplicationHelper::~ApplicationHelper()
{
}

void ApplicationHelper::bringToFront()
{
	NSArray*  windows = (NSArray*)CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
	NSUInteger count = [windows count];

	for( NSUInteger i = 0; i < count; i++ )
	{
		NSDictionary*   nswindowsdescription = [windows objectAtIndex:i];
		NSNumber* windowid = (NSNumber*)[nswindowsdescription objectForKey:@"kCGWindowNumber"];
		//NSString* windowName = (NSString*)[nswindowsdescription objectForKey:@"kCGWindowOwnerName"];
		NSInteger pid = [[nswindowsdescription objectForKey:(id)kCGWindowOwnerPID] integerValue];
		//NSString *windowName1 = [nswindowsdescription objectForKey:(id)kCGWindowName];
		NSInteger layer = [[nswindowsdescription objectForKey:(id)kCGWindowLayer] integerValue];

		if( windowid && layer == 0 )
		{
			//qDebug() << [windowName UTF8String] << [[windowid stringValue] UTF8String] << pid;

			[[NSRunningApplication runningApplicationWithProcessIdentifier: pid] activateWithOptions:(NSApplicationActivateAllWindows | NSApplicationActivateIgnoringOtherApps)];

			break;
		}
	}
}
