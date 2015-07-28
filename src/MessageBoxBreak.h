#pragma once

#include <QDialog>
#include <QTimer>

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

protected slots:
	void slotUpdateTime();

private:
	Ui::MessageBoxBreak *ui;
	int interval;
	QTimer timer;
	QTimer timerTime;
};

