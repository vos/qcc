#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCryptographicHash>
#include <QDebug>

#include "qccpacket.h"

QCryptographicHash *MainWindow::HASH = new QCryptographicHash(QCryptographicHash::Sha1);

QString MainWindow::hashString(const QString &str)
{
    MainWindow::HASH->reset();
    MainWindow::HASH->addData(str.toAscii());
    return MainWindow::HASH->result().toHex();
}

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
    if (m_packetSize == 0) {
        if (m_socket.bytesAvailable() < (int)sizeof(quint32))
            return;
        in >> m_packetSize;
    }
    if (m_socket.bytesAvailable() < m_packetSize)
        return;
    m_packetSize = 0; // reset packet size

    qint32 type;
    in >> type;

#ifdef DEBUG
    qDebug("MainWindow::socket_readyRead: PacketType = %i", type);
#endif

    switch ((QccPacket::PacketType)type) {
    case QccPacket::ConnectionAccepted:
    {
        QccPacket packet(QccPacket::UserAuthentication);
        packet.stream() << ui->usernameLineEdit->text()
                        << MainWindow::hashString(ui->passwordLineEdit->text());
        packet.send(&m_socket);
        break;
    }
    case QccPacket::ConnectionRefused:
        break;
    case QccPacket::AuthenticationSuccess:
        QccPacket(QccPacket::RequestContactList).send(&m_socket);
        ui->stackedWidget->setCurrentIndex(1);
        break;
    case QccPacket::AuthenticationFailure:
    {
        QString reason;
        in >> reason;
        m_socket.disconnectFromHost();
        qDebug() << "AuthenticationFailure: " << reason;
        break;
    }
    case QccPacket::ContactList:
    {
        qint32 contactCount;
        in >> contactCount;
        for (int i = 0; i < contactCount; i++) {
            QString username;
            qint32 status;
            in >> username >> status;
            QListWidgetItem *contactItem = new QListWidgetItem(style()->standardIcon(status ? QStyle::SP_ArrowUp : QStyle::SP_ArrowDown), username);
            ui->contactListWidget->addItem(contactItem);
        }
        break;
    }
    case QccPacket::Message:
        break;
    case QccPacket::MessageSuccess:
        break;
    case QccPacket::MessageFailure:
        break;
    default:
        qWarning("MainWindow::socket_readyRead: Illegal PacketType %i", type);
        return;
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
    m_packetSize = 0;
    m_socket.connectToHost("127.0.0.1", 12345);
    ui->loginButton->setEnabled(false);
}
