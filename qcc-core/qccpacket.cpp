#include "qccpacket.h"

#include <QTcpSocket>

QccPacket::QccPacket(PacketType type) :
    m_type(type), m_stream(&m_data, QIODevice::WriteOnly)
{
    m_stream.setVersion(QDataStream::Qt_4_0);
    m_stream << (quint32)0;
    m_stream << (qint32)type;
}

bool QccPacket::send(QTcpSocket *socket)
{
    if (!socket || m_data.length() < int(sizeof(quint32) + sizeof(qint32))) // size + type
        return false;

    m_stream.device()->seek(0);
    m_stream << (quint32)size();
    socket->write(m_data);
    return true;
}
