#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QModelIndex>
#include <QtCrypto>

class ContactListModel;
class MessageWindow;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void connectToHost();
    void socket_connected();
    void socket_disconnected();
    void socket_readyRead();
    void socket_error(QAbstractSocket::SocketError error);
#ifdef DEBUG
    void socket_stateChanged(QAbstractSocket::SocketState state);
#endif
    void removeCurrentContact(bool);

    void on_loginButton_clicked();
    void on_registerButton_clicked();
    void on_contactListView_activated(const QModelIndex &index);
    void on_contactListView_customContextMenuRequested(const QPoint &pos);
    void on_addContactButton_clicked();

private:
    static const quint16 DEFAULT_PORT = 54321;

    Ui::MainWindow *ui;
    ContactListModel *m_contacts;
    MessageWindow *m_messageWindow;
    QTcpSocket m_socket;
    quint32 m_packetSize;
    bool m_register;

    QCA::PrivateKey m_privateKey;

    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
