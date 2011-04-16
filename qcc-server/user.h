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

#ifndef USER_H
#define USER_H

#include "abstractuser.h"

#include <QSet>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

QT_BEGIN_NAMESPACE
class QTcpSocket;
QT_END_NAMESPACE

class User : public AbstractUser
{
    Q_OBJECT

public:
    explicit User(QObject *parent = 0);
    User(const QString &username, const QString &password, QObject *parent = 0);

    inline const QString& password() const { return m_password; }
    inline const QSet<QString>& contacts() const { return m_contacts; }
    inline const QByteArray& publicKey() const { return m_publicKey; }
    inline QTcpSocket* socket() const { return m_socket; }

    inline void addContact(const QString &username) { m_contacts.insert(username); }
    inline bool containsContact(const QString &username) const { return m_contacts.contains(username); }
    inline bool removeContact(const QString &username) { return m_contacts.remove(username); }
    inline void clearContacts() { m_contacts.clear(); }

    inline bool matchPassword(const QString &password) const { return m_password == password; }

    static User* readUser(QXmlStreamReader &xml);
    void writeUser(QXmlStreamWriter &xml);

public slots:
    void setPassword(const QString &password) { m_password = password; }
    void setPublicKey(const QByteArray &publicKey) { m_publicKey = publicKey; }
    void setSocket(QTcpSocket *socket) { m_socket = socket; }
    void reset();

private:
    QString m_password;
    QSet<QString> m_contacts;
    QByteArray m_publicKey;
    QTcpSocket *m_socket;
};

#endif // USER_H
