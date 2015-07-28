#pragma once

#include <QDialog>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QPointer>
#include <QMap>
#include <QTime>

#include "Timer.h"

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
	void slotUpdateMicroBreakEvery();
	void slotUpdateRestBreakEvery();

	void slotDebug();

	void slotTimeChanged( const QTime &time = QTime() );

	void slotHelp();
	void slotRestoreDefaults();

private:
	Ui::Options *ui;

	QPointer< QAction > showAction;
	QPointer< QAction >	quitAction;

	QPointer< QSystemTrayIcon >	trayIcon;
	QPointer< QMenu >			trayIconMenu;

	Timer timerMicroBreakEvery;
	Timer timerRestBreakEvery;

	Timer timerDebug;
	QTime timeDebug;

	int microBreak;
	int microBreakEvery;

	int restBreak;
	int restBreakEvery;

};

