/****************************************************************************
**
** Copyright (C) 2011 Alexander Vos <info@vossoft.de>,
**                    Kai Wellmann <KaiWellmann@gmx.de>
**
** This file is part of Qt Crypto Chat (QCC).
**
** Qt Crypto Chat is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Qt Crypto Chat is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with Qt Crypto Chat.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

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
