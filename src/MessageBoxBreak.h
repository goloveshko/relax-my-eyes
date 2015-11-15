#pragma once

#include <QDialog>
#include <QTimer>
#include <QTime>

namespace Ui {
class MessageBoxBreak;
}

class MessageBoxBreak : public QDialog
{
	Q_OBJECT

public:
	explicit MessageBoxBreak( const QString &title, int inter, QWidget *parent = 0 );
	~MessageBoxBreak();

	void setInterval( int inter );

protected:
	void showEvent(QShowEvent * event);

protected slots:
	void slotUpdateTime();

signals:
	void signalPlaySound();

private:
	Ui::MessageBoxBreak *ui;
	int interval;
	QTimer timer;
	QTimer currentTimer;
	QTime currentTime;
};

