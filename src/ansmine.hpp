


#ifndef __ANSMINE_HPP__
# define __ANSMINE_HPP__

#include <QtCore>
#include <QtGui/QMainWindow>
#include "ui_main.h"

class QWidget;

class AnsmineMainwindow: public QMainWindow, private Ui::MainWindow {
    Q_OBJECT

public:
    AnsmineMainwindow(QObject* parent=0);

};


#endif // __ANSMINE_HPP__
