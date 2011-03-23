#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QModelIndex>
#include <QtCrypto>

QT_BEGIN_NAMESPACE
class QCryptographicHash;
QT_BEGIN_NAMESPACE

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
    static QCryptographicHash *Hash;
    static QString hashString(const QString &str);

    Ui::MainWindow *ui;
    ContactListModel *m_contacts;
    MessageWindow *m_messageWindow;
    QCA::Initializer *m_qcaInit;
    QTcpSocket m_socket;
    quint32 m_packetSize;
    bool m_register;

    QCA::PrivateKey m_privateKey;

    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
