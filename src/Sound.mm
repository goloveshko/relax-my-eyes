#include "Sound.h"
#import <AppKit/AppKit.h>

#include <QDir>
#include <QDebug>

Sound::Sound()
	: volume( 1.0 )
{
}

Sound::~Sound()
{
}

void Sound::play()
{
	NSString *path_ns = nil;

	if( !path.isEmpty() )
	{
		path_ns = [[NSString alloc]initWithUTF8String:path.toUtf8().data()];
		NSSound *sound = [[NSSound alloc] initWithContentsOfFile:path_ns byReference:YES];
		[sound setVolume:volume];
		[sound play];
		[sound release];
	}

	[path_ns release];
}

void Sound::setVolume( float vol )
{
	volume = vol;
}

void Sound::setPath( const QString &path )
{
	this->path = path;
}

QFileInfoList Sound::getSoundInfoList() const
{
	QFileInfoList result;

	QStringList filters;
	filters << "*.aiff";

	QString path("/System/Library/Sounds");

	QDir dir( path );
	dir.setNameFilters( filters );
	dir.setFilter( QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot );

	result = dir.entryInfoList();

	return result;
}

//https://github.com/mackuba/WindowFairy/blob/master/WindowManager.m
void Sound::getWindow()
{
	/*char *buffer = (char *)malloc(400);
	int layer;

	CFArrayRef windowList = CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly, kCGNullWindowID);

	CFIndex numWindows = CFArrayGetCount( windowList );

	for( int i = 0; i < (int)numWindows; i++ )
	{
		CFDictionaryRef info = (CFDictionaryRef)CFArrayGetValueAtIndex(windowList, i);

		CFStringRef appName = (CFStringRef)CFDictionaryGetValue(info, kCGWindowOwnerName);

		CFNumberGetValue((CFNumberRef)CFDictionaryGetValue(info, kCGWindowLayer),kCFNumberIntType, &layer);

		NSInteger ownerPID = [[info objectForKey:(id)kCGWindowOwnerPID] integerValue];

		if( appName != 0 )
		{
			CFStringGetCString(appName, buffer, 400, kCFStringEncodingUTF8);
			if (layer == 0)
			{
				qDebug() << buffer << "\n";
				[[NSRunningApplication runningApplicationWithProcessIdentifier: (pid_t)ownerPID] activateWithOptions:(NSApplicationActivateAllWindows | NSApplicationActivateIgnoringOtherApps)];
				break;
			}
		}
	}

	CFRelease(windowList);*/

	//CFArrayRef windowArray = CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
	//NSMutableArray *windowsInMap = [NSMutableArray arrayWithCapacity:64];
	NSArray*  windows = (NSArray*)CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
	NSUInteger count = [windows count];

	for (NSUInteger i = 0; i < count; i++)
	{
		NSDictionary*   nswindowsdescription = [windows objectAtIndex:i];
		NSNumber* windowid = (NSNumber*)[nswindowsdescription objectForKey:@"kCGWindowNumber"];
		NSString* windowName = (NSString*)[nswindowsdescription objectForKey:@"kCGWindowOwnerName"];
		NSInteger ownerPID = [[nswindowsdescription objectForKey:(id)kCGWindowOwnerPID] integerValue];
		//NSString *windowName1 = [nswindowsdescription objectForKey:(id)kCGWindowName];
		NSString *windowName1 = [nswindowsdescription objectForKey:(id)kCGWindowName];
		NSInteger layer = [[nswindowsdescription objectForKey:(id)kCGWindowLayer] integerValue];


		if( windowid && layer == 0 )
		{
			qDebug() << [windowName UTF8String] << [[windowid stringValue] UTF8String] << ownerPID;

			[[NSRunningApplication runningApplicationWithProcessIdentifier: (pid_t)ownerPID] activateWithOptions:(NSApplicationActivateAllWindows | NSApplicationActivateIgnoringOtherApps)];


			//app = [NSRuningApplication runningApplicationWithProcessIdentifier: windowid];
			//[app activateWithOptions: (NSApplicationActivateAllWindows | NSApplicationActivateIgnoringOtherApps)]

			break;
		}
	}
	//CFRelease(windowArray);

/*
	CFArrayRef windowArray = CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
	NSMutableArray *windowsInMap = [NSMutableArray arrayWithCapacity:64];
	NSArray*  windows = (NSArray*)CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
	NSUInteger count = [windows count];

	for (NSUInteger i = 0; i < count; i++)
	{
		NSDictionary*   nswindowsdescription = [windows objectAtIndex:i];
		NSNumber* windowid = (NSNumber*)[nswindowsdescription objectForKey:@"kCGWindowNumber"];
		NSString* windowName = (NSString*)[nswindowsdescription objectForKey:@"kCGWindowOwnerName"];
		NSInteger ownerPID = [[nswindowsdescription objectForKey:(id)kCGWindowOwnerPID] integerValue];
		NSString *windowName1 = [nswindowsdescription objectForKey:(id)kCGWindowName];


		if( windowid )
		{
			qDebug() << [windowName UTF8String] << [windowName1 UTF8String] << [[windowid stringValue] UTF8String] << ownerPID;

			//[[NSRunningApplication runningApplicationWithProcessIdentifier: (pid_t)ownerPID] activateWithOptions:(NSApplicationActivateAllWindows | NSApplicationActivateIgnoringOtherApps)];


			//app = [NSRuningApplication runningApplicationWithProcessIdentifier: windowid];
			//[app activateWithOptions: (NSApplicationActivateAllWindows | NSApplicationActivateIgnoringOtherApps)]

			//break;
		}
	}
	CFRelease(windowArray);
*/


}
