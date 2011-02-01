#include "messagepage.h"
#include "ui_messagepage.h"

#include <QKeyEvent>

#include "qccpacket.h"

MessagePage::MessagePage(QWidget *parent) :
    QWidget(parent), ui(new Ui::MessagePage)
{
    initialize();
}

MessagePage::MessagePage(QTcpSocket *socket, const QString &username, QWidget *parent) :
    QWidget(parent), ui(new Ui::MessagePage), m_socket(socket), m_username(username)
{
    initialize();
}

void MessagePage::initialize()
{
    ui->setupUi(this);

    connect(ui->closeButton, SIGNAL(clicked()), SIGNAL(closeButtonClicked()));
    ui->messageTextEdit->installEventFilter(this);
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

bool MessagePage::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->messageTextEdit) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if ((keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) && keyEvent->modifiers() == Qt::NoModifier) {
                on_sendButton_clicked();
                return true;
            } else
                return false;
        } else
            return false;
    } else
        return QWidget::eventFilter(obj, event);
}
