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

signals:

private slots:
    void client_readyRead();
    void client_disconnected();

private:
    enum MessageType {
        ConnectionAccepted,
        ConnectionRefused,
        UserAuthentication,
        AuthenticationSuccess,
        AuthenticationFailure,
        Message,
        MessageSuccess,
        MessageFailure,
        IllegalMessage = -1
    };

    QHash<QTcpSocket*, User*> m_clients;

    void incomingConnection(int socketDescriptor);

};

#endif // SERVER_H
