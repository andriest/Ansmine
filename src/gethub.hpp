

#ifndef __GETHUB_HPP__
#define __GETHUB_HPP__

#include <QtCore/QObject>
#include <QtCore/QString>

//class QNetworkAccessManager;
//class QNetworkReply;
//class QNetworkRequest;
class QTcpSocket;

class GethubClient: public QObject 
{
    Q_OBJECT;
    
public:
    GethubClient(): m_socket(0), authorized(false),
        connected(false)
    {};
    GethubClient(const QString& host, int port): 
        m_socket(0),
        m_host(host), m_port(port),
        authorized(false), connected(false)
    {};
    ~GethubClient();
    
    void start();
    
    bool isConnected(){ return connected; }
    bool isAuthorized(){ return authorized; }
    
    void join(const QString& channelName);
    void bind(const QString& channelName);
    void message(const QString& channelName, const QString& _message);
    
signals:
    void onMessage(const QString& channelName, const QString& userName, const QString& message);
    
private slots:
//    void requestCompleted(QNetworkReply* repl);
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    
    
private:
    
    
    QTcpSocket* m_socket;
    
//    QNetworkAccessManager* m_network;
//    QNetworkReply* m_reply;
    
    QString m_host;
    int m_port;
    QString sessionId;
    bool authorized;
    bool connected;
    
};


#endif // __GETHUB_HPP__
