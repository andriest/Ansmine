

#include <QApplication>
#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtGui/QMainWindow>
#include <QtCore/QSharedMemory>

#ifdef Q_WS_MAC
#include <Carbon/Carbon.h>
#include <ServiceManagement/SMLoginItem.h>
#endif

#include <iostream>

#include "config.hpp"
#include "tray.hpp"
#include "login.hpp"

using namespace std;

void initSettings(){
    QCoreApplication::setOrganizationName("Ansvia");
    QCoreApplication::setOrganizationDomain("ansvia.com");
    QCoreApplication::setApplicationName("Ansmine");
    
    QSettings settings;
    
    settings.beginGroup("redmine");
    
    if (!settings.contains("userName") || !settings.contains("userPass")
        || !settings.contains("userId")) {
        LoginDialog loginDlg;
        loginDlg.show();
        
        int code = loginDlg.exec();
        
        if (code > 0) {
            qDebug() << "Login failed? code " + code;
        }
    }
    else{
        qDebug() << "Using Redmine account: " << settings.value("userName").toString() 
                    << " (" << settings.value("userId").toInt() << ")";
    }
    
    if (!settings.contains("host")) {
        settings.setValue("host", "redmine.digaku.com");
        settings.sync();
    }
}

int main(int argc, char** argv)
{
	cout << "Ansmined v" VERSION_ALL_STR << endl;
    

    QApplication app(argc, argv);
    
    // detect is already running.
    QSharedMemory mem("AnsminedPid");
    if (!mem.create(1)) {
        qDebug() << "Another instance already running";
        return 1;
    }
    
    
    app.setQuitOnLastWindowClosed(true);
    
    initSettings();
    
    Tray tray("http://redmine.digaku.com");
    
#ifdef Q_WS_MAC
    // in mac we won't any dock icon.
    ProcessSerialNumber psn = {0, kCurrentProcess};
    TransformProcessType(&psn, kProcessTransformToBackgroundApplication);
#endif
    
	return app.exec();
}


