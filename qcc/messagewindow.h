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

#ifndef MESSAGEWINDOW_H
#define MESSAGEWINDOW_H

#include <QTabWidget>
#include <QHash>

QT_BEGIN_NAMESPACE
class QTcpSocket;
QT_BEGIN_NAMESPACE

class Contact;
class MessagePage;

namespace Ui {
    class MessageWindow;
}

class MessageWindow : public QTabWidget
{
    Q_OBJECT

public:
    explicit MessageWindow(QTcpSocket *socket, QWidget *parent = 0);
    ~MessageWindow();

    bool addTab(Contact *contact);
    void closeTab(Contact *contact);
    void closeAllTabs();
    void appendMessage(Contact *contact, const QString &message);

private slots:
    void contact_statusChanged();
    void tabCloseRequested(int index);
    void page_closeButtonClicked();

private:
    Ui::MessageWindow *ui;
    QTcpSocket *m_socket;
    QHash<Contact*, MessagePage*> m_pages;
};

#endif // MESSAGEWINDOW_H
