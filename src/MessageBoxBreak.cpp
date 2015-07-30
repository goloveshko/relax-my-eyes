#include "MessageBoxBreak.h"
#include "ui_MessageBoxBreak.h"
#include <QTime>
#include <QPushButton>

MessageBoxBreak::MessageBoxBreak( const QString &title, int inter, QWidget *parent )
	: QDialog( parent )
	, ui(new Ui::MessageBoxBreak)
	, interval( inter )
{
	ui->setupUi( this );

	setWindowTitle( tr("Take a %1").arg( title ) );

	//setWindowFlags( Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint );
	setWindowFlags( Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint );

	connect( ui->buttonBox->button( QDialogButtonBox::Abort ), SIGNAL( clicked() ), this, SLOT( reject() ) );

	timer.setInterval( interval );
	connect( &timer, SIGNAL( timeout() ), this, SLOT( accept() ) );

	timerTime.setInterval( 100 );
	connect( &timerTime, SIGNAL( timeout() ), this, SLOT( slotUpdateTime() ) );

	timer.start();
	timerTime.start();

	QApplication::alert( this );
	QApplication::beep();
}

MessageBoxBreak::~MessageBoxBreak()
{
	delete ui;
	QApplication::beep();
}

void MessageBoxBreak::slotUpdateTime()
{
	QTime time( 0, 0 );
	time = time.addMSecs( timer.remainingTime() );

	QString str = time.toString();

	ui->labelTimer->setText( str );
}

void MessageBoxBreak::setInterval( int inter )
{
	interval = inter;

	timer.setInterval( interval );
}
