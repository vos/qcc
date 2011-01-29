#ifndef MESSAGEWINDOW_H
#define MESSAGEWINDOW_H

#include <QTabWidget>
#include <QHash>

QT_BEGIN_NAMESPACE
class QTcpSocket;
QT_BEGIN_NAMESPACE

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

    bool addTab(const QString &username, const QIcon &icon);
    void appendMessage(const QString &username, const QString &message);

private slots:
    void tabCloseRequested(int index);
    void page_closeButtonClicked();

private:
    Ui::MessageWindow *ui;
    QTcpSocket *m_socket;
    QHash<QString, MessagePage*> m_pages;
};

#endif // MESSAGEWINDOW_H
