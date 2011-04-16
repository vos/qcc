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

#ifndef ABSTRACTUSER_H
#define ABSTRACTUSER_H

#include <QObject>

class AbstractUser : public QObject
{
    Q_OBJECT

public:
    enum Status {
        Offline,
        Online
    };

    static QString statusString(Status type);

    explicit AbstractUser(QObject *parent = 0);
    AbstractUser(const QString &username, QObject *parent = 0);

    inline const QString& username() const { return m_username; }
    inline Status status() const { return m_status; }
    inline QString statusString() const { return AbstractUser::StatusNames[m_status]; }

    inline bool isValid() const { return !m_username.isEmpty(); }
    inline void invalidate() { m_username.clear(); }
    inline bool isOnline() const { return m_status == Online; }

signals:
    void statusChanged();

public slots:
    inline void setUsername(const QString &username) { m_username = username; }
    void setStatus(Status status);
    virtual void reset();

protected:
    QString m_username;
    Status m_status;

private:
    static const char *StatusNames[];
};

#endif // ABSTRACTUSER_H
