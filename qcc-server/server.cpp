#include "server.h"

#include <QTcpSocket>

Server::Server(QObject *parent) :
    QTcpServer(parent)
{

}

void Server::incomingConnection(int socketDescriptor)
{
#ifdef DEBUG
    qDebug("Server::incomingConnection(%i)", socketDescriptor);
#endif

    QTcpSocket *socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    addPendingConnection(socket);

#ifdef DEBUG
    qDebug("Server::incomingConnection: %s:%i", qPrintable(socket->peerAddress().toString()), socket->peerPort());
#endif
}
