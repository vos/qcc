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
    out << (int)Qcc::ConnectionAccepted;
    client->write(data);
}

void Server::client_readyRead()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client) {
        qWarning("Server::client_readyRead: Cast of sender() to QTcpSocket* failed.");
        return;
    }
    if (client->bytesAvailable() < sizeof(Qcc::MessageType)) {
        qWarning("Server::client_readyRead: Not enough data to read.");
        return;
    }

    QDataStream in(client);
    in.setVersion(QDataStream::Qt_4_0);
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    int type;
    in >> type;
#ifdef DEBUG
    qDebug() << "client_readyRead: MessageType = " << type;
#endif
    switch ((Qcc::MessageType)type) {
    case Qcc::UserAuthentication:
    {
        QString username, password;
        in >> username >> password;
        // TODO check authentication
        m_clients[client] = new User(username, password);
        out << (int)Qcc::AuthenticationSuccess;
        //out << (int)AuthenticationFailure;
        break;
    }
    case Qcc::Message:
        // TODO read and forward message
        out << (int)Qcc::MessageSuccess;
        break;
    default:
        qWarning("Server::client_readyRead: Illegal MessageType %i.", type);
        out << (int)Qcc::IllegalMessage;
    }
    client->write(data);
}

void Server::client_disconnected()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client) {
        qWarning("Server::client_disconnected: Cast of sender() to QTcpSocket* failed.");
        return;
    }
    User *user = m_clients.value(client);
    QString username = user ? user->getUsername() : "[no user object]";

#ifdef DEBUG
    qDebug("client disconnected: %s (%s:%i)", qPrintable(username), qPrintable(client->peerAddress().toString()), client->peerPort());
#endif

    m_clients.remove(client);
    if (user)
        delete user;
}
