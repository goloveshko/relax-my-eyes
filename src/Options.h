#pragma once

#include <QDialog>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QPointer>
#include <QMap>
#include <QTime>

#include "Timer.h"
#include "SystemIdleTime.h"

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

private:
	Ui::Options *ui;

	QPointer< QAction > actionShow;
	QPointer< QAction >	actionQuit;
	QPointer< QAction >	actionTakeMicroBreak;
	QPointer< QAction >	actionTakeRestBreak;

	QPointer< QSystemTrayIcon >	trayIcon;
	QPointer< QMenu >			trayIconMenu;

	Timer timerMicroBreakEvery;
	Timer timerRestBreakEvery;

	Timer timerCheckIdleTime;
	QTime timeCheckIdleTime;

	int microBreak;
	int microBreakEvery;

	int restBreak;
	int restBreakEvery;

	SystemIdleTime systemIdleTime;
};

