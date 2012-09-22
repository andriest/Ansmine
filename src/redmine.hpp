

#ifndef __REDMINE_HPP__
#define __REDMINE_HPP__

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariantMap>

class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;

class RedmineClient: public QObject 
{
    Q_OBJECT;
    
public:
    RedmineClient(const QString& host);
    RedmineClient(const QString& host, const QString& userName, const QString& userPass);
    ~RedmineClient();
    
    void setUserId(int id){
        this->m_userId = id;
    }
    
    void query(const QString& url);
    void onIssues(const QVariantMap& data);
    void onUsers(const QVariantMap& data);

Q_SIGNALS:
    void success(const QByteArray& data);
    void failed(const QString& url, int errorCode);
    void issues(const QVariantMap& data);
    void users(const QVariantMap& data);

public Q_SLOTS:
    void checkUpdate();

    
private Q_SLOTS:
    void requestCompleted(QNetworkReply* repl);
    
private:
    void init();
    
    const QString& host;
    const QString& userName;
    const QString& userPass;
    
    QNetworkAccessManager* m_networkManager;
    QNetworkRequest* m_req;
    
    int m_userId;
    
};


#endif // __REDMINE_HPP__

