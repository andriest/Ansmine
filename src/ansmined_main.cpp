

#include <QApplication>
#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtGui/QMainWindow>

#ifdef Q_WS_MAC
#include <Carbon/Carbon.h>
#include <ServiceManagement/SMLoginItem.h>
#endif

#include <iostream>

#include "config.hpp"
#include "tray.hpp"

using namespace std;

void initSettings(){
    QCoreApplication::setOrganizationName("Ansvia");
    QCoreApplication::setOrganizationDomain("ansvia.com");
    QCoreApplication::setApplicationName("Ansmine");
    
    QSettings settings;
    
    settings.beginGroup("redmine_account");
    
    if (!settings.contains("userName") || !settings.contains("userPass")) {
        
    }
    
}

int main(int argc, char** argv)
{
	cout << "Ansmined v" VERSION_ALL_STR << endl;
    

    QApplication app(argc, argv);
    
    app.setQuitOnLastWindowClosed(true);
    
    initSettings();
    
    Tray tray;
    
#ifdef Q_WS_MAC
    // in mac we won't any dock icon.
    ProcessSerialNumber psn = {0, kCurrentProcess};
    TransformProcessType(&psn, kProcessTransformToBackgroundApplication);
#endif
    
	return app.exec();
}


