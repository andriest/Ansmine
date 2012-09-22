

#ifndef __REDMINE_MODEL_HPP__
#define __REDMINE_MODEL_HPP__

#include <QtCore/QString>

class RedmineModel {
public:
    RedmineModel(const QString& url):
        baseUrl(url){
    }
    
    const QString& getBaseUrl(){
        return baseUrl;
    }
    
protected:
    QString baseUrl;
};

#endif // __REDMINE_MODEL_HPP__

