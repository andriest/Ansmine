

#ifndef __SETTINGS_CHAT_HPP__
#define __SETTINGS_CHAT_HPP__


#include <QtCore/QObject>

#include "separate_class.hpp"

class AnsmineMainwindow;

class SettingsChat: public QObject 
{
    Q_OBJECT;
    
public:
    
    SettingsChat(AnsmineMainwindow* parent):
        QObject(),
        s_ptr(parent)
    { init(); }
    ~SettingsChat(){}
    
    
protected:
    AnsmineMainwindow* const s_ptr;
    
private slots:
    void onConnectChatClicked();
    
private:
    void init();
    
    
    DECLARE_SCLASS(AnsmineMainwindow);
};


#endif // __SETTINGS_CHAT_HPP__
