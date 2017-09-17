#include "LaunchAgents.h"
#include <QDir>
#include <QCoreApplication>
#include <QSettings>
#include <QApplication>

LaunchAgents::LaunchAgents(QObject *parent)
	: QObject(parent)
{
}

LaunchAgents::~LaunchAgents()
{
}

void LaunchAgents::slotCheckBoxStartOnLogin(bool checked)
{
#ifdef Q_OS_MAC
	QDir autorunDir = QDir::home();
	
	if (!autorunDir.cd("Library") || !autorunDir.cd("LaunchAgents"))
	{
		return;
	}
	
	QString label = "org." + QCoreApplication::applicationName();
	label = label.toLower();
	label.replace(' ', '.');
	QString autorunFilePath = autorunDir.absoluteFilePath(label + ".plist");
	QSettings autorunSettings(autorunFilePath, QSettings::NativeFormat);
	autorunSettings.setValue("Label", label);
	autorunSettings.setValue("Program", QApplication::applicationFilePath());
	autorunSettings.setValue("RunAtLoad", checked);
	autorunSettings.setValue("ProcessType", "InterActive");
#elif defined(Q_OS_WIN)
	QSettings autorunSettings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
	QString keyName = QString("%1Wallet").arg(CurrencyAdapter::instance().getCurrencyDisplayName());
	if(checked)
	{
		autorunSettings.setValue(keyName, QDir::toNativeSeparators(QApplication::applicationFilePath()));
	}
	else
	{
		autorunSettings.remove(keyName);
	}
#endif
}
