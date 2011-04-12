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
    if (publicKey.isEmpty()) // should only happen if the contact is offline
        return;

    QCA::ConvertResult conversionResult;
    m_publicKey = QCA::PublicKey::fromDER(publicKey, &conversionResult);
    if (conversionResult != QCA::ConvertGood || !m_publicKey.canEncrypt())
        qWarning("Contact::setPublicKey: not a valid public key");
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
    QByteArray bytes = text.toAscii();
    QCA::SecureArray result;

    static const int padding = 42; // always constant?
    const int keySize = m_publicKey.bitSize() / 8;
    const int blockSize = keySize - padding;

    for (int i = 0; i < bytes.length(); i += blockSize)
        result.append(m_publicKey.encrypt(bytes.mid(i, blockSize), QCA::EME_PKCS1_OAEP));

    if (result.isEmpty())
        qWarning("Error encrypting");

    return result.toByteArray();
}
