

#include <QtCore/QDebug>
#include <QtGui/QWidget>
#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtGui/QSystemTrayIcon>

#include "tray.hpp"

Tray::Tray():
    QWidget(),
    trayIconMenu(new QMenu(this))
{
    createActions();
    createTrayIcons();
}

Tray::~Tray()
{
    
}

void Tray::createActions(){
    quitAction = new QAction(tr("Quit"), this);
}

void Tray::createTrayIcons(){
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}