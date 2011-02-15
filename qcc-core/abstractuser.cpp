#include "abstractuser.h"

const char* AbstractUser::StatusNames[] = {
    "Offline",
    "Online"
};

QString AbstractUser::statusString(Status status)
{
    return AbstractUser::StatusNames[status];
}

AbstractUser::AbstractUser(QObject *parent) :
    QObject(parent), m_status(Offline)
{
}

AbstractUser::AbstractUser(const QString &username, QObject *parent) :
    QObject(parent), m_username(username), m_status(Offline)
{
}

void AbstractUser::setStatus(Status status)
{
    if (status == m_status)
        return;

    m_status = status;
    emit statusChanged();
}

void AbstractUser::reset()
{
    setStatus(Offline);
}
