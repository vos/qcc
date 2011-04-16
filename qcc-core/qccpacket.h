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

#ifndef TCPPACKET_H
#define TCPPACKET_H

#include <QByteArray>
#include <QDataStream>

QT_BEGIN_NAMESPACE
class QTcpSocket;
QT_END_NAMESPACE

class QccPacket
{
public:
    enum PacketType {
        ConnectionAccepted,
        ConnectionRefused,
        UserRegister,          // QString(username), QString(passwort|SHA-1), QByteArray(publickey)
        RegisterSuccess,
        RegisterFailure,       // QString(reason)
        UserAuthentication,    // QString(username), QString(passwort|SHA-1), QByteArray(publickey)
        AuthenticationSuccess,
        AuthenticationFailure, // QString(reason)
        RequestAuthorization,  // QString(username)
        AuthorizationAccepted, // QString(username), [qint32(status)|client], [QByteArray(publickey)|client]
        AuthorizationDeclined, // QString(username)
        AuthorizationFailure,  // QString(reason)
        RequestContactList,
        ContactList,           // qint32(count), QString(username), qint32(status), QByteArray(publicKey), ...
        ContactStatusChanged,  // QString(username), qint32(status), QByteArray(publicKey)
        RemoveContact,         // QString(username)
        ContactRemoved,        // QString(username)
        Message,               // qint32(message-id), QString(receiver|sender), QString(message)
        MessageSuccess,        // qint32(message-id), QString(sender)
        MessageFailure,        // qint32(message-id), QString(receiver), QString(reason)
        IllegalMessage = -1
    };

    static QString typeString(PacketType type);

    QccPacket(PacketType type = Message);
    inline PacketType type() const { return m_type; }
    inline QString typeString() const { return QccPacket::PacketTypeNames[m_type]; }
    inline QByteArray& data() { return m_data; }
    inline QDataStream& stream() { return m_stream; }
    inline int size() const { return m_data.size() - sizeof(quint32); }
    bool send(QTcpSocket *socket);

private:
    static const char *PacketTypeNames[];

    PacketType m_type;
    QByteArray m_data;
    QDataStream m_stream;
};

#endif // TCPPACKET_H
