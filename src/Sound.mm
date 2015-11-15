#include "Sound.h"
#import <AppKit/AppKit.h>

#include <QDir>

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
