

#include <QtCore/QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QTcpSocket>

#include "gethub.hpp"


GethubClient::~GethubClient()
{
    if (m_socket) {
        disconnect(m_socket, SIGNAL(connected()), this, SLOT(onConnected()));
        disconnect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        delete m_socket;
        m_socket = 0;
    }
};

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
}

void GethubClient::onReadyRead(){
    qDebug() << "gethub connection ready";
}