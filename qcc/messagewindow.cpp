#include "messagewindow.h"
#include "ui_messagewindow.h"

#include "messagepage.h"

MessageWindow::MessageWindow(QTcpSocket *socket, QWidget *parent) :
    QTabWidget(parent), ui(new Ui::MessageWindow), m_socket(socket)
{
    ui->setupUi(this);

    connect(this, SIGNAL(tabCloseRequested(int)), SLOT(tabCloseRequested(int)));
}

MessageWindow::~MessageWindow()
{
    delete ui;
}

bool MessageWindow::addTab(const QString &username, const QIcon &icon)
{
    if (m_pages.contains(username)) {
        setCurrentWidget(m_pages.value(username));
        return false;
    }

    MessagePage *page = new MessagePage(m_socket, username, icon);
    connect(page, SIGNAL(closeButtonClicked()), SLOT(page_closeButtonClicked()));
    int index = QTabWidget::addTab(page, icon, username);
    m_pages.insert(username, page);
    setCurrentIndex(index);
    return true;
}

void MessageWindow::appendMessage(const QString &username, const QString &message)
{
    if (!m_pages.contains(username))
        return;
    m_pages.value(username)->appendMessage(username, message, Qt::red);
}

void MessageWindow::tabCloseRequested(int index)
{
    MessagePage *page = qobject_cast<MessagePage*>(widget(index));
    if (!page) {
        qWarning("MessageWindow::tabCloseRequested(int): Cast of sender() to MessagePage* failed");
        return;
    }

    page->disconnect();
    m_pages.remove(page->getUsername());
    removeTab(index);
    delete page;

#ifdef DEBUG
    qDebug("tab closed %i", index);
#endif

    if (count() == 0) {
#ifdef DEBUG
    qDebug("last tab closed => closing MessageWindow");
#endif
        close();
    }
}

void MessageWindow::page_closeButtonClicked()
{
    emit tabCloseRequested(currentIndex());
}
