

#include <QtCore/QDebug>
#include <QtGui/QWidget>
#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtGui/QSystemTrayIcon>
#include <QtGui/QIcon>
#include <QtGui/QSound>
#include <QtCore/QProcess>
#include <QtCore/QTimer>
#include <QtCore/QSettings>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>


#include "tray.hpp"
#include "redmine.hpp"
#include "issue.hpp"

Tray::Tray(const QString& baseRedmineUrl):
    QWidget(),
    trayIconMenu(new QMenu(this)),
    baseUrl(baseRedmineUrl),
    issues(new IssueContainer()),
    updateTimer(new QTimer()),
    firstRun(true)
{
    this->setWindowIcon(QIcon(":/digaku-logo_16x16x32.png"));
    
    createActions();
    createTrayIcons();
    
    this->hide();
    
    loadRedmine();
    
    QSettings settings;
    settings.beginGroup("redmine");
    
    userId = settings.value("userId").toInt();
    userName = settings.value("userName").toString();
    userPass = settings.value("userPass").toString();
    
    redmine->query(baseUrl + "/issues.json?assigned_to_id=" + settings.value("userId").toString());
    
    updateTimer->setInterval(10000);

    connect(updateTimer, SIGNAL(timeout()), this, SLOT(update()));
    
    updateTimer->start();
    
    //playSound("new-task");
    
    trayIcon->showMessage("app dir", QCoreApplication::applicationDirPath());
}

Tray::~Tray()
{
    
    foreach(QAction* act, trayIconMenu->actions()){
        disconnect(act, SIGNAL(triggered()), this, SLOT(onIssueClick()));
    }

    delete trayIconMenu;

    disconnect(openAction, SIGNAL(triggered()), this, SLOT(openMainWindow()));
    disconnect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));

    delete openAction;
    delete quitAction;
    
    unloadRedmine();

    // clean up issues memory
//    QList<Issue*>::const_iterator it;
//    for (it = issues->begin(); it != issues->end(); ++it) {
//        delete *it;
//    }
//    
//    issues->clear();

    issues->clear();
    delete issues;
    
    disconnect(updateTimer, SIGNAL(timeout()), this, SLOT(update()));
    updateTimer->stop();
    
    delete updateTimer;
    
}

void Tray::loadRedmine()
{
    // load redmine client
    QSettings settings;
    settings.beginGroup("redmine");
    
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
    openAction = new QAction(tr("Open"), this);
    
    connect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));
    connect(openAction, SIGNAL(triggered()), this, SLOT(openMainWindow()));
}


void Tray::createTrayIcons(){
    trayIconMenu->addAction(quitAction);
    
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/digaku-logo_16x16x32.png"));
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->show();
}

void Tray::openMainWindow(){
    QProcess ps;
    QString path = QCoreApplication::applicationDirPath() + "/Ansmine";
    ps.startDetached(path, QStringList(), QCoreApplication::applicationDirPath());
}


void Tray::quit(){
    this->close();
}



void Tray::onIssues(const QVariantMap& data){
    
    QList<int> removedIssueIds;
    
    IssueContainer::const_iterator it;
    for (it = issues->begin(); it != issues->end(); ++it) {
        removedIssueIds.append((*it)->getId());
    }
    
    foreach(QVariant issuev, data["issues"].toList()){
        QVariantMap issue = issuev.toMap();
        
        int id = issue["id"].toInt();
        QString status = issue["status"].toMap()["name"].toString();
        QString subject = issue["subject"].toString();
        
        // check is we already know that?
        Issue* iss = issues->getById(id);
        if (iss != 0) {
            
            bool statusChanged = iss->getStatus() != status;
            bool subjectChanged = iss->getSubject() != subject; 
            QString updateTitle = QString("Task %1 Updated").arg(iss->getId());
            
            if (statusChanged && subjectChanged) {
                trayIcon->showMessage(updateTitle, QString("Status and subject changed. Status: %1, Subject: %2").arg(status).arg(subject));
                iss->setStatus(status);
                iss->setSubject(subject);
                
                if (status.toLower() == "closed" || status.toLower() == "resolved") {
                    playSound("task-done");
                }else{
                    playSound("general");
                }
                
            }else if(statusChanged){
                
                trayIcon->showMessage(updateTitle, QString("Status changed to: %1").arg(status));

                if (status.toLower() == "closed" || status.toLower() == "resolved") {
                    playSound("task-done");
                }else{
                    playSound("general");
                }
                
                iss->setStatus(status);
                
            }else if(subjectChanged){
                
                trayIcon->showMessage(updateTitle, QString("Subject changed to: %1").arg(subject));
                playSound("general");
                
                iss->setSubject(subject);
            }
            
        }else{
            iss = new Issue(baseUrl,
                            id, 
                            status,
                            subject, issue["description"].toString());
            
            issues->append(iss);
            
            // if not first init
            // then show notification
            if (!firstRun) {
                
                playSound("new-task");
                trayIcon->showMessage("New Task Received", iss->toString());
                
            }
        }
        
        if (removedIssueIds.contains(id)) {
            removedIssueIds.removeOne(id);
        }
        
    }
    
    if (removedIssueIds.length() > 0) {
        QList<int>::const_iterator it;
        for (it = removedIssueIds.begin(); it != removedIssueIds.end(); ++it) {
            Issue* iss = issues->getById(*it);
            if (iss != 0) {
                
                playSound("task-done");
                trayIcon->showMessage("Task Closed/Resolved", QString("#%1 %2").arg(iss->getId()).arg(iss->getSubject()));
                
                issues->removeOne(iss);
                
                delete iss;
            }
        }
    }
    
    
    firstRun = false;
    rebuildMenu();
}

void Tray::playSound(const QString &name){
    QString path = QCoreApplication::applicationDirPath() + "/../Resources/" + name + ".wav";
    QDir dir;
    
    if (!dir.exists(path)) {
        path = QCoreApplication::applicationDirPath() + "/../resources/" + name + ".wav";
    }
    if (!dir.exists(path)) {
        path = QCoreApplication::applicationDirPath() + "/" + name + ".wav";
    }
    if (!dir.exists(path)) {
        path = QCoreApplication::applicationDirPath() + "/resources/" + name + ".wav";
    }
    if (!dir.exists(path)) {
        qDebug() << "Cannot find sound for " << name + ".wav";
        qDebug() << "app dir " << QCoreApplication::applicationDirPath();
        return;
    }
    QSound::play(path);
}

void Tray::onIssueClick(){
    QAction* act = (QAction*)this->sender();
    
    QString text = act->text();
    qDebug() << text;
    
    int id = text.mid(1, text.indexOf(' ')).toInt();

    if (issues->contains(id)) {
        issues->getById(id)->open();
    }
    
}

void Tray::rebuildMenu(){
    trayIconMenu->clear();
//    trayIconMenu->removeAction(quitAction);
    
    QList<Issue*>::const_iterator it;
    
    for (it = issues->begin(); it != issues->end(); ++it) {
        Issue* issue = *it;
        QAction* act = new QAction(issue->toString(), this);
        connect(act, SIGNAL(triggered()), this, SLOT(onIssueClick()));
        trayIconMenu->addAction(act);        
    }
    
    
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(openAction);
    trayIconMenu->addAction(quitAction);
}

void Tray::update(){
    qDebug() << "Checking...";
    redmine->query(baseUrl + "/issues.json?assigned_to_id=" + QString("%1").arg(userId));
}





