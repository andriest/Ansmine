



#ifndef __TRAY_HPP__
# define __TRAY_HPP__

#include <QtGui/QWidget>

class QMenu;
class QAction;
class QSystemTrayIcon;

class Tray: public QWidget 
{
    Q_OBJECT
    
public:
    Tray();
    ~Tray();
    
private:
    void createActions();
    void createTrayIcons();
    
    QMenu* trayIconMenu;
    QAction* quitAction;
    QSystemTrayIcon* trayIcon;
};


#endif //__TRAY_HPP__


