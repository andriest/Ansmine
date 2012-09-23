

#include <iostream>

#include <QtCore/QDebug>
#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>

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
        << " (" << settings.value("userId").toInt() << ")" << settings.value("userPass").toString();
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
    
    initSettings();
    
    AnsmineMainwindow win;
    
    win.show();
    
    // run Ansmined
    
    QString path = QCoreApplication::applicationDirPath() + "/Ansmined";
    
    qDebug() << "running " << path;
    
    QProcess ps;;
    ps.startDetached(path, QStringList(), QCoreApplication::applicationDirPath());
    
    
	return app.exec();
}


