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

class MessagePage : public QWidget
{
    Q_OBJECT

public:
    explicit MessagePage(QWidget *parent = 0);
    MessagePage(QTcpSocket *socket, const QString &username, QWidget *parent = 0);
    ~MessagePage();

    const QString& getUsername() const { return m_username; }
    void appendMessage(const QString &username, const QString &message, const QColor &color = Qt::black);

signals:
    void closeButtonClicked();

public slots:
    void setFocusOnInput();

private slots:
    void on_sendButton_clicked();

private:
    Ui::MessagePage *ui;
    QTcpSocket *m_socket;
    QString m_username;

    void initialize();
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // MESSAGEPAGE_H
