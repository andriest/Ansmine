

#ifndef __REDMINE_HPP__
#define __REDMINE_HPP__

#include <QtCore/QObject>
#include <QtCore/QString>

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

public slots:
    void checkUpdate();
    
private slots:
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

