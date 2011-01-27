#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
class QCryptographicHash;
QT_BEGIN_NAMESPACE

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
    void socket_connected();
    void socket_disconnected();
    void socket_readyRead();
    void socket_error(QAbstractSocket::SocketError error);
#ifdef DEBUG
    void socket_stateChanged(QAbstractSocket::SocketState state);
#endif

    void on_loginButton_clicked();

private:
    static QCryptographicHash *HASH;
    static QString hashString(const QString &str);

    Ui::MainWindow *ui;
    QTcpSocket m_socket;
    quint32 m_packetSize;
};

#endif // MAINWINDOW_H
