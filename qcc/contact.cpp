#include "contact.h"

QIcon Contact::OfflineIcon;
QIcon Contact::OnlineIcon;

Contact::Contact(QObject *parent) :
    AbstractUser(parent)
{
    init();
}

Contact::Contact(const QString &username, QObject *parent) :
    AbstractUser(username, parent)
{
    init();
}

void Contact::init()
{
    if (Contact::OfflineIcon.isNull()) {
        Contact::OfflineIcon = QIcon(":/icons/offline.png");
        Contact::OnlineIcon = QIcon(":/icons/online.png");
    }
}

QIcon Contact::statusIcon() const
{
    switch (m_status) {
    case Offline: return Contact::OfflineIcon;
    case Online: return Contact::OnlineIcon;
    default: return QIcon();
    }
}
