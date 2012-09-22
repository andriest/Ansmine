

#include <QtCore/QDebug>
#include <QtGui/QWidget>
#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtGui/QSystemTrayIcon>
#include <QtGui/QIcon>
#include <QtCore/QSettings>

#include "tray.hpp"
#include "redmine.hpp"
#include "issue.hpp"

Tray::Tray(const QString& baseRedmineUrl):
    QWidget(),
    trayIconMenu(new QMenu(this)),
    baseUrl(baseRedmineUrl),
    issues(new QList<Issue*>())
{
    this->setWindowIcon(QIcon(":/digaku-logo_16x16x32.png"));
    
    createActions();
    createTrayIcons();
    
    this->hide();
    
    loadRedmine();
    
    QSettings settings;
    settings.beginGroup("redmine_account");
    
    redmine->query(baseUrl + "/issues.json?assigned_to_id=" + settings.value("userId").toString());
}

Tray::~Tray()
{
    
    foreach(QAction* act, trayIconMenu->actions()){
        disconnect(act, SIGNAL(triggered()), this, SLOT(onIssueClick()));
    }

    delete trayIconMenu;

    disconnect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));
    delete quitAction;
    
    unloadRedmine();

    // clean up issues memory
    QList<Issue*>::const_iterator it;
    for (it = issues->begin(); it != issues->end(); ++it) {
        delete *it;
    }
    
    issues->clear();
    
    delete issues;
    
}

void Tray::loadRedmine()
{
    // load redmine client
    QSettings settings;
    settings.beginGroup("redmine_account");
    
    redmine = new RedmineClient("redmine.digaku.com", 
                                settings.value("userName").toString(), 
                                settings.value("userPass").toString());
    
    redmine->setUserId(settings.value("userId").toInt());
    
    connect(redmine, SIGNAL(issues(const QVariantMap&)), this, SLOT(onIssues(const QVariantMap&)));
}

void Tray::unloadRedmine()
{
    disconnect(redmine, SIGNAL(issues(const QVariantMap&)), this, SLOT(onIssues(const QVariantMap&)));
    delete redmine;
}

void Tray::createActions(){
    quitAction = new QAction(tr("Quit"), this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));
}


void Tray::createTrayIcons(){
    trayIconMenu->addAction(quitAction);
    
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/digaku-logo_16x16x32.png"));
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->show();
}

void Tray::quit(){
    this->close();
}


void Tray::onIssues(const QVariantMap& data){
    foreach(QVariant issuev, data["issues"].toList()){
        QVariantMap issue = issuev.toMap();
        
        Issue* iss = new Issue(baseUrl,
                               issue["id"].toInt(), 
                               issue["status"].toMap()["name"].toString(),
                               issue["subject"].toString(), issue["description"].toString());
        issues->append(iss);
        
    }
    rebuildMenu();
}

void Tray::onIssueClick(){
    QAction* act = (QAction*)this->sender();
    
    QString text = act->text();
    qDebug() << text;
    
    int id = text.mid(1, text.indexOf(' ')).toInt();

    QList<Issue*>::const_iterator it;
    
    for (it = issues->begin(); it != issues->end(); ++it) {
        if ((*it)->getId() == id) {
            (*it)->open();
            break;
        }
    }
    
}

void Tray::rebuildMenu(){
    trayIconMenu->removeAction(quitAction);
    
    QList<Issue*>::const_iterator it;
    
    for (it = issues->begin(); it != issues->end(); ++it) {
        Issue* issue = *it;
        QAction* act = new QAction(issue->toString(), this);
        connect(act, SIGNAL(triggered()), this, SLOT(onIssueClick()));
        trayIconMenu->addAction(act);        
    }
    
    
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
}



