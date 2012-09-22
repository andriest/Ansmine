

#ifndef __ISSUE_HPP__
#define __ISSUE_HPP__

#include <QtCore/QDebug>
#include <QtCore/QString>

#include "redmine_model.hpp"

class Issue: public RedmineModel {
public:
    
    Issue(const QString& baseUrl, int id):
        RedmineModel(baseUrl, id),
        m_doneRatio(0)
    {
        
    }
    
    Issue(const QString& baseUrl, int id, const QString& subject, const QString& desc):
        RedmineModel(baseUrl, id),
        m_doneRatio(0),
        m_subject(subject), 
        m_desc(desc)
    {
        
    }

    Issue(const QString& baseUrl, int id, const QString& status, const QString& subject, const QString& desc):
        RedmineModel(baseUrl, id),
        m_doneRatio(0),
        m_status(status),
        m_subject(subject), 
        m_desc(desc)
    {
        
    }
    
    ~Issue(){
        qDebug() << "issue " << m_id << " memory freed.";
    }
    
    void open();
    
    inline const QString& getStatus(){
        return m_status;
    }
    inline const QString& getSubject(){
        return m_subject;
    }
    inline const QString& getDesc(){
        return m_desc;
    }
    inline void setStatus(const QString& status){
        m_status = status;
    }
    inline void setSubject(const QString& subject){
        m_subject = subject;
    }
    
    const QString toString(){
        QString issueStr( "#" + QString("%1").arg(this->m_id) + " [" + this->m_status + "] " +
                         this->m_subject);
        return issueStr;
    }
    
private:
    int m_doneRatio;
    QString m_status;
    QString m_subject;
    QString m_desc;
    QString m_tracker;
};

class IssueContainer: public RedmineModelContainer<Issue>
{
public:
    IssueContainer(): RedmineModelContainer<Issue>(){};
};


#endif // __ISSUE_HPP__

