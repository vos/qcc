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
        UserRegister,          // QString(username), QString(passwort|SHA-1)
        RegisterSuccess,
        RegisterFailure,       // QString(reason)
        UserAuthentication,    // QString(username), QString(passwort|SHA-1)
        AuthenticationSuccess,
        AuthenticationFailure, // QString(reason)
        RequestAuthorization,  // QString(username)
        AuthorizationAccepted, // QString(username), [qint32(status)|client]
        AuthorizationRejected, // QString(username)
        AuthorizationFailure,  // QString(reason)
        RequestContactList,
        ContactList,           // qint32(count), QString(username), qint32(status), QString(username), qint32(status), ...
        ContactStatusChanged,  // QString(username), qint32(status)
        RemoveContact,         // QString(username)
        ContactRemoved,        // QString(username)
        Message,               // qint32(message-id), QString(receiver|sender), QString(message)
        MessageSuccess,        // qint32(message-id), QString(sender)
        MessageFailure,        // qint32(message-id), QString(receiver), QString(reason)
        IllegalMessage = -1
    };

    static QString typeString(PacketType type);

    QccPacket(PacketType type = Message);
    PacketType type() const { return m_type; }
    QString typeString() const { return QccPacket::PacketTypeNames[m_type]; }
    QByteArray& data() { return m_data; }
    QDataStream& stream() { return m_stream; }
    bool send(QTcpSocket *socket);
    int size() const { return m_data.size() - sizeof(quint32); }

private:
    static const char *PacketTypeNames[];

    PacketType m_type;
    QByteArray m_data;
    QDataStream m_stream;
};

#endif // TCPPACKET_H
