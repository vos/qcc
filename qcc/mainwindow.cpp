#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qccnamespace.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&socket, SIGNAL(connected()), SLOT(socket_connected()));
    connect(&socket, SIGNAL(disconnected()), SLOT(socket_disconnected()));
    connect(&socket, SIGNAL(readyRead()), SLOT(socket_readyRead()));
    connect(&socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(socket_error(QAbstractSocket::SocketError)));
#ifdef DEBUG
    connect(&socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), SLOT(socket_stateChanged(QAbstractSocket::SocketState)));
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::socket_connected()
{
#ifdef DEBUG
    qDebug("MainWindow::socket_connected");
#endif
}

void MainWindow::socket_disconnected()
{
#ifdef DEBUG
    qDebug("MainWindow::socket_disconnected");
#endif

    if (isHidden()) // hack, true when window is closing
        return;

    ui->stackedWidget->setCurrentIndex(0);
    ui->loginButton->setEnabled(true);
}

void MainWindow::socket_readyRead()
{
#ifdef DEBUG
    qDebug("MainWindow::socket_readyRead: %li bytes available", socket.bytesAvailable());
#endif
    if (socket.bytesAvailable() < sizeof(qint32)) {
        qWarning("MainWindow::socket_readyRead: Not enough data to read");
        return;
    }

    QDataStream in(&socket);
    in.setVersion(QDataStream::Qt_4_0);
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    qint32 type;
    in >> type;
#ifdef DEBUG
    qDebug("MainWindow::socket_readyRead: MessageType = %i", type);
#endif
    switch ((Qcc::MessageType)type) {
    case Qcc::ConnectionAccepted:
        out << (qint32)Qcc::UserAuthentication;
        out << ui->usernameLineEdit->text() << ui->passwordLineEdit->text(); // TODO encode password
        break;
    case Qcc::ConnectionRefused:
        break;
    case Qcc::AuthenticationSuccess:
        ui->stackedWidget->setCurrentIndex(1);
        break;
    case Qcc::AuthenticationFailure:
        break;
    case Qcc::Message:
        break;
    case Qcc::MessageSuccess:
        break;
    case Qcc::MessageFailure:
        break;
    default:
        qWarning("MainWindow::socket_readyRead: Illegal MessageType %i", type);
        return;
    }

    if (data.length() > 0)
        socket.write(data);
}

void MainWindow::socket_error(QAbstractSocket::SocketError error)
{
#ifdef DEBUG
    qDebug("MainWindow::socket_error(%i) => %s", error, qPrintable(socket.errorString()));
#endif

    ui->loginButton->setEnabled(true);
}

#ifdef DEBUG
void MainWindow::socket_stateChanged(QAbstractSocket::SocketState state)
{
    qDebug("MainWindow::socket_stateChanged(%i)", state);
}
#endif

void MainWindow::on_loginButton_clicked()
{
    socket.connectToHost("127.0.0.1", 12345);
    ui->loginButton->setEnabled(false);
}
