#include "user.h"

User::User(QObject *parent) :
    QObject(parent), m_status(Offline)
{
}

User::User(const QString &username, const QString &password, QObject *parent) :
    QObject(parent), m_username(username), m_password(password), m_status(Offline)
{
}

User::~User()
{
}

User::User(const User &other) :
    m_username(other.m_username), m_password(other.m_password), m_status(other.m_status)
{
}

User& User::operator =(const User &other)
{
    if (this != &other) {
        m_username = other.m_username;
        m_password = other.m_password;
        m_status = other.m_status;
    }
    return *this;
}
