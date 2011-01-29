#include "messagepage.h"
#include "ui_messagepage.h"

#include "qccpacket.h"

MessagePage::MessagePage(QWidget *parent) :
    QWidget(parent), ui(new Ui::MessagePage)
{
    ui->setupUi(this);
}

MessagePage::MessagePage(QTcpSocket *socket, const QString &username, const QIcon &icon, QWidget *parent) :
    QWidget(parent), ui(new Ui::MessagePage), m_socket(socket), m_username(username), m_icon(icon)
{
    ui->setupUi(this);
}

MessagePage::~MessagePage()
{
    delete ui;
}

void MessagePage::appendMessage(const QString &username, const QString &message, const QColor &color)
{
    QString messageHtml = message;
    messageHtml.replace('\n', "<br/>");
    ui->messagesTextEdit->append("<span style=\"color: " + color.name() + ";\">" + username + "</span>: " + messageHtml);
}

void MessagePage::on_closeButton_clicked()
{
    emit closeButtonClicked();
}

void MessagePage::on_sendButton_clicked()
{
    QString text = ui->messageTextEdit->toPlainText();
    if (text.isEmpty())
        return;

    appendMessage("You", text, Qt::blue);

    QccPacket message;
    message.stream() << qint32(qrand()) << m_username << text;
    message.send(m_socket);

    ui->messageTextEdit->clear();
    ui->messageTextEdit->setFocus();
}
