#include "LaunchAgents.h"
#include <QDir>
#include <QCoreApplication>
#include <QSettings>
#include <QApplication>
#include <QStandardPaths>
#include <QTextStream>

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
#elif defined(Q_OS_UNIX)
	QString autostartPath = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).at(0) + QLatin1String("/autostart");
	QDir autorunDir(autostartPath);
	if(!autorunDir.exists())
	{
		autorunDir.mkpath(autostartPath);
	}

	QString appName = QCoreApplication::applicationName();

	QString autorunFilePath = autorunDir.filePath("Autorun " + appName + ".desktop");

	QFile autorunFile(autorunFilePath);

	if(checked)
	{
		if(!autorunFile.exists())
		{
			if(autorunFile.open(QFile::WriteOnly))
			{

				QString autorunContent("[Desktop Entry]\n"
									   "Type=Application\n"
									   "Exec=" + QCoreApplication::applicationFilePath() + "\n"
																						   "Hidden=false\n"
																						   "NoDisplay=false\n"
																						   "X-GNOME-Autostart-enabled=true\n"
																						   "Name[en_GB]=Autorun " + appName + "\n"
																						   "Name=Autorun " + appName + "\n"
																						   "Comment[en_GB]=Autorun " + appName + " at system startup\n"
																						   "Comment=Autorun " + appName + " at system startup\n");
				QTextStream outStream(&autorunFile);
				outStream << autorunContent;
				autorunFile.setPermissions(QFileDevice::ExeUser|QFileDevice::ExeOwner|QFileDevice::ExeOther|QFileDevice::ExeGroup|
										   QFileDevice::WriteUser|QFileDevice::ReadUser);
				autorunFile.close();
			}
		}
	}
	else
	{
		if(autorunFile.exists())
		{
			autorunFile.remove();
		}
	}
#endif
}
