

#ifndef __LOGIN_HPP__
#define __LOGIN_HPP__

#include "ui_login.h"

class RedmineClient;

class LoginDialog: public QDialog, private Ui::Dialog
{
    Q_OBJECT;
    
public:
    LoginDialog(QWidget* parent=0);
    ~LoginDialog();
    
public slots:
    void accept();
    void success(const QByteArray& data);
    void failed(const QString& url, int errorCode);
    void users(const QVariantMap& data);
private:
    void connectAll();
    
    RedmineClient* m_redmineClient;
    
};


#endif // __LOGIN_HPP__
