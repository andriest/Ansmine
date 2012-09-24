

#include <QtCore/QDebug>

#include "settings_chat.hpp"
#include "ansmine.hpp"



void SettingsChat::init(){
    P_S(AnsmineMainwindow);
    
    s->btnDisconnectChat->setEnabled(false);
    
    connect(s->btnConnectChat, SIGNAL(clicked()), this, SLOT(onConnectChatClicked()));
}


void SettingsChat::onConnectChatClicked(){
    qDebug() << "Hello connect!!";
}
