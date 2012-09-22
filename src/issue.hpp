

#ifndef __ISSUE_HPP__
#define __ISSUE_HPP__

#include <QtCore/QDebug>
#include <QtCore/QString>

#include "redmine_model.hpp"

class Issue: public RedmineModel {
public:
    Issue(): 
        m_id(0),
        m_doneRatio(0)
    {
        
    }
    
    Issue(int id):
        m_id(id),
        m_doneRatio(0)
    {
        
    }
    
    Issue(int id, const QString& subject, const QString& desc):
        m_id(id),
        m_doneRatio(0),
        m_subject(subject), 
        m_desc(desc)
    {
        
    }

    Issue(int id, const QString& status, const QString& subject, const QString& desc):
        m_id(id),
        m_doneRatio(0),
        m_status(status),
        m_subject(subject), 
        m_desc(desc)
    {
        
    }
    
    ~Issue(){
        qDebug() << "issue " << m_id << " destroyed";
    }
    
    const QString toString(){
        QString issueStr( "#" + QString("%1").arg(this->m_id) + " [" + this->m_status + "] " +
                         this->m_subject);
        return issueStr;
    }
    
private:
    int m_id;
    int m_doneRatio;
    QString m_status;
    QString m_subject;
    QString m_desc;
    QString m_tracker;
};

#endif // __ISSUE_HPP__

