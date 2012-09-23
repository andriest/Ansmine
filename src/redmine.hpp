

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
    RedmineClient(const QString& _host);
    RedmineClient(const QString& _host, 
                  const QString& _userName, const QString& _userPass);
    ~RedmineClient();
    
    void setUserId(int id){
        m_userId = id;
        setAccount(userName, userPass);
    }
    
    void setAccount(const QString& _userName, const QString& _userPass);
    void query(const QString& url);
    void onIssues(const QVariantMap& data);
    void onUsers(const QVariantMap& data);
    
    // for connection test
    void test(const QString& _host, const QString& _userName, const QString& password);
    inline void getIssues(){
        query(QString("http://%1/issues.json?assigned_to_id=%2").arg(host).arg(m_userId));
    }

Q_SIGNALS:
    void success(const QByteArray& data);
    void failed(const QString& url, int errorCode);
    void issues(const QVariantMap& data);
    void users(const QVariantMap& data);
    
    void testSuccess(const QByteArray& data);
    void testFailed(const QString& url, int errorCode);

public Q_SLOTS:
    void checkUpdate();

    
private Q_SLOTS:
    void requestCompleted(QNetworkReply* repl);
    
private:
    void init();
    
    QString host;
    QString userName;
    QString userPass;
    
    QNetworkAccessManager* m_networkManager;
    QNetworkRequest* m_req;
    
    int m_userId;
    bool m_inTest;
    
};


#endif // __REDMINE_HPP__

