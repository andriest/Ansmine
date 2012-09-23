



#ifndef __TRAY_HPP__
# define __TRAY_HPP__

#include <QtGui/QWidget>
#include <QtCore/QVariantMap>
#include <QtCore/QList>

//#include "issue.hpp"

class QMenu;
class QAction;
class QSystemTrayIcon;
//class QWidget;
class RedmineClient;
class QTimer;
class IssueContainer;

class Tray: public QWidget 
{
    Q_OBJECT
    
public:
    Tray(const QString& baseRedmineUrl);
    ~Tray();
    
    void playSound(const QString& name);
    
public slots:
    void quit();
    void onIssues(const QVariantMap& data);
    void onIssueClick();
    void update();
    void openMainWindow();
    
private:
    void createActions();
    void createTrayIcons();
    void loadRedmine();
    void unloadRedmine();
    void rebuildMenu();
    
    QMenu* trayIconMenu;
    QAction* quitAction;
    QAction* openAction;
    QSystemTrayIcon* trayIcon;
    RedmineClient* redmine;
    const QString baseUrl;
    IssueContainer* issues;
    QTimer* updateTimer;
    int userId;
    QString userName;
    QString userPass;
    bool firstRun;
};


#endif //__TRAY_HPP__


