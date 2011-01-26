#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include "qccnamespace.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&m_socket, SIGNAL(connected()), SLOT(socket_connected()));
    connect(&m_socket, SIGNAL(disconnected()), SLOT(socket_disconnected()));
    connect(&m_socket, SIGNAL(readyRead()), SLOT(socket_readyRead()));
    connect(&m_socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(socket_error(QAbstractSocket::SocketError)));
#ifdef DEBUG
    connect(&m_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), SLOT(socket_stateChanged(QAbstractSocket::SocketState)));
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
    qDebug("MainWindow::socket_readyRead: %li bytes available", (long)m_socket.bytesAvailable());
#endif

    QDataStream in(&m_socket);
    in.setVersion(QDataStream::Qt_4_0);
    if (m_blockSize == 0) {
        if (m_socket.bytesAvailable() < (int)sizeof(quint32))
            return;
        in >> m_blockSize;
    }
    if (m_socket.bytesAvailable() < m_blockSize)
        return;
    m_blockSize = 0; // reset block size

    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint32)0;

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
        out << (qint32)Qcc::RequestContactList;
        ui->stackedWidget->setCurrentIndex(1);
        break;
    case Qcc::AuthenticationFailure:
    {
        QString reason;
        in >> reason;
        m_socket.disconnectFromHost();
        qDebug() << "AuthenticationFailure: " << reason;
        break;
    }
    case Qcc::ContactList:
    {
        qint32 contactCount;
        in >> contactCount;
        for (int i = 0; i < contactCount; ++i) {
            QString username;
            qint32 status;
            in >> username >> status;
            QListWidgetItem *contactItem = new QListWidgetItem(style()->standardIcon(status ? QStyle::SP_ArrowUp : QStyle::SP_ArrowDown), username);
            ui->contactListWidget->addItem(contactItem);
        }
        break;
    }
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

    if (data.length() > (int)sizeof(quint32)) {
        out.device()->seek(0);
        out << (quint32)(data.size() - sizeof(quint32));
        m_socket.write(data);
    }
}

void MainWindow::socket_error(QAbstractSocket::SocketError error)
{
#ifdef DEBUG
    qDebug("MainWindow::socket_error(%i) => %s", error, qPrintable(m_socket.errorString()));
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
    m_blockSize = 0;
    m_socket.connectToHost("127.0.0.1", 12345);
    ui->loginButton->setEnabled(false);
}
