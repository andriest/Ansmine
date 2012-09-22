

#include <QtCore/QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include <qjson/parser.h>

#include "redmine.hpp"




RedmineClient::RedmineClient(const QString& host):
    host(host),
    userName(""),
    userPass(""),
    m_userId(0)
{
    init();
}

RedmineClient::RedmineClient(const QString& host, const QString& userName, const QString& userPass):
    host(host),
    userName(userName),
    userPass(userPass),
    m_userId(0)
{
    
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
    m_req->setUrl(url);
    m_networkManager->get(*m_req);
}






