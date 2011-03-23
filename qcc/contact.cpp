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

void Contact::setPublicKey(const QByteArray &publicKey)
{
    m_publicKey = QCA::PublicKey::fromDER(publicKey);
}

QIcon Contact::statusIcon() const
{
    switch (m_status) {
    case Offline: return Contact::OfflineIcon;
    case Online: return Contact::OnlineIcon;
    default: return QIcon();
    }
}

QByteArray Contact::encrypt(const QString &text)
{
    QCA::SecureArray a = m_publicKey.encrypt(text.toAscii(), QCA::EME_PKCS1_OAEP);
    qDebug("text length = %d", m_publicKey.canEncrypt());
    return a.toByteArray();
}
