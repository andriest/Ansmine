

#include <iostream>

#include <QtCore/QDebug>
#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>
#include <QtCore/QSharedMemory>

#include "config.hpp"
#include "ansmine.hpp"

using namespace std;


void initSettings(){
    QCoreApplication::setOrganizationName("Ansvia");
    QCoreApplication::setOrganizationDomain("ansvia.com");
    QCoreApplication::setApplicationName("Ansmine");
    
    QSettings settings;
    
    settings.beginGroup("redmine");
    
    if (!settings.contains("userName") || !settings.contains("userPass")
        || !settings.contains("userId")) {
        qDebug() << "account not configured";
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
	cout << "Ansmine v" VERSION_ALL_STR << endl;
    
    QApplication app(argc, argv);
    
    QStringList args = QApplication::arguments();
    
    if ( !args.contains("--multi") ) {
        // detect is already running.
        QSharedMemory mem("AnsminePid");
        if (!mem.create(1)) {
            qDebug() << "Another instance already running";
            return 1;
        }
    }else{
        qDebug() << "multiple instance allowed";
    }
    
    initSettings();
    
    AnsmineMainwindow win;
    
    win.show();
    
    bool runTray = true;
    if (argc > 1) {
        if (QString(argv[1]) == "--open") {
            runTray = false;
        }
    }
    
    if (runTray) {
        // run Ansmined
        
        QString path = QCoreApplication::applicationDirPath() + "/Ansmined";
        
        qDebug() << "running " << path;
        
        QProcess ps;;
        ps.startDetached(path, QStringList(), QCoreApplication::applicationDirPath());
        
    }
    
    
	return app.exec();
}


