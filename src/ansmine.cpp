

#include <QtGui/QApplication>
#include <QtGui/QWidget>
#include <QtGui/QDesktopWidget>
#include <QtGui/QStandardItemModel>
#include <QtGui/QStandardItem>
#include <QtGui/QMessageBox>
#include <QtCore/QSettings>
#include <qjson/parser.h>

#include "ansmine.hpp"
#include "redmine.hpp"
#include "issue.hpp"
#include "gethub.hpp"


AnsmineMainwindow::AnsmineMainwindow(QObject* parent):
    inTestConnection(false)
{
    setupUi(this);
    

    init();
    centerize();
    
    setFocus();
    activateWindow();
}

AnsmineMainwindow::~AnsmineMainwindow(){
    disconnect(redmine, SIGNAL(success(const QByteArray&)), this, SLOT(onSuccess(const QByteArray&)));
    disconnect(redmine, SIGNAL(issues(const QVariantMap&)), this, SLOT(onIssues(const QVariantMap&)));
    disconnect(redmine, SIGNAL(failed(const QString&, int)), this, SLOT(onFailed(const QString&, int)));

    delete redmine;
    
    disconnect(btnDisconnect, SIGNAL(clicked()), this, SLOT(onDisconnectButtonClicked()));
    
    issues->clear();
    delete issues;
    
    disconnect(gethub, SIGNAL(onReady()), this, SLOT(onGethubConnected()));
    disconnect(gethub, SIGNAL(onMessage(const QString&, const QString&, const QString&)), this, SLOT(onGethubGotMessage(const QString&, const QString&, const QString&)));
    disconnect(gethub, SIGNAL(onJoin(const QString&, const QStringList&)), this, SLOT(onGethubJoin(const QString&, const QStringList&)));
    
    delete gethub;
}

void AnsmineMainwindow::init(){
    
    QSettings sett;
    sett.beginGroup("redmine");
    
    host = sett.value("host").toString();
    userId = sett.value("userId").toInt();
    userName = sett.value("userName").toString();
    userPass = sett.value("userPass").toString();
    baseRedmineUrl = "http://" + host;
    
    redmine = new RedmineClient(host, userName, userPass);
    redmine->setUserId(userId);
    
    connect(redmine, SIGNAL(success(const QByteArray&)), this, SLOT(onSuccess(const QByteArray&)));
    connect(redmine, SIGNAL(issues(const QVariantMap&)), this, SLOT(onIssues(const QVariantMap&)));
    connect(redmine, SIGNAL(failed(const QString&, int)), this, SLOT(onFailed(const QString&, int)));
    
    issues = new IssueContainer();
    
    model = new QStandardItemModel();
    issueListView->setModel(model);
    
    chatOnlineUsersModel = new QStandardItemModel();
    lstOnlineUsers->setModel(chatOnlineUsersModel);
    

    connect(btnAuthorize, SIGNAL(clicked()), this, SLOT(onAuthorizeButtonClicked()));
    
    txtHost->setText(host);
    txtUserName->setText(userName);
    //txtPassword->setText(userPass);
    
    txtPassword->setEchoMode(QLineEdit::Password);
    
    txtChat->setReadOnly(true);
    
    connect(btnTest, SIGNAL(clicked()), this, SLOT(testConnection()));
    
    btnAuthorize->setVisible(false);
    btnDisconnect->setEnabled(false);
    
    connect(btnDisconnect, SIGNAL(clicked()), this, SLOT(onDisconnectButtonClicked()));
    
    tabMain->setCurrentIndex(0);
    
    connect(tabMain, SIGNAL(currentChanged(int)), this, SLOT(mainTabChanged(int)));
    
    QString queryStr = QString("%1/issues.json?assigned_to_id=%2").arg(baseRedmineUrl).arg(userId);

    qDebug() << "queryStr: " << queryStr;
    
    redmine->query(queryStr);
    
    // setup chat
    gethub = new GethubClient("127.0.0.1", 6060);
    
    connect(gethub, SIGNAL(onReady()), this, SLOT(onGethubConnected()));
    connect(gethub, SIGNAL(onMessage(const QString&, const QString&, const QString&)), this, SLOT(onGethubGotMessage(const QString&, const QString&, const QString&)));
    connect(gethub, SIGNAL(onJoin(const QString&, const QStringList&)), this, SLOT(onGethubJoin(const QString&, const QStringList&)));
    
    gethub->start();
    
}

void AnsmineMainwindow::centerize(){
    QDesktopWidget dw;
    QRect screenGeometry = dw.screenGeometry();
    int x = (screenGeometry.width()-this->width()) / 2;
    int y = (screenGeometry.height()-this->height()) / 2;
    this->move(x, y);
    this->show();
}


void AnsmineMainwindow::onIssues(const QVariantMap& data){
    issues->clear();
    
    foreach(QVariant v, data["issues"].toList()){
        QVariantMap issm = v.toMap();
        
        int id = issm["id"].toInt();
        QString status = issm["status"].toMap()["name"].toString();
        QString subject = issm["subject"].toString();
        QString desc = issm["description"].toString();
        
        Issue* issue = new Issue(baseRedmineUrl, id, status, subject, desc);
        
        if (!issues->contains(id)) {
            issues->append(issue);
        }
    }
    rebuildIssueList();
}

void AnsmineMainwindow::rebuildIssueList(){
    model->clear();
    
    IssueContainer::const_iterator it;
    for (it = issues->begin(); it != issues->end(); ++it) {
        Issue* issue = *it;
        QStandardItem* item = new QStandardItem(issue->toString());
        item->setEditable(false);
        model->appendRow(item);
    }
}

void AnsmineMainwindow::onAuthorizeButtonClicked(){
    tabMain->setCurrentIndex(2);
}

void AnsmineMainwindow::onSuccess(const QByteArray& data){
    btnDisconnect->setEnabled(true);
}

void AnsmineMainwindow::onFailed(const QString& url, int errorCode){
    if (errorCode == 204) {
        btnAuthorize->setVisible(true);
    }
    btnDisconnect->setEnabled(false);
}

void AnsmineMainwindow::mainTabChanged(int index){
    qDebug() << "tab changed to " << index;
    
    
    QString newHost = txtHost->text();
    QString newUserName = txtUserName->text();
    QString newUserPass = txtPassword->text();
    
    //qDebug() << "newHost: " << newHost;
    //qDebug() << "newUserName: " << newUserName;
//    qDebug() << "newUserPass: " << newUserPass;
    
    if (newHost.length() > 0 && newUserName.length() > 0 && newUserPass.length() > 0) {
        if (newHost != host || newUserName != userName || newUserPass != userPass) {
            // save config
            
            QSettings st;
            st.beginGroup("redmine");
            st.setValue("host", newHost);
            st.setValue("userName", newUserName);
            st.setValue("userPass", newUserPass);
            st.sync();
        }        
    }
    
}

void AnsmineMainwindow::testConnection(){
    QString newHost = txtHost->text();
    QString newUserName = txtUserName->text();
    QString newUserPass = txtPassword->text();
    
    inTestConnection = true;

    connect(redmine, SIGNAL(testSuccess(const QByteArray&)), this, SLOT(testSuccess(const QByteArray&)));
    connect(redmine, SIGNAL(testFailed(const QString&, int)), this, SLOT(testFailed(const QString&, int)));
    
    redmine->test(newHost, newUserName, newUserPass);
}

void AnsmineMainwindow::testSuccess(const QByteArray& data){
    disconnect(redmine, SIGNAL(testSuccess(const QByteArray&)), this, SLOT(testSuccess(const QByteArray&)));
    disconnect(redmine, SIGNAL(testFailed(const QString&, int)), this, SLOT(testFailed(const QString&, int)));
    
    QJson::Parser parser;
    bool ok;
    
    QVariantMap result = parser.parse(data, &ok).toMap();
    
    if (!ok) {
        qDebug() << "server response: " << data;
        QMessageBox::warning(this, "Failed", "Invalid server response");
        return;
    }
    
    int newUserId = -1;
    
    foreach(QVariant v, result["users"].toList()){
        QVariantMap user = v.toMap();
        if (user["login"].toString().toLower() == txtUserName->text().toLower()) {
            newUserId = user["id"].toInt();
            break;
        }
    }
    
    if (newUserId == -1) {
        qDebug() << "server response: " << data;
        QMessageBox::information(this, "Failed", "Cannot get current user id");
        return;
    }
    
    
    userId = newUserId;
    userName = txtUserName->text();
    userPass = txtPassword->text();
    
    redmine->setUserId(newUserId);
    redmine->setAccount(userName, userPass);
    redmine->getIssues();
    
    QSettings st;
    st.beginGroup("redmine");
    st.setValue("userId", userId);
    st.setValue("userName", userName);
    st.setValue("userPass", userPass);
    st.sync();
    
    QMessageBox::information(this, "Success", "Connection Success");
    
}

void AnsmineMainwindow::testFailed(const QString& url, int errorCode){
    disconnect(redmine, SIGNAL(testSuccess(const QByteArray&)), this, SLOT(testSuccess(const QByteArray&)));
    disconnect(redmine, SIGNAL(testFailed(const QString&, int)), this, SLOT(testFailed(const QString&, int)));
    
    if (errorCode == 204 || errorCode == 3) {
        QMessageBox::warning(this, "Fail", "Invalid user name or password");
    }else{
        QMessageBox::warning(this, "Fail", QString("Connection Failed. Code %1").arg(errorCode));
    }
}

void AnsmineMainwindow::onDisconnectButtonClicked(){
    redmine->close();
    
    QSettings st;
    st.beginGroup("redmine");
    st.setValue("userId", 0);
    st.setValue("userName", "");
    st.setValue("userPass", "");
    st.sync();
    
    userId = 0;
    userName = "";
    userPass = "";
    
    model->clear();
}

void AnsmineMainwindow::onGethubConnected(){
    qDebug() << "GethubClient: do joining...";
    gethub->join("ansmine");
    
}

void AnsmineMainwindow::onGethubGotMessage(const QString& channelName, const QString& userName, const QString& message){
//    QString currentText = txtChat->document()->toPlainText();
    txtChat->append(QString("%1: %2").arg(userName).arg(message));
}

void AnsmineMainwindow::onGethubJoin(const QString& channelName, const QStringList& participantns){
    qDebug() << "GethubClient: do binding...";
    
    foreach(QString uname, participantns){
        QStandardItem* item = new QStandardItem(uname);
        item->setEditable(false);
        chatOnlineUsersModel->appendRow(item);
    }
    
    gethub->bind(channelName);
}



