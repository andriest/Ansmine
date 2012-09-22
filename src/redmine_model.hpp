

#ifndef __REDMINE_MODEL_HPP__
#define __REDMINE_MODEL_HPP__

#include <QtCore/QString>
#include <QtCore/QList>

class RedmineModel {
public:
    RedmineModel(const QString& url, int id):
        baseUrl(url),
        m_id(id)
    {
    }
    
    const QString& getBaseUrl(){
        return baseUrl;
    }
    
    int getId(){
        return m_id;
    }
    
protected:
    QString baseUrl;
    int m_id;
};

template<class T>
class RedmineModelContainer: public QList<T*>
{
public:
    RedmineModelContainer():QList<T*>(){
    }

    /**
     * get model by id
     * @param id -- id of model.
     */
    inline T* getById(int id){
        typename QList<T*>::const_iterator it;
        for (it = this->begin(); it != this->end(); ++it) {
            RedmineModel* model = (RedmineModel*)*it;
            if (model->getId() == id) {
                return (T*)model;
            }
        }
        return 0;
    }
    
    inline bool contains(int id){
        return getById(id) != 0;
    }
    
    inline bool contains(T* m){
        return getById(((RedmineModel*)m)->getId());
    }
    
    /**
     * This clear method override original
     * QList::clear method for support auto-free-ing
     * memory to items before clear-ing container.
     */
    inline void clear(){
        typename QList<T*>::const_iterator it;
        for (it = this->begin(); it != this->end(); ++it) {
            delete *it;
        }
        QList<T*>::clear();
    }
    
};


#endif // __REDMINE_MODEL_HPP__

