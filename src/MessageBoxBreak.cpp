#include "MessageBoxBreak.h"
#include "ui_MessageBoxBreak.h"
#include <QTime>
#include <QPushButton>
#include <QDebug>

MessageBoxBreak::MessageBoxBreak( const QString &title, int inter, QWidget *parent )
	: QDialog( parent )
	, ui(new Ui::MessageBoxBreak)
	, interval( inter )
	, currentTime( QTime::currentTime() )
{
	ui->setupUi( this );

	setWindowTitle( tr("Take a %1").arg( title ) );

	setWindowFlags( Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint );

	connect( ui->buttonBox->button( QDialogButtonBox::Cancel ), SIGNAL( clicked() ), this, SLOT( slotSkip() ) );

	ui->buttonBox->button( QDialogButtonBox::Cancel )->setText( tr("Skip") );

	timer.setInterval( interval );
	connect( &timer, SIGNAL( timeout() ), this, SLOT( accept() ) );

	currentTimer.setInterval( 400 );
	connect( &currentTimer, SIGNAL( timeout() ), this, SLOT( slotUpdateTime() ) );

	timer.start();
	currentTimer.start();

    QMargins margins = ui->gridLayout->contentsMargins();
    setFixedSize(width() + margins.left() + margins.right(), height() + margins.top() + margins.bottom());
}

MessageBoxBreak::~MessageBoxBreak()
{
	emit signalPlaySound();
	delete ui;
}

void MessageBoxBreak::slotUpdateTime()
{
	int msecs = currentTime.msecsTo( QTime::currentTime() );
	int remTime = timer.remainingTime();

	if( remTime < msecs )
	{
		accept();
		return;
	}

	QTime time( 0, 0 );
	time = time.addMSecs( remTime );

	QString str = time.toString();

	ui->labelTimer->setText( str );

	currentTime = QTime::currentTime();
}

void MessageBoxBreak::setInterval( int inter )
{
	interval = inter;

	timer.setInterval( interval );
}

void MessageBoxBreak::showEvent(QShowEvent *event)
{
	Q_UNUSED( event )
	emit signalPlaySound();
}

void MessageBoxBreak::slotSkip()
{
	reject();
	helper.bringToFront();
}

void MessageBoxBreak::leaveEvent(QEvent *event)
{
	QDialog::leaveEvent(event);
	helper.bringToFront();
}

void MessageBoxBreak::moveEvent(QMoveEvent *event)
{
	QDialog::moveEvent(event);
	helper.bringToFront();
}
