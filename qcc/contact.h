#ifndef CONTACT_H
#define CONTACT_H

#include "abstractuser.h"

#include <QIcon>

class Contact : public AbstractUser
{
    Q_OBJECT

public:
    explicit Contact(QObject *parent = 0);
    Contact(const QString &username, QObject *parent = 0);
    void init();

    QIcon statusIcon() const;

private:
    static QIcon OfflineIcon, OnlineIcon;
};

#endif // CONTACT_H
