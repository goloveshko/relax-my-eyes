#include "Options.h"
#include "ui_Options.h"

#include "MessageBoxBreak.h"
#include <QPushButton>
#include <QDesktopServices>
#include <QUrl>
#include <QSound>
#include <QDir>

#include <QSettings>

#include <QMessageBox>
#include <QDebug>

#define PAUSE_TIMER_AFTER_SEC	30

#define TIME_MICRO_BREAK "Time Micro Break"
#define TIME_MICRO_BREAK_EVERY "Time Micro Break Every"
#define TIME_REST_BREAK "Time Rest Break"
#define TIME_REST_BREAK_EVERY "Time Rest Every"

#define ALERT "Alert"
#define ALERT_VOLUME "Alert Volume"

Options::Options(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::Options)
	, timeCheckIdleTime( 0, 0 )
	, currentTime( QTime::currentTime() )
	, isTimersPaused( false )
{
	ui->setupUi(this);

	setWindowFlags( Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint );

	createActions();
	createTrayIcon();
	createComboBoxAlert();

	trayIcon->show();

	connect( &timerMicroBreakEvery, SIGNAL( timeout() ), this, SLOT( slotUpdateMicroBreakEvery() ) );
	connect( &timerRestBreakEvery, SIGNAL( timeout() ), this, SLOT( slotUpdateRestBreakEvery() ) );

	connect( ui->buttonBox->button( QDialogButtonBox::Close ), SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( ui->buttonBox->button( QDialogButtonBox::Help ), SIGNAL( clicked() ), this, SLOT( slotHelp() ) );
	connect( ui->buttonBox->button( QDialogButtonBox::RestoreDefaults ), SIGNAL( clicked() ), this, SLOT( slotRestoreDefaults() ) );

	connect( ui->timeEditMicroBreak, SIGNAL( timeChanged(const QTime &) ), this, SLOT( slotTimeChanged(const QTime &) ) );
	connect( ui->timeEditMicroBreakEvery, SIGNAL( timeChanged(const QTime &) ), this, SLOT( slotTimeChanged(const QTime &) ) );
	connect( ui->timeEditRestBreak, SIGNAL( timeChanged(const QTime &) ), this, SLOT( slotTimeChanged(const QTime &) ) );
	connect( ui->timeEditRestBreakEvery, SIGNAL( timeChanged(const QTime &) ), this, SLOT( slotTimeChanged(const QTime &) ) );

	connect( ui->sliderVolume, SIGNAL( sliderReleased() ), this, SLOT( slotVolumeChanged() ) );

	timerCheckIdleTime.start( 400 );
	connect( &timerCheckIdleTime, SIGNAL( timeout() ), this, SLOT( slotCheckIdleTime() ) );

	ui->labelDebug->hide();

	loadSettings();
}

Options::~Options()
{
	saveSettings();
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

void Options::createComboBoxAlert()
{
	foreach( QFileInfo var, sound.getSoundInfoList() )
	{
		ui->comboBoxAlert->addItem( var.baseName(), var.filePath() );
	}

	connect( ui->comboBoxAlert, SIGNAL( activated(int) ), this, SLOT( slotAlertChanged(int) ) );
}

void Options::slotShow()
{
	if( isVisible() )
	{
		hide();
	}
	else
	{
		show();
	}

	//raise();
	//activateWindow();
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
	timerCheckIdleTime.stop();

	MessageBoxBreak msg( tr( "micro break" ), microBreak );
	connect( &msg, SIGNAL( signalPlaySound() ), this, SLOT( slotPlaySound() ) );
	msg.exec();

	timerMicroBreakEvery.start( microBreakEvery );

	int iter = timerMicroBreakEvery.interval();
	int remTime = timerRestBreakEvery.remainingTime();

	if( remTime * 2 < iter )
	{
		remTime += iter / 4;
	}

	timerRestBreakEvery.start( remTime );
	timerCheckIdleTime.start();
}

void Options::slotUpdateRestBreakEvery()
{
	timerMicroBreakEvery.stop();
	timerRestBreakEvery.stop();
	timerCheckIdleTime.stop();

	MessageBoxBreak msg( tr( "rest break" ), restBreak );
	connect( &msg, SIGNAL( signalPlaySound() ), this, SLOT( slotPlaySound() ) );
	msg.exec();

	timerMicroBreakEvery.start( microBreakEvery );
	timerRestBreakEvery.start( restBreakEvery );
	timerCheckIdleTime.start();
}

void Options::slotCheckIdleTime()
{
	int interval = timerCheckIdleTime.interval();
	timeCheckIdleTime = timeCheckIdleTime.addMSecs( interval );

	int remainingTimeMicroBreak = timerMicroBreakEvery.remainingTime();
	int remainingTimeRestBreak = timerRestBreakEvery.remainingTime();

	QTime timeMicroBreak( 0, 0 );
	timeMicroBreak = timeMicroBreak.addMSecs( qMax( 0, remainingTimeMicroBreak ) );

	QTime timeRestBreak( 0, 0 );
	timeRestBreak = timeRestBreak.addMSecs( qMax( 0, remainingTimeRestBreak ) );

	actionTakeMicroBreak->setText( tr("Next micro break in ") + timeMicroBreak.toString() );
	actionTakeRestBreak->setText( tr("Next rest break in ") + timeRestBreak.toString() );


	int intervalTimeMicroBreak = timerMicroBreakEvery.interval();
	int intervalTimeRestBreak = timerRestBreakEvery.interval();
	int msecs = currentTime.msecsTo( QTime::currentTime() );

	if( !isTimersPaused && ( intervalTimeMicroBreak < msecs || intervalTimeRestBreak < msecs ) )
	{
		timerMicroBreakEvery.stop();
		timerMicroBreakEvery.start( microBreakEvery );
		//qDebug() << "intervalTimeMicroBreak" << intervalTimeMicroBreak / 1000 << msecs << currentTime << microBreakEvery / 1000;
		//QMessageBox::information( NULL, "timerMicroBreakEvery reset", "timerMicroBreakEvery reset");

		if( intervalTimeRestBreak < msecs )
		{
			timerRestBreakEvery.stop();
			timerRestBreakEvery.start( restBreakEvery );
			//qDebug() << "intervalTimeRestBreak" << intervalTimeRestBreak / 1000 << msecs << currentTime << restBreakEvery / 1000;
			//QMessageBox::information( NULL, "timerRestBreakEvery reset", "timerRestBreakEvery reset");
		}
		currentTime = QTime::currentTime();

		return;
	}

	double it = systemIdleTime.getIdleTime();

	if( !isTimersPaused )
	{
		currentTime = QTime::currentTime();
	}

	if( !isTimersPaused && PAUSE_TIMER_AFTER_SEC < it )
	{
		timerMicroBreakEvery.pause();
		timerRestBreakEvery.pause();
		isTimersPaused = true;
		//qDebug() << "Pause" << it << intervalTimeMicroBreak / 1000 << intervalTimeRestBreak / 1000 << msecs << QTime::currentTime();
	}
	else if( isTimersPaused && PAUSE_TIMER_AFTER_SEC > it )
	{
		timerMicroBreakEvery.resume();
		timerRestBreakEvery.resume();
		isTimersPaused = false;
		//qDebug() << "Resume" << it << intervalTimeMicroBreak / 1000 << intervalTimeRestBreak / 1000 << msecs << QTime::currentTime();
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

	ui->comboBoxAlert->setCurrentIndex( 0 );

	ui->sliderVolume->setValue( 50 );

	slotTimeChanged();
}

void Options::slotHelp()
{
	QString domain = qApp->organizationDomain();
	QDesktopServices::openUrl( QUrl( domain, QUrl::TolerantMode ) );
}

void Options::alertChanged( bool playSound )
{
	QString path = ui->comboBoxAlert->itemData( ui->comboBoxAlert->currentIndex() ).toString();

	sound.setPath( path );

	if( playSound )
	{
		sound.play();
	}
}

void Options::slotAlertChanged( int index )
{
	Q_UNUSED( index )
	alertChanged( true );
}

void Options::slotVolumeChanged()
{
	int value = ui->sliderVolume->value();

	float volume = (float)value / 100.0f;

	volume = qMax( 0.0f, volume );
	volume = qMin( 1.0f, volume );
	sound.setVolume( volume );
	sound.play();
}

void Options::slotPlaySound()
{
	sound.play();
}

void Options::saveSettings()
{
	QSettings settings("goloveshko", "RelaxMyEyes");

	QTime timeMicroBreak = ui->timeEditMicroBreak->time();
	QTime timeMicroBreakEvery = ui->timeEditMicroBreakEvery->time();
	QTime timeRestBreak = ui->timeEditRestBreak->time();
	QTime timeRestBreakEvery = ui->timeEditRestBreakEvery->time();

	int index = ui->comboBoxAlert->currentIndex();

	int value = ui->sliderVolume->value();

	settings.setValue( TIME_MICRO_BREAK,		timeMicroBreak );
	settings.setValue( TIME_MICRO_BREAK_EVERY,	timeMicroBreakEvery );
	settings.setValue( TIME_REST_BREAK,			timeRestBreak );
	settings.setValue( TIME_REST_BREAK_EVERY,	timeRestBreakEvery );

	settings.setValue( ALERT,					index );
	settings.setValue( ALERT_VOLUME,			value );
}

void Options::loadSettings()
{
	QSettings settings("goloveshko", "RelaxMyEyes");

	QTime timeMicroBreak = settings.value( TIME_MICRO_BREAK, QTime( 0, 1, 0 ) ).toTime();
	QTime timeMicroBreakEvery = settings.value( TIME_MICRO_BREAK_EVERY, QTime( 0, 10, 0 ) ).toTime();
	QTime timeRestBreak = settings.value( TIME_REST_BREAK, QTime( 0, 10, 0 ) ).toTime();
	QTime timeRestBreakEvery = settings.value( TIME_REST_BREAK_EVERY, QTime( 0, 45, 0 ) ).toTime();

	int index = settings.value( ALERT, 0 ).toInt();
	int value = settings.value( ALERT_VOLUME, 50 ).toInt();

	ui->timeEditMicroBreak->setTime( timeMicroBreak );
	ui->timeEditMicroBreakEvery->setTime( timeMicroBreakEvery );
	ui->timeEditRestBreak->setTime( timeRestBreak );
	ui->timeEditRestBreakEvery->setTime( timeRestBreakEvery );

	ui->comboBoxAlert->setCurrentIndex( index );

	ui->sliderVolume->setValue( value );

	slotTimeChanged();
	alertChanged( false );
	slotVolumeChanged();
}
