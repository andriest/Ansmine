

#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtGui/QDesktopServices>


#include "issue.hpp"



void Issue::open(){
    QDesktopServices qds;
    QUrl url(QString("%1/issues/%2").arg(getBaseUrl()).arg(m_id));
    qds.openUrl(url);
}


