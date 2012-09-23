

#include <QtCore/QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QTcpSocket>
#include <qjson/parser.h>

#include "gethub.hpp"

#define NS_AUTHORIZE QString("{\"ns\":\"authorize\",\"version\":1,\"id\":\"123\",\"userName\":\"%1\",\"password\":\"%2\"}\n")

GethubClient::~GethubClient()
{
    if (m_socket) {
        disconnect(m_socket, SIGNAL(connected()), this, SLOT(onConnected()));
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

void GethubClient::onReadyRead(){
    qDebug() << "gethub connection ready";
    
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
            }
            
        }
        
    }
    
    
}


