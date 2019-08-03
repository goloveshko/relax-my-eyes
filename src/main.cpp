#include "Options.h"
#include <QApplication>

#include <QMessageBox>

#include <QDir>
#include <QProcessEnvironment>

#include <QSystemTrayIcon>

#define VERSION "0.0.0.1"
int main(int argc, char *argv[])
{
#ifdef Q_OS_MAC
	QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
	QString qtpath = env.value("QTDIR");
	QDir qtdir( qtpath );
	qtdir.cd( "plugins" );
	QCoreApplication::addLibraryPath( qtdir.absolutePath() );

	QDir dir( argv[0] );
	dir.cdUp();
	dir.cdUp();
	dir.cd("PlugIns");
	QCoreApplication::addLibraryPath(dir.absolutePath());
#endif

	QCoreApplication::setOrganizationName( "Goloveshko" );
	QCoreApplication::setOrganizationDomain( "https://github.com/goloveshko/relax-my-eyes/" );
	QCoreApplication::setApplicationVersion( VERSION );
	QCoreApplication::setApplicationName( "Relax my eyes" );


	QApplication a(argc, argv);

    QIcon icon = QIcon(":/images/tray.png");
    icon.setIsMask(true);
    a.setWindowIcon(icon);

	if( !QSystemTrayIcon::isSystemTrayAvailable() )
	{
		QMessageBox::critical(0, QObject::tr("Systray"), QObject::tr("I couldn't detect any system tray on this system."));
		return 1;
	}
	QApplication::setQuitOnLastWindowClosed(false);

	Options w;
	w.hide();
	//w.show();

	return a.exec();
}
