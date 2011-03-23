#ifndef CONTACT_H
#define CONTACT_H

#include "abstractuser.h"

#include <QIcon>
#include <QtCrypto>

class Contact : public AbstractUser
{
    Q_OBJECT

public:
    explicit Contact(QObject *parent = 0);
    Contact(const QString &username, QObject *parent = 0);
    void init();

    inline const QCA::PublicKey& publicKey() const { return m_publicKey; }
    void setPublicKey(const QByteArray &publicKey);
    QIcon statusIcon() const;

    QByteArray encrypt(const QString &text);

private:
    static QIcon OfflineIcon, OnlineIcon;

    QCA::PublicKey m_publicKey;
};

#endif // CONTACT_H
