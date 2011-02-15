#include "messagepage.h"
#include "ui_messagepage.h"

#include <QKeyEvent>

#include "qccpacket.h"
#include "contact.h"

MessagePage::MessagePage(QWidget *parent) :
    QWidget(parent), ui(new Ui::MessagePage)
{
    initialize();
}

MessagePage::MessagePage(QTcpSocket *socket, Contact *contact, QWidget *parent) :
    QWidget(parent), ui(new Ui::MessagePage), m_socket(socket), m_contact(contact)
{
    initialize();
}

void MessagePage::initialize()
{
    Q_ASSERT(m_contact); // should never happen, just in case...

    ui->setupUi(this);
    connect(m_contact, SIGNAL(statusChanged()), SLOT(contact_statusChanged()));
    connect(ui->closeButton, SIGNAL(clicked()), SIGNAL(closeButtonClicked()));
    ui->messageTextEdit->installEventFilter(this);
}

MessagePage::~MessagePage()
{
    delete ui;
}

void MessagePage::appendMessage(Contact *contact, const QString &message, const QColor &color)
{
    QString messageHtml = message;
    messageHtml.replace('\n', "<br/>");
    ui->messagesTextEdit->append("<span style=\"color: " + color.name() + ";\">" +
                                 (contact ? contact->username() : "You") + "</span>: " + messageHtml);
}

void MessagePage::contact_statusChanged()
{
    ui->messagesTextEdit->append(QString("%1 is now %2.").arg(m_contact->username()).arg(m_contact->statusString()));
}

void MessagePage::on_sendButton_clicked()
{
    QString text = ui->messageTextEdit->toPlainText();
    if (text.isEmpty())
        return;

    appendMessage(NULL, text, Qt::blue);

    QccPacket message;
    message.stream() << qint32(qrand()) << m_contact->username() << text;
    message.send(m_socket);

    ui->messageTextEdit->clear();
    ui->messageTextEdit->setFocus();
}

void MessagePage::setFocusOnInput()
{
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
