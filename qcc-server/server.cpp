#include "server.h"
#include "qccpacket.h"

#include <QTcpSocket>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDebug>

Server::Server(QObject *parent) :
    QTcpServer(parent)
{
    loadUsers();
    //saveUsers();
}

Server::~Server()
{
    //saveUsers();
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
            User user = User::readUser(xml);
            if (user.isValid()) {
                m_users.insert(user.getUsername(), user);
                qDebug() << "user: " << user.getUsername() << ", password = " << user.getPassword()
                         << ", contacts = " << user.getContacts().count();
            }
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
    foreach (User user, m_users)
        user.writeUser(xml);
    xml.writeEndElement();
    xml.writeEndDocument();
    file.close();

#ifdef DEBUG
    qDebug("Server::saveUsers(): %i users written to users.xml", m_users.count());
#endif
}

void Server::incomingConnection(int socketDescriptor)
{
    QTcpSocket *socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    addPendingConnection(socket);

#ifdef DEBUG
    qDebug("Server::incomingConnection(%i): %s:%i", socketDescriptor, qPrintable(socket->peerAddress().toString()), socket->peerPort());
#endif

    connect(socket, SIGNAL(readyRead()), SLOT(client_readyRead()));
    connect(socket, SIGNAL(disconnected()), SLOT(client_disconnected()));

    m_clients.insert(socket, new Client);
    QccPacket(QccPacket::ConnectionAccepted).send(socket); // QccPacket::ConnectionRefused

#ifdef DEBUG
    qDebug("Connection accepted");
#endif
}

void Server::client_disconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) {
        qWarning("Server::client_disconnected: Cast of sender() to QTcpSocket* failed");
        return;
    }

    Client *client = m_clients[socket];
    User *user = client->user;
    if (user)
        user->reset();

#ifdef DEBUG
    QString username = user ? user->getUsername() : "[no user object]";
    qDebug("client disconnected: '%s' (%s:%i)", qPrintable(username), qPrintable(socket->peerAddress().toString()), socket->peerPort());
#endif

    m_clients.remove(socket);
    delete client;
}

void Server::client_readyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) {
        qWarning("Server::client_readyRead: Cast of sender() to QTcpSocket* failed");
        return;
    }

#ifdef DEBUG
    qDebug("Server::client_readyRead: %li bytes available", (long)socket->bytesAvailable());
#endif

    Client *client = m_clients[socket];
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_0);
    if (client->packetSize == 0) {
        if (socket->bytesAvailable() < (int)sizeof(quint32))
            return;
        in >> client->packetSize;
    }
    if (socket->bytesAvailable() < client->packetSize)
        return;
    client->packetSize = 0; // reset packet size

    qint32 type;
    in >> type;

#ifdef DEBUG
    qDebug("Server::client_readyRead: User = '%s', PacketType = %i", qPrintable(client->user ? client->user->getUsername() : "[no user object]"), type);
#endif

    switch ((QccPacket::PacketType)type) {
    case QccPacket::UserRegister:
    {
        QString username, password;
        in >> username >> password;
#ifdef DEBUG
        qDebug("Server::client_readyRead: username = '%s' password = '%s'", qPrintable(username), qPrintable(password));
#endif
        if (username.isEmpty()) {
            QString reason = "The username cannot be empty.";
            QccPacket packet(QccPacket::RegisterFailure);
            packet.stream() << reason;
            packet.send(socket);
#ifdef DEBUG
            qDebug("RegisterFailure: %s", qPrintable(reason));
#endif
            break;
        }
        if (m_users.contains(username)) {
            QString reason = QString("The username \"%1\" is not available.").arg(username);
            QccPacket packet(QccPacket::RegisterFailure);
            packet.stream() << reason;
            packet.send(socket);
#ifdef DEBUG
            qDebug("RegisterFailure: %s", qPrintable(reason));
#endif
            break;
        }
        User u(username, password);
        u.setStatus(User::Online);
        m_users.insert(username, u);
        QccPacket(QccPacket::RegisterSuccess).send(socket);
#ifdef DEBUG
            qDebug("RegisterSuccess");
#endif
        break;
    }
    case QccPacket::UserAuthentication:
    {
        QString username, password;
        in >> username >> password;
#ifdef DEBUG
        qDebug("Server::client_readyRead: username = '%s' password = '%s'", qPrintable(username), qPrintable(password));
#endif
        User *user = m_users.contains(username) ? &m_users[username] : NULL;
        if (user && user->matchPassword(password)) {
            user->setStatus(User::Online);
            user->setSocket(socket);
            m_clients[socket]->user = user;
            QccPacket(QccPacket::AuthenticationSuccess).send(socket);
#ifdef DEBUG
            qDebug("AuthenticationSuccess");
#endif
        } else {
            const QString reason = "The username or password you entered is incorrect.";
            QccPacket packet(QccPacket::AuthenticationFailure);
            packet.stream() << reason;
            packet.send(socket);
#ifdef DEBUG
            qDebug("AuthenticationFailure: %s", qPrintable(reason));
#endif
        }
        break;
    }
    case QccPacket::RequestAuthorization:
    {
        QString username;
        in >> username;
        if (!m_users.contains(username)) {
            QString reason = QString("The username \"%1\" does not exist.").arg(username);
            QccPacket packet(QccPacket::AuthorizationFailure);
            packet.stream() << reason;
            packet.send(socket);
#ifdef DEBUG
            qDebug("AuthorizationFailure: %s", qPrintable(reason));
#endif
            break;
        }
        User receiver = m_users.value(username);
        if (receiver.isOnline()) {
            QccPacket p(QccPacket::RequestAuthorization);
            p.stream() << "username" << "password";
            p.send(receiver.getSocket());
#ifdef DEBUG
            qDebug("RequestAuthorization: forwarded to '%s'", qPrintable(username));
#endif
        } else {
            QString reason = QString("The user \"%1\" is not online.").arg(username);
            QccPacket packet(QccPacket::AuthorizationFailure);
            packet.stream() << reason;
            packet.send(socket);
#ifdef DEBUG
            qDebug("AuthorizationFailure: %s", qPrintable(reason));
#endif
        }
        break;
    }
    case QccPacket::AuthorizationAccepted:
    case QccPacket::AuthorizationRejected:
        break;
    case QccPacket::RequestContactList:
    {
        QccPacket packet(QccPacket::ContactList);
        QSet<QString> contacts = client->user->getContacts();
        packet.stream() << (qint32)contacts.count();
        foreach (const QString &contactName, contacts) {
            User contact = m_users.value(contactName);
            packet.stream() << contactName << (qint32)contact.getStatus();
        }
        packet.send(socket);
        break;
    }
    case QccPacket::Message:
        // TODO read and forward message
        break;
    default:
        qWarning("Server::client_readyRead: Illegal PacketType %i", type);
        return;
    }
}
