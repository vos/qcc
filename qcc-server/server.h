#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QHash>

#include "user.h"

class Server : public QTcpServer
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = 0);
    ~Server();

    void loadUsers();
    void saveUsers();

signals:

private slots:
    void client_readyRead();
    void client_disconnected();

private:
    QHash<QString, User> m_users;
    QHash<QTcpSocket*, quint32> m_blockSizes;
    QHash<QTcpSocket*, User*> m_clients;

    void incomingConnection(int socketDescriptor);

};

#endif // SERVER_H
