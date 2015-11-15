#pragma once

#include <QTimer>

class Timer : public QTimer
{
	Q_OBJECT

enum TimerState
{
	None,
	MicroBreak,
	MicroBreakEvery,
	RestBreak,
	RestBreakEvery
};

public:
	explicit Timer(QObject *parent = 0);
	~Timer();

	void setState( TimerState state );
	TimerState getState() const;

	int remainingTime() const;

	void pause();
	void resume();

	void restart();

public slots:
	void start(int msec);

	void start();
	void stop();

private:
	int timeLeft;
	TimerState timerState;
};

