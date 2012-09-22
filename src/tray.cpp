

#include <QtCore/QDebug>
#include <QtGui/QWidget>
#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtGui/QSystemTrayIcon>
#include <QtGui/QIcon>

#include "tray.hpp"
#include "redmine.hpp"

Tray::Tray():
    QWidget(),
    trayIconMenu(new QMenu(this))
{
    this->setWindowIcon(QIcon(":/digaku-logo_16x16x32.png"));
    
    createActions();
    createTrayIcons();
    
    this->hide();
    
    redmine = new RedmineClient("redmine.digaku.com");
    
}

Tray::~Tray()
{
    delete trayIconMenu;
    disconnect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));
    delete quitAction;
}

void Tray::createActions(){
    quitAction = new QAction(tr("Quit"), this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));
}


void Tray::createTrayIcons(){
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/digaku-logo_16x16x32.png"));
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->show();
}

void Tray::quit(){
    this->close();
}

