#pragma once

#include <QDialog>
#include <QTime>
#include <QTimer>
#include "ApplicationHelper.h"

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
	void showEvent(QShowEvent *event);
	void leaveEvent(QEvent *event);
	void moveEvent(QMoveEvent *event);

protected slots:
	void slotUpdateTime();
	void slotSkip();

signals:
	void signalPlaySound();

private:
	Ui::MessageBoxBreak *ui;
	int interval;
	QTimer timer;
	QTimer currentTimer;
	QTime currentTime;
	ApplicationHelper helper;
};

