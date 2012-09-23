

#include <QtCore/QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QStringList>
#include <qjson/parser.h>

#include "gethub.hpp"

#define NS_AUTHORIZE QString("{\"ns\":\"authorize\",\"version\":1,\"id\":\"123\",\"userName\":\"%1\",\"password\":\"%2\"}\n")
#define NS_JOIN QString("{\"ns\":\"chat::join\",\"version\":1,\"id\":\"123\",\"channel\":\"%1\",\"sessid\":\"%2\"}\n")
#define NS_BIND QString("{\"ns\":\"chat::bind\",\"version\":1,\"id\":\"123\",\"channel\":\"%1\",\"sessid\":\"%2\"}\n")
#define NS_MESSAGE QString("{\"ns\":\"chat::message\",\"version\":1,\"id\":\"123\",\"channel\":\"%1\",\"message\":\"%2\",\"sessid\":\"%3\"}\n")
#define NS_CHANNEL_MESSAGES QString("{\"ns\":\"chat::channel::messages\",\"version\":1,\"id\":\"123\",\"channel\":\"%1\",\"sessid\":\"%2\"}\n")

GethubClient::~GethubClient()
{
    if (m_socket) {
        disconnect(m_socket, SIGNAL(connected()), this, SLOT(onConnected()));
        disconnect(m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
        disconnect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        
        m_socket->close();
        
        delete m_socket;
        m_socket = 0;
    }
    
    qDebug() << "gethub client closed";
 
}

void GethubClient::start()
{
    if (!m_socket) {
        m_socket = new QTcpSocket(this);
        
        connect(m_socket, SIGNAL(connected()), this, SLOT(onConnected()));
        connect(m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
        connect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    }
    m_socket->connectToHost(m_host, m_port);
}

void GethubClient::onConnected(){
    qDebug() << "gethub connected.";
    
    QByteArray ba = NS_AUTHORIZE.arg("robin").arg("123").toLocal8Bit();
    const char* data = ba.data();
    m_socket->write(data, strlen(data));
}

void GethubClient::onDisconnected(){
    qDebug() << "gethub connection disconnected";
}

void GethubClient::onReadyRead(){
    qDebug() << "gethub connection ready";
    
    _connected = true;
    
    while (m_socket->canReadLine()) {
        
        QString line = QString::fromUtf8(m_socket->readLine()).trimmed();
        
        QJson::Parser parser;
        bool ok;
        
        QVariantMap result = parser.parse(line.toLocal8Bit(), &ok).toMap();
        
        if (!ok) {
            qDebug() << "Cannot parse data from gethub server";
            qDebug() << "data :";
            qDebug() << line;
        }else{
            result = result["result"].toMap();
            
            if (!authorized) {
                sessionId = result["sessid"].toString();
                qDebug() << "Session id: " << sessionId;
                authorized = true;
                
                Q_EMIT onReady();
            }
            else{
                QString ns = result["ns"].toString();
                if (ns == "chat::message") {

                    QString channelName = result["channel"].toString();
                    QString userName = result["from"].toString();
                    QString chatMessage = result["message"].toString();

                    Q_EMIT onMessage(channelName, userName, chatMessage);
                    
                }else if(ns == "chat::join"){
                    
                    QString channelName = result["channel"].toString();
                    QVariantList participantsv = result["participants"].toList();
                    
                    QStringList participants;
                    
                    foreach(QVariant v, participantsv){
                        QString user = v.toString();
                        participants.append(user);
                    }
                    
                    
                    Q_EMIT onJoin(channelName, participants);
                    
                }else if(ns == "chat::channel::messages"){
                    
                    QString channelName = result["channel"].toString();
                    QString messages = result["messages"].toString();
                    
                    Q_EMIT onChannelMessages(channelName, messages);
                    
                }else{
                    qDebug() << "unhandled server data :";
                    qDebug() << line;
                }
            }
            
        }
        
    }
    
    
}



void GethubClient::join(const QString& channelName){
    QByteArray ba = NS_JOIN.arg(channelName).arg(sessionId).toLocal8Bit();
    const char* data = ba.data();
    m_socket->write(data, strlen(data));
    m_socket->flush();
}

void GethubClient::bind(const QString& channelName){
    QByteArray ba = NS_BIND.arg(channelName).arg(sessionId).toLocal8Bit();
    const char* data = ba.data();
    m_socket->write(data, strlen(data));
    m_socket->flush();
}

void GethubClient::message(const QString& channelName, const QString& _message){
    QByteArray ba = NS_MESSAGE.arg(channelName).arg(_message).arg(sessionId).toLocal8Bit();
    const char* data = ba.data();
    m_socket->write(data, strlen(data));
    m_socket->flush();
}

void GethubClient::channelMessages(const QString& channelName){
    QByteArray ba = NS_CHANNEL_MESSAGES.arg(channelName).arg(sessionId).toLocal8Bit();
    const char* data = ba.data();
    m_socket->write(data, strlen(data));    
    m_socket->flush();
}







