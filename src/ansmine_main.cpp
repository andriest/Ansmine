

#include <iostream>

#include <QtCore/QDebug>
#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>

#include "config.hpp"
#include "ansmine.hpp"

using namespace std;

int main(int argc, char** argv)
{
	cout << "Ansmine v" VERSION_ALL_STR << endl;
    
    QApplication app(argc, argv);
    
    AnsmineMainwindow win;
    
    win.show();
    
    // run Ansmined
    
    QString path = QCoreApplication::applicationDirPath() + "/Ansmined";
    
    qDebug() << "running " << path;
    
    QProcess ps;;
    ps.startDetached(path, QStringList(), QCoreApplication::applicationDirPath());
    
    
	return app.exec();
}


