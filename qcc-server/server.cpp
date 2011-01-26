#include "qccnamespace.h"
#include "server.h"

#include <QTcpSocket>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QStringList>
#include <QDebug>

Server::Server(QObject *parent) :
    QTcpServer(parent)
{
    loadUsers();
    //saveUsers();
}

Server::~Server()
{
    saveUsers();
}

void Server::loadUsers()
{
    QFile file("users.xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Server::loadUsers(): cannot read file users.xml => %s", qPrintable(file.errorString()));
        return;
    }
    m_users.clear();
    QXmlStreamReader xml(&file);
    while (!xml.atEnd()) {
        if (!xml.readNextStartElement())
            continue;
        if (xml.name() == "user") {
            QXmlStreamAttributes attr = xml.attributes();
            QString username = attr.value("username").toString();
            QString password = attr.value("password").toString();
            //qDebug() << "user: " << " username = " << username << ", password = " << password;
            m_users.insert(username, User(username, password));
        }
    }
    if (xml.hasError()) {
        qCritical("Server::loadUsers(): parse error in file users.xml => %s: line %li, column %li",
                  qPrintable(xml.errorString()), (long)xml.lineNumber(), (long)xml.columnNumber());
    }
    file.close();

#ifdef DEBUG
    qDebug("Server::loadUsers(): %i users read from users.xml", m_users.count());
#endif
}

void Server::saveUsers()
{
    QFile file("users.xml");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning("Server::saveUsers(): cannot write file users.xml => %s", qPrintable(file.errorString()));
        return;
    }
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("users");
    foreach (const User &user, m_users) {
        xml.writeStartElement("user");
        xml.writeAttribute("username", user.getUsername());
        xml.writeAttribute("password", user.getPassword());
        xml.writeEndElement();
    }
    xml.writeEndElement();
    xml.writeEndDocument();
    file.close();

#ifdef DEBUG
    qDebug("Server::saveUsers(): %i users written to users.xml", 5);
#endif
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

    m_blockSizes.insert(client, 0);
    m_clients.insert(client, NULL);

    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint32)sizeof(qint32);
    out << (qint32)Qcc::ConnectionAccepted;
    //out << (qint32)Qcc::ConnectionRefused;
    client->write(data);

#ifdef DEBUG
    qDebug("Connection accepted");
#endif
}

void Server::client_readyRead()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client) {
        qWarning("Server::client_readyRead: Cast of sender() to QTcpSocket* failed");
        return;
    }

#ifdef DEBUG
    qDebug("Server::client_readyRead: %li bytes available", (long)client->bytesAvailable());
#endif

    QDataStream in(client);
    in.setVersion(QDataStream::Qt_4_0);
    if (m_blockSizes.value(client) == 0) {
        if (client->bytesAvailable() < (int)sizeof(quint32))
            return;
        in >> m_blockSizes[client];
    }
    if (client->bytesAvailable() < m_blockSizes.value(client))
        return;
    m_blockSizes[client] = 0; // reset block size

    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint32)0;

    qint32 type;
    in >> type;

#ifdef DEBUG
    User *user_debug = m_clients[client];
    QString username_debug = user_debug ? user_debug->getUsername() : "[no user object]";
    qDebug("Server::client_readyRead: User = '%s', MessageType = %i", qPrintable(username_debug), type);
#endif

    switch ((Qcc::MessageType)type) {
    case Qcc::UserAuthentication:
    {
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
    case Qcc::RequestContactList:
    {
        out << (qint32)Qcc::ContactList;
        QStringList contacts;
        contacts << "kai" << "till" << "ernst";
        out << contacts;
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

    if (data.length() > (int)sizeof(quint32)) {
        out.device()->seek(0);
        out << (quint32)(data.size() - sizeof(quint32));
        client->write(data);
    }
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

    m_blockSizes.remove(client);
    m_clients.remove(client);
    if (user)
        delete user;
}
