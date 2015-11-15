//! Objective-C++ class to enable Cocoa calls in C++

#pragma once

#include <QObject>
#include <QFileInfoList>

class Sound
{
public:
	Sound();
	~Sound();

	void play();

	void setPath( const QString &path );
	void setVolume( float vol );

	QFileInfoList getSoundInfoList() const;

private:
	QString path;
	float volume;
};
