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
    void client_disconnected();
    void client_readyRead();
    void client_statusChanged();

private:
    struct Client {
        Client(User *u = NULL) : packetSize(0), user(u) { }
        quint32 packetSize;
        User *user;
    };

    QHash<QString, User*> m_users;
    QHash<QTcpSocket*, Client*> m_clients;

    void incomingConnection(int socketDescriptor);
};

#endif // SERVER_H
