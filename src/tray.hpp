



#ifndef __TRAY_HPP__
# define __TRAY_HPP__

#include <QtGui/QWidget>

class QMenu;
class QAction;
class QSystemTrayIcon;
//class QWidget;
class RedmineClient;

class Tray: public QWidget 
{
    Q_OBJECT
    
public:
    Tray();
    ~Tray();
    
public slots:
    void quit();
    
private:
    void createActions();
    void createTrayIcons();
    
    QMenu* trayIconMenu;
    QAction* quitAction;
    QSystemTrayIcon* trayIcon;
    RedmineClient* redmine;
};


#endif //__TRAY_HPP__


