



#ifndef __TRAY_HPP__
# define __TRAY_HPP__

#include <QtGui/QWidget>
#include <QtCore/QVariantMap>
#include <QtCore/QList>

#include "issue.hpp"

class QMenu;
class QAction;
class QSystemTrayIcon;
//class QWidget;
class RedmineClient;

class Tray: public QWidget 
{
    Q_OBJECT
    
public:
    Tray(const QString& baseRedmineUrl);
    ~Tray();
    
public slots:
    void quit();
    void onIssues(const QVariantMap& data);
    void onIssueClick();
    
private:
    void createActions();
    void createTrayIcons();
    void loadRedmine();
    void unloadRedmine();
    void rebuildMenu();
    
    QMenu* trayIconMenu;
    QAction* quitAction;
    QSystemTrayIcon* trayIcon;
    RedmineClient* redmine;
    const QString baseUrl;
    QList<Issue*>* issues;
};


#endif //__TRAY_HPP__


