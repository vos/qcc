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
    qDeleteAll(m_pages.values());
    delete ui;
}

bool MessageWindow::addTab(const QString &username, const QIcon &icon)
{
    bool tabAdded = false;
    MessagePage *page = NULL;

    if (m_pages.contains(username)) {
        page = m_pages.value(username);
        setCurrentWidget(page);
    } else {
        page = new MessagePage(m_socket, username);
        connect(page, SIGNAL(closeButtonClicked()), SLOT(page_closeButtonClicked()));
        int index = QTabWidget::addTab(page, icon, username);
        m_pages.insert(username, page);
        setCurrentIndex(index);
        tabAdded = true;
    }

    show();
    activateWindow();
    raise();
    if (page != NULL)
        page->setFocusOnInput();

    return tabAdded;
}

void MessageWindow::removeTab(const QString &username)
{
    if (!m_pages.contains(username))
        return;

    // find tab and close it
    for (int i = 0; i < count(); i++) {
        if (tabText(i) == username) {
            tabCloseRequested(i);
            return;
        }
    }
}

void MessageWindow::appendMessage(const QString &username, const QString &message)
{
    if (!m_pages.contains(username))
        return;

    m_pages.value(username)->appendMessage(username, message, Qt::red);
}

void MessageWindow::updateStatus(const QString &username, int status, const QIcon &icon)
{
    if (!m_pages.contains(username))
        return;

    // find tab and update the icon
    for (int i = 0; i < count(); i++) {
        if (tabText(i) == username) {
            setTabIcon(i, icon);
            break;
        }
    }
    m_pages.value(username)->appendMessage("System", QString("%1 is now %2.").arg(username).arg(status ? "online" : "offline"), Qt::gray);
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
    QTabWidget::removeTab(index);
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
