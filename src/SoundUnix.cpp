#include "Sound.h"
#include <QDirIterator>
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

	QDirIterator it("/usr/share/sounds", QStringList() << "*.ogg" << "*.wav", QDir::Files, QDirIterator::Subdirectories);

	while (it.hasNext())
	{
		result << it.next();
	}

	return result;
}
