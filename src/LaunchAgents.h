#pragma once

#include <QObject>

class LaunchAgents : public QObject
{
	Q_OBJECT
public:
	explicit LaunchAgents(QObject *parent = 0);
	~LaunchAgents();

public slots:
	void slotCheckBoxStartOnLogin(bool checked);
};

