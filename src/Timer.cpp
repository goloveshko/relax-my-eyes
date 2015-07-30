#include "Timer.h"

Timer::Timer(QObject *parent)
	: QTimer(parent)
	, timeLeft( - 1 )
//	, timerStatus( eTimerStatusNone )
{
}

Timer::~Timer()
{
}

void Timer::start(int msec)
{
	timeLeft = -1;
	QTimer::start( msec );
}

void Timer::start()
{
	if( 0 < timeLeft )
	{
		start( timeLeft );
	}
	else
	{
		QTimer::start();
	}
}

void Timer::stop()
{
	timeLeft = -1;
	QTimer::stop();
}

void Timer::pause()
{
	if( -1 == timeLeft )
	{
		timeLeft = remainingTime();
		QTimer::stop();
	}
//	else
//	{
//		start( timeLeft );
//	}
}

void Timer::resume()
{
	if( 0 < timeLeft )
	{
		start( timeLeft );
	}
}

void Timer::setState( Timer::TimerState state )
{
	timerState = state;
}

Timer::TimerState Timer::getState() const
{
	return timerState;
}

int Timer::remainingTime() const
{
	int result = -1;

	if( -1 == timeLeft )
	{
		result = QTimer::remainingTime();
	}
	else
	{
		result = timeLeft;
	}

	return result;
}
