#include "qccnamespace.h"
#include "server.h"
#include "user.h"

#include <QTcpSocket>
#include <QDebug>

Server::Server(QObject *parent) :
    QTcpServer(parent)
{
}

void Server::incomingConnection(int socketDescriptor)
{
    QTcpSocket *client = new QTcpSocket(this);
    client->setSocketDescriptor(socketDescriptor);
    addPendingConnection(client);

#ifdef DEBUG
    qDebug("Server::incomingConnection(%i): %s:%i", socketDescriptor, qPrintable(client->peerAddress().toString()), client->peerPort());
#endif

    connect(client, SIGNAL(readyRead()), SLOT(client_readyRead()));
    connect(client, SIGNAL(disconnected()), SLOT(client_disconnected()));
    m_clients.insert(client, NULL);

    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (qint32)Qcc::ConnectionAccepted;
    //out << (qint32)Qcc::ConnectionRefused;
    client->write(data);
}

void Server::client_readyRead()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client) {
        qWarning("Server::client_readyRead: Cast of sender() to QTcpSocket* failed");
        return;
    }
#ifdef DEBUG
    qDebug("Server::client_readyRead: %li bytes available", client->bytesAvailable());
#endif
    if (client->bytesAvailable() < sizeof(qint32)) {
        qWarning("Server::client_readyRead: Not enough data to read");
        return;
    }

    QDataStream in(client);
    in.setVersion(QDataStream::Qt_4_0);
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    qint32 type;
    in >> type;
#ifdef DEBUG
    qDebug("Server::client_readyRead: MessageType = %i", type);
#endif
    switch ((Qcc::MessageType)type) {
    case Qcc::UserAuthentication:
    {
        if (client->bytesAvailable() < sizeof(QString) * 2) {
            qWarning("Server::client_readyRead: Not enough data to read");
            return;
        }
        QString username, password;
        in >> username >> password;
#ifdef DEBUG
        qDebug("Server::client_readyRead: username = '%s' password = '%s'", qPrintable(username), qPrintable(password));
#endif
        // TODO check authentication
        m_clients[client] = new User(username, password);
        out << (qint32)Qcc::AuthenticationSuccess;
        //out << (qint32)AuthenticationFailure;
        break;
    }
    case Qcc::Message:
        // TODO read and forward message
        out << (qint32)Qcc::MessageSuccess;
        break;
    default:
        qWarning("Server::client_readyRead: Illegal MessageType %i", type);
        return;
    }

    if (data.length() > 0)
        client->write(data);
}

void Server::client_disconnected()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client) {
        qWarning("Server::client_disconnected: Cast of sender() to QTcpSocket* failed");
        return;
    }
    User *user = m_clients[client];
    QString username = user ? user->getUsername() : "[no user object]";

#ifdef DEBUG
    qDebug("client disconnected: '%s' (%s:%i)", qPrintable(username), qPrintable(client->peerAddress().toString()), client->peerPort());
#endif

    m_clients.remove(client);
    if (user)
        delete user;
}
