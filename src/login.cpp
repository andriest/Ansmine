

#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtGui/QMessageBox>

#include "login.hpp"
#include "redmine.hpp"


LoginDialog::LoginDialog(QWidget* parent):
    QDialog(parent),
    m_redmineClient(0)
{
    setupUi(this);
    
    txtUserName->setFocus();
    txtUserPassword->setEchoMode(QLineEdit::Password);
    
    connectAll();
}

LoginDialog::~LoginDialog()
{
    if (m_redmineClient != 0){
        delete m_redmineClient;
    }
}

void LoginDialog::connectAll(){

}

void LoginDialog::accept(){
    
    // try login
    
    m_redmineClient = new RedmineClient("redmine.digaku.com",
                     txtUserName->text(),
                     txtUserPassword->text());

    connect(m_redmineClient, SIGNAL(success(const QByteArray&)), this, SLOT(success(const QByteArray&)));
    connect(m_redmineClient, SIGNAL(failed(const QString&, int)), this, SLOT(failed(const QString&, int)));
    connect(m_redmineClient, SIGNAL(users(const QVariantMap&)), this, SLOT(users(const QVariantMap&)));
    
    m_redmineClient->query("http://redmine.digaku.com/users.json");
    
}

void LoginDialog::success(const QByteArray& data){
    qDebug() << "Login success";    
    //qDebug() << "data:" << data;
    
}

void LoginDialog::users(const QVariantMap& data){
    
    int userId = -1;
    
    foreach(QVariant v, data["users"].toList()){
        QVariantMap user = v.toMap();
        
        if (user["login"].toString().toLower() == txtUserName->text().toLower()) {
            userId = user["id"].toInt();
        }
    }
    
    if (userId < 0) {
        qDebug() << "User not found on list";
        return;
    }
    
    QSettings settings;
    
    settings.beginGroup("redmine_account");
    
    settings.setValue("userId", userId);
    settings.setValue("userName", txtUserName->text());
    settings.setValue("userPass", txtUserPassword->text());
    
    done(0);
}

void LoginDialog::failed(const QString &url, int errorCode){
    qDebug() << "Login failed. error code " << errorCode;
    
    if (errorCode == 204) {
        QMessageBox::warning(this, "Fail", "Invalid user name or password");
    }else{
        QMessageBox::warning(this, "Error", "Login failed. error code " + errorCode);
    }
    
}



