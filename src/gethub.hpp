

#ifndef __GETHUB_HPP__
#define __GETHUB_HPP__

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>

//class QNetworkAccessManager;
//class QNetworkReply;
//class QNetworkRequest;
class QTcpSocket;

class GethubClient: public QObject 
{
    Q_OBJECT;
    
public:
    GethubClient(): m_socket(0), authorized(false),
        _connected(false)
    {};
    GethubClient(const QString& host, int port): 
        m_socket(0),
        m_host(host), m_port(port),
        authorized(false), _connected(false)
    {};
    ~GethubClient();
    
    void start();
    
    bool isConnected(){ return _connected; }
    bool isAuthorized(){ return authorized; }
    
    void join(const QString& channelName);
    void bind(const QString& channelName);
    void message(const QString& channelName, const QString& _message);
    void channelMessages(const QString& channelName);
    
signals:
    void onReady();
    void onMessage(const QString& channelName, const QString& userName, const QString& message);
    void onJoin(const QString& channelName, const QStringList& participants);
    void onChannelMessages(const QString& channelName, const QString& messages);
    
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
    bool _connected;
    
};


#endif // __GETHUB_HPP__
