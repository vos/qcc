#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);

signals:

public slots:

private:
    void incomingConnection(int socketDescriptor);

};

#endif // SERVER_H
