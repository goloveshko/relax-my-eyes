#include "Options.h"
#include "ui_Options.h"
#include "MessageBoxBreak.h"
#include <QPushButton>

Options::Options(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::Options)
	, timeDebug( 0, 0 )
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

	timerDebug.start( 1000 );
	connect( &timerDebug, SIGNAL( timeout() ), this, SLOT( slotDebug() ) );

	ui->labelDebug->hide();
}

Options::~Options()
{
	delete ui;
}

void Options::createActions()
{
	showAction = new QAction(tr("&Show"), this);
	connect(showAction, SIGNAL(triggered()), this, SLOT(slotShow()));

	quitAction = new QAction(tr("&Quit"), this);
	connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void Options::createTrayIcon()
{
	trayIconMenu = new QMenu(this);
	trayIconMenu->addAction(showAction);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(quitAction);

	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setContextMenu(trayIconMenu);

	QIcon icon = QIcon(":/images/tray.png");
	trayIcon->setIcon(icon);
	setWindowIcon(icon);

	trayIcon->setToolTip(tr("Relax My Eyes"));

	connect( trayIconMenu, SIGNAL(aboutToShow()), this, SLOT(slotAboutToShowTrayMenu()) );
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

	//qApp->processEvents();
	raise();
	activateWindow();
}

void Options::slotAboutToShowTrayMenu()
{
	if( showAction.isNull() )
	{
		return;
	}

	if( isVisible() )
	{
		showAction->setText(tr("&Hide Options"));
	}
	else
	{
		showAction->setText(tr("&Show Options"));
	}
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
		remTime += iter / 2;
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

void Options::slotDebug()
{
	int interval = timerDebug.interval();
	timeDebug = timeDebug.addMSecs( interval );

	int interval1 = timerMicroBreakEvery.interval();
	int remainingTime1 = timerMicroBreakEvery.remainingTime();
	int remainingTime2 = timerRestBreakEvery.remainingTime();
	int diff = interval1 - remainingTime1;

	QTime time1( 0, 0 );
	time1 = time1.addMSecs( remainingTime1 );

	QTime time2( 0, 0 );
	time2 = time2.addMSecs( remainingTime2 );

	QString str = time1.toString() + " " + time2.toString();

	ui->labelDebug->setText( str );
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

}
