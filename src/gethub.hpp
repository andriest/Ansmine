

#ifndef __GETHUB_HPP__
#define __GETHUB_HPP__

#include <QtCore/QObject>
#include <QtCore/QString>

class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;
class QTcpSocket;

class GethubClient: public QObject 
{
    Q_OBJECT;
    
public:
    GethubClient(){};
    GethubClient(const QString& host, int port): m_host(host), m_port(port)
    {};
    ~GethubClient();
    
    void start();
    
private slots:
    void requestCompleted(QNetworkReply* repl);
    void onConnected();
    void onReadyRead();
    
    
private:
    
    
    QTcpSocket* m_socket;
    
    QNetworkAccessManager* m_network;
    QNetworkReply* m_reply;
    
    QString m_host;
    int m_port;
    
};


#endif // __GETHUB_HPP__
