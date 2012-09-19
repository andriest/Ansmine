

#include <QtGui/QApplication>
#include <QtGui/QWidget>
#include <QtGui/QDesktopWidget>
#include "ansmine.hpp"


AnsmineMainwindow::AnsmineMainwindow(QObject* parent)
{
    setupUi(this);
    
    QDesktopWidget dw;
    QRect screenGeometry = dw.screenGeometry();
    int x = (screenGeometry.width()-this->width()) / 2;
    int y = (screenGeometry.height()-this->height()) / 2;
    this->move(x, y);
    this->show();
}
