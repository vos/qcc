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
    QHash<QTcpSocket*, quint32> m_blockSizes;
    QHash<QTcpSocket*, User*> m_clients;

    void incomingConnection(int socketDescriptor);

};

#endif // SERVER_H
