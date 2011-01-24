#include "user.h"

User::User(QObject *parent) :
    QObject(parent), m_status(Offline)
{
}

User::User(const QString &username, const QString &password, QObject *parent) :
    QObject(parent), m_status(Offline)
{
    m_username = username;
    m_password = password;
}
