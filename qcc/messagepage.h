#ifndef MESSAGEPAGE_H
#define MESSAGEPAGE_H

#include <QWidget>
#include <QIcon>

QT_BEGIN_NAMESPACE
class QTcpSocket;
QT_BEGIN_NAMESPACE

namespace Ui {
    class MessagePage;
}

class Contact;

class MessagePage : public QWidget
{
    Q_OBJECT

public:
    explicit MessagePage(QWidget *parent = 0);
    MessagePage(QTcpSocket *socket, Contact *contact, QWidget *parent = 0);
    ~MessagePage();

    inline Contact* contact() { return m_contact; }
    void appendMessage(const QString &message, const QColor &color = Qt::black);

signals:
    void closeButtonClicked();

public slots:
    void setFocusOnInput();

private slots:
    void on_sendButton_clicked();
    void contact_statusChanged();

private:
    Ui::MessagePage *ui;
    QTcpSocket *m_socket;
    Contact *m_contact;

    void initialize();
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // MESSAGEPAGE_H
