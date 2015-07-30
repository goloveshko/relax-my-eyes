#include "Options.h"
#include "ui_Options.h"
#include "MessageBoxBreak.h"
#include <QPushButton>
#include <QDesktopServices>
#include <QUrl>
#include <QSound>

#include <QDebug>

#define PAUSE_TIMER_AFTER_SEC	30

Options::Options(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::Options)
	, timeCheckIdleTime( 0, 0 )
{
	ui->setupUi(this);

	setWindowFlags( Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint );

	createActions();
	createTrayIcon();

	trayIcon->show();

	connect( &timerMicroBreakEvery, SIGNAL( timeout() ), this, SLOT( slotUpdateMicroBreakEvery() ) );
	connect( &timerRestBreakEvery, SIGNAL( timeout() ), this, SLOT( slotUpdateRestBreakEvery() ) );

	//connect( ui->buttonBox->button( QDialogButtonBox::Cancel ), SIGNAL( clicked() ), this, SLOT( reject() ) );
	//connect( ui->buttonBox->button( QDialogButtonBox::Ok ), SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( ui->buttonBox->button( QDialogButtonBox::Close ), SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( ui->buttonBox->button( QDialogButtonBox::Help ), SIGNAL( clicked() ), this, SLOT( slotHelp() ) );
	connect( ui->buttonBox->button( QDialogButtonBox::RestoreDefaults ), SIGNAL( clicked() ), this, SLOT( slotRestoreDefaults() ) );

	connect( ui->timeEditMicroBreak, SIGNAL( timeChanged(const QTime &) ), this, SLOT( slotTimeChanged(const QTime &) ) );
	connect( ui->timeEditMicroBreakEvery, SIGNAL( timeChanged(const QTime &) ), this, SLOT( slotTimeChanged(const QTime &) ) );
	connect( ui->timeEditRestBreak, SIGNAL( timeChanged(const QTime &) ), this, SLOT( slotTimeChanged(const QTime &) ) );
	connect( ui->timeEditRestBreakEvery, SIGNAL( timeChanged(const QTime &) ), this, SLOT( slotTimeChanged(const QTime &) ) );

	slotTimeChanged();

	timerCheckIdleTime.start( 400 );
	connect( &timerCheckIdleTime, SIGNAL( timeout() ), this, SLOT( slotCheckIdleTime() ) );

	ui->labelDebug->hide();
}

Options::~Options()
{
	delete ui;
}

void Options::createActions()
{
	actionShow = new QAction(tr("&Show"), this);
	connect(actionShow, SIGNAL(triggered()), this, SLOT(slotShow()));

	actionQuit = new QAction(tr("&Quit"), this);
	connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

	actionTakeMicroBreak = new QAction(tr("Take a micro break"), this);
	connect(actionTakeMicroBreak, SIGNAL(triggered()), this, SLOT(slotUpdateMicroBreakEvery()));

	actionTakeRestBreak = new QAction(tr("Take a rest break"), this);
	connect(actionTakeRestBreak, SIGNAL(triggered()), this, SLOT(slotUpdateRestBreakEvery()));
}

void Options::createTrayIcon()
{
	trayIconMenu = new QMenu(this);
	trayIconMenu->addAction(actionTakeMicroBreak);
	trayIconMenu->addAction(actionTakeRestBreak);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(actionShow);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(actionQuit);

	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setContextMenu(trayIconMenu);

	QIcon icon = QIcon(":/images/tray.png");
	trayIcon->setIcon(icon);
	setWindowIcon(icon);

	trayIcon->setToolTip(tr("Relax My Eyes"));

	connect( trayIconMenu, SIGNAL(aboutToShow()), this, SLOT(slotAboutToShowTrayMenu()) );
	connect( trayIconMenu, SIGNAL(aboutToHide()), this, SLOT(slotAboutToHideTrayMenu()) );
}

void Options::slotShow()
{
	//QSound::play("/System/Library/Sounds/Purr.aiff");
	if( isVisible() )
	{
		hide();
	}
	else
	{
		show();
	}

	//qApp->processEvents();
	raise();
	activateWindow();
}

void Options::slotAboutToShowTrayMenu()
{
	if( actionShow.isNull() )
	{
		return;
	}

	if( isVisible() )
	{
		actionShow->setText(tr("&Hide Options"));
	}
	else
	{
		actionShow->setText(tr("&Show Options"));
	}
}

void Options::slotAboutToHideTrayMenu()
{

}

void Options::slotUpdateMicroBreakEvery()
{
	timerMicroBreakEvery.stop();
	timerRestBreakEvery.pause();

	MessageBoxBreak msg( tr( "micro break" ), microBreak );
	msg.exec();

	timerMicroBreakEvery.start( microBreakEvery );

	int iter = timerMicroBreakEvery.interval();
	int remTime = timerRestBreakEvery.remainingTime();

	if( remTime * 2 < iter )
	{
		remTime += iter / 4;
	}

	timerRestBreakEvery.start( remTime );
}

void Options::slotUpdateRestBreakEvery()
{
	timerMicroBreakEvery.stop();
	timerRestBreakEvery.stop();

	MessageBoxBreak msg( tr( "rest break" ), restBreak );
	msg.exec();

	timerMicroBreakEvery.start( microBreakEvery );
	timerRestBreakEvery.start( restBreakEvery );
}

void Options::slotCheckIdleTime()
{
	int interval = timerCheckIdleTime.interval();
	timeCheckIdleTime = timeCheckIdleTime.addMSecs( interval );

	int remainingTime1 = timerMicroBreakEvery.remainingTime();
	int remainingTime2 = timerRestBreakEvery.remainingTime();

	QTime time1( 0, 0 );
	time1 = time1.addMSecs( qMax( 0, remainingTime1 ) );

	QTime time2( 0, 0 );
	time2 = time2.addMSecs( qMax( 0, remainingTime2 ) );

	QString str = time1.toString() + " " + time2.toString();

	ui->labelDebug->setText( str );

	actionTakeMicroBreak->setText( tr("Next micro break in ") + time1.toString() );
	actionTakeRestBreak->setText( tr("Next rest break in ") + time2.toString() );

	double it = systemIdleTime.getIdleTime();

	if( PAUSE_TIMER_AFTER_SEC < it )
	{
		timerMicroBreakEvery.pause();
		timerRestBreakEvery.pause();
		//qDebug() << "Pause" << it;
	}
	else
	{
		timerMicroBreakEvery.resume();
		timerRestBreakEvery.resume();
		//qDebug() << "Resume" << it;
	}
}

void Options::slotTimeChanged( const QTime &time )
{
	Q_UNUSED( time );

	QTime time0( 0, 0 );

	microBreak = time0.msecsTo( ui->timeEditMicroBreak->time() );
	microBreakEvery = time0.msecsTo( ui->timeEditMicroBreakEvery->time() );

	restBreak = time0.msecsTo( ui->timeEditRestBreak->time() );
	restBreakEvery = time0.msecsTo( ui->timeEditRestBreakEvery->time() );

	timerMicroBreakEvery.start( microBreakEvery );
	timerRestBreakEvery.start( restBreakEvery );
}

void Options::slotRestoreDefaults()
{
	ui->timeEditMicroBreak->setTime( QTime( 0, 1, 0 ) );
	ui->timeEditMicroBreakEvery->setTime( QTime( 0, 10, 0 ) );
	ui->timeEditRestBreak->setTime( QTime( 0, 10, 0 ) );
	ui->timeEditRestBreakEvery->setTime( QTime( 0, 45, 0 ) );

	slotTimeChanged();
}

void Options::slotHelp()
{
	QDesktopServices::openUrl( QUrl( "https://github.com/goloveshko/relax-my-eyes", QUrl::TolerantMode ) );
}
