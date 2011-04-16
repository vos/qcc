/****************************************************************************
**
** Copyright (C) 2011 Alexander Vos <info@vossoft.de>,
**                    Kai Wellmann <KaiWellmann@gmx.de>
**
** This file is part of Qt Crypto Chat (QCC).
**
** Qt Crypto Chat is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Qt Crypto Chat is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with Qt Crypto Chat.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

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
