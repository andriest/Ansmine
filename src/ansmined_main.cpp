

#include <QApplication>
#include <QtCore/QDebug>
#include <QtGui/QMainWindow>

#ifdef Q_WS_MAC
# include <Carbon/Carbon.h>
#endif

#include <iostream>

#include "config.hpp"
#include "tray.hpp"



using namespace std;

int main(int argc, char** argv)
{
	cout << "Ansmined v" VERSION_ALL_STR << endl;
    

    
    QApplication app(argc, argv);
    
    app.setQuitOnLastWindowClosed(true);
    
    Tray tray;
    
#ifdef Q_WS_MAC
    // in mac we won't see any dock icon.
    ProcessSerialNumber psn = {0, kCurrentProcess};
    TransformProcessType(&psn, kProcessTransformToBackgroundApplication);
    qDebug() << "in mac";
#endif
    
	return app.exec();
}


