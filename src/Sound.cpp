#include "Sound.h"

Sound::Sound()
: volume( 1.0 )
{
}

Sound::~Sound()
{
}

void Sound::play()
{
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
	
	return result;
}
