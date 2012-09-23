

#include <QtCore/QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include <qjson/parser.h>

#include "redmine.hpp"




RedmineClient::RedmineClient(const QString& _host):
    host(_host),
    m_userId(0),
    m_inTest(false)
{
    init();
}

RedmineClient::RedmineClient(const QString& _host, const QString& _userName, const QString& _userPass):
    host(_host),
    m_userId(0),
    m_inTest(false)
{
    userName = _userName;
    userPass = _userPass;
    init();
}

RedmineClient::~RedmineClient()
{
    disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)), 
               this, SLOT(requestCompleted(QNetworkReply *)));
    delete m_networkManager;
    delete m_req;
}


void RedmineClient::init()
{
    m_networkManager = new QNetworkAccessManager(this);
 
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), 
            this, SLOT(requestCompleted(QNetworkReply *)));
    
    m_req = new QNetworkRequest();
    
    m_req->setUrl("http://" + host + "/users/" + m_userId);
    
    QByteArray basicAuth(QString(userName + ":" + userPass).toAscii());
    
    m_req->setRawHeader("Authorization", "Basic " + basicAuth.toBase64());
    
    
}


void RedmineClient::requestCompleted(QNetworkReply* repl)
{
    if (repl->error() > 0) {
        qDebug() << "Request error code" << repl->error();
        
        Q_EMIT failed(repl->url().toString(), repl->error());
        
        // if in connection test 
        if (m_inTest) {
            m_inTest = false;
            Q_EMIT testFailed(repl->url().toString(), repl->error());
        }
        
    }else{
        
        QJson::Parser parser;
        bool ok;
        QByteArray data = repl->readAll();
        
        Q_EMIT success(data);
        
        QVariantMap result = parser.parse(data, &ok).toMap();
        
        if (!ok) {
            qDebug() << "Cannot parse data from server. " << data;
            return;
        }
        
        // if in connection test 
        if (m_inTest) {
            m_inTest = false;
            Q_EMIT testSuccess(data);
            return;
        }
        
        //qDebug() << "Your issues count " << result["total_count"].toInt();
        
        if (result.contains("issues")) {
            onIssues(result);
        }else if (result.contains("users")){
            onUsers(result);
        }
        
    }
}

void RedmineClient::onIssues(const QVariantMap& data){
    qDebug() << "issues " << data["total_count"].toInt();
    Q_EMIT issues(data);
}

void RedmineClient::onUsers(const QVariantMap &data){
    qDebug() << "users " << data["total_count"].toInt();
    Q_EMIT users(data);
}

void RedmineClient::checkUpdate()
{
    
}

void RedmineClient::query(const QString &url){
    if (m_inTest) {
        qDebug() << "Cannot create any query during network test";
        return;
    }
    m_req->setUrl(url);
    m_networkManager->get(*m_req);
}

void RedmineClient::test(const QString& _host, const QString& _userName, const QString& password){
    QNetworkRequest req;
    
    QByteArray basicAuth(QString(_userName + ":" + password).toAscii());
    
    req.setRawHeader("Authorization", "Basic " + basicAuth.toBase64());
    req.setUrl("http://" + _host + "/users.json");
    
    m_inTest = true;
    
    m_networkManager->get(req);
}

//
//void RedmineClient::testSuccess(const QByteArray& data){
//    
//}
//
//void RedmineClient::testFailed(const QString& url, int errorCode){
//    
//}
//

void RedmineClient::setAccount(const QString& _userName, const QString& _userPass){
    userName = _userName;
    userPass = _userPass;
    
    delete m_req;
    
    m_req = new QNetworkRequest();
    
    QByteArray basicAuth(QString(userName + ":" + userPass).toAscii());
    
    qDebug() << "basicAuth: " << QString(userName + ":" + userPass);
    
    m_req->setRawHeader("Authorization", "Basic " + basicAuth.toBase64());
}




