#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QHash>

class User;

class Server : public QTcpServer
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = 0);
    ~Server();

    void loadUsers();
    void saveUsers();

private slots:
    void client_disconnected();
    void client_readyRead();
    void client_statusChanged();

private:
    struct Client {
        explicit inline Client(User *u = NULL) : packetSize(0), user(u) { }
        quint32 packetSize;
        User *user;
    };

    static const QString USERS_FILE;

    QHash<QString, User*> m_users;
    QHash<QTcpSocket*, Client*> m_clients;

    void incomingConnection(int socketDescriptor);
};

#endif // SERVER_H
