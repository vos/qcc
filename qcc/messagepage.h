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

#ifndef MESSAGEPAGE_H
#define MESSAGEPAGE_H

#include <QWidget>
#include <QIcon>

QT_BEGIN_NAMESPACE
class QTcpSocket;
QT_BEGIN_NAMESPACE

namespace Ui {
    class MessagePage;
}

class Contact;

class MessagePage : public QWidget
{
    Q_OBJECT

public:
    explicit MessagePage(QWidget *parent = 0);
    MessagePage(QTcpSocket *socket, Contact *contact, QWidget *parent = 0);
    ~MessagePage();

    inline Contact* contact() { return m_contact; }
    void appendMessage(const QString &message, const QColor &color = Qt::black);

signals:
    void closeButtonClicked();

public slots:
    void setFocusOnInput();

private slots:
    void on_sendButton_clicked();
    void contact_statusChanged();

private:
    Ui::MessagePage *ui;
    QTcpSocket *m_socket;
    Contact *m_contact;

    void initialize();
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // MESSAGEPAGE_H
