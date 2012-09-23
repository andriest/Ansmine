


#ifndef __ANSMINE_HPP__
# define __ANSMINE_HPP__

#include <QtCore>
#include <QtGui/QMainWindow>
#include "ui_main.h"

class QWidget;
class RedmineClient;
class IssueContainer;
class QStandardItemModel;
class GethubClient;

class AnsmineMainwindow: public QMainWindow, private Ui::MainWindow {
    Q_OBJECT

public:
    AnsmineMainwindow(QObject* parent=0);
    ~AnsmineMainwindow();

private slots:
    void onIssues(const QVariantMap& data);
    void onFailed(const QString& url, int errorCode);
    void onSuccess(const QByteArray& data);
    void onAuthorizeButtonClicked();
    void onDisconnectButtonClicked();
    void mainTabChanged(int index);
    void testConnection();
    void testSuccess(const QByteArray& data);
    void testFailed(const QString& url, int errorCode);
    void onGethubConnected();
    void onGethubGotMessage(const QString& channelName, const QString& userName, const QString& message);
    void onGethubJoin(const QString&, const QStringList&);

private:
    void init();
    void centerize();
    void rebuildIssueList();
    
    RedmineClient* redmine;
    
    QString host;
    int userId;
    QString userName;
    QString userPass;
    QString baseRedmineUrl;
    IssueContainer* issues;
    QStandardItemModel* model;
    QStandardItemModel* chatOnlineUsersModel;
    bool inTestConnection;
    GethubClient* gethub;
};


#endif // __ANSMINE_HPP__
