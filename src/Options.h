#pragma once

#include <QDialog>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QPointer>
#include <QMap>
#include <QTime>

#include "Timer.h"
#include "SystemIdleTime.h"
#include "Sound.h"
#include "LaunchAgents.h"

namespace Ui {
class Options;
}

class Options : public QDialog
{
	Q_OBJECT

public:
	explicit Options(QWidget *parent = 0);
	~Options();

protected:
	void createActions();
	void createTrayIcon();
	void createComboBoxAlert();
	void saveSettings();
	void loadSettings();
	void alertChanged( bool playSound );

protected slots:
	void slotShow();
	void slotAboutToShowTrayMenu();
	void slotAboutToHideTrayMenu();
	void slotUpdateMicroBreakEvery();
	void slotUpdateRestBreakEvery();

	void slotCheckIdleTime();

	void slotTimeChanged( const QTime &time = QTime() );

	void slotHelp();
	void slotRestoreDefaults();

	void slotAlertChanged( int index );

	void slotVolumeChanged();

	void slotPlaySound();

private:
	Ui::Options *ui;

	QPointer< QAction > actionShow;
	QPointer< QAction >	actionQuit;
	QPointer< QAction >	actionTakeMicroBreak;
	QPointer< QAction >	actionTakeRestBreak;

	QPointer< QSystemTrayIcon >	trayIcon;
	QPointer< QMenu >			trayIconMenu;
	
	QPointer<LaunchAgents> 		launchAgents;

	Timer timerMicroBreakEvery;
	Timer timerRestBreakEvery;

	Timer timerCheckIdleTime;
	QTime timeCheckIdleTime;

	QTime currentTime;

	int microBreak;
	int microBreakEvery;

	int restBreak;
	int restBreakEvery;

	SystemIdleTime systemIdleTime;

	Sound sound;

	bool isTimersPaused;
};

