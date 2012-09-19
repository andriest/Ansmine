

#include <QApplication>
#include <QtGui/QMainWindow>


#include <iostream>

#include "config.hpp"
#include "tray.hpp"



using namespace std;

int main(int argc, char** argv)
{
	cout << "Ansmined v" VERSION_ALL_STR << endl;
    
    QApplication app(argc, argv);;
    
    Tray tray;
    tray.setVisible(true);
    
	return app.exec();
}


