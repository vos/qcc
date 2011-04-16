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

#ifndef CONTACTLISTMODEL_H
#define CONTACTLISTMODEL_H

#include <QAbstractListModel>
#include <QList>

class Contact;

class ContactListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ContactListModel(QObject *parent = 0);
    ~ContactListModel();

    int rowCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    Contact* contact(const QString &contactName);
    Contact* contact(const QModelIndex &index);
    void add(Contact *contact);
    void add(const QList<Contact*> &contacts);
    bool remove(const Contact *contact);
    bool remove(const QString &contactName);
    bool remove(const QModelIndex &index);

public slots:
    void clear();

private slots:
    void contact_statusChanged();

private:
    QList<Contact*> m_contacts;
};

#endif // CONTACTLISTMODEL_H
