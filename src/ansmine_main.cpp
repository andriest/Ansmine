

#include <iostream>

#include <QtCore/QProcess>

#include "config.hpp"
#include "ansmine.hpp"

using namespace std;

int main(int argc, char** argv)
{
	cout << "Ansmine v" VERSION_ALL_STR << endl;
    
    QApplication app(argc, argv);
    
    AnsmineMainwindow win;
    
    win.show();
    
	return app.exec();
}


