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
