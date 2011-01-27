#ifndef USER_H
#define USER_H

#include <QObject>
#include <QSet>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

QT_BEGIN_NAMESPACE
class QTcpSocket;
QT_END_NAMESPACE

class User : public QObject
{
    Q_OBJECT

public:
    enum Status { Offline, Online };

    explicit User(QObject *parent = 0);
    User(const QString &username, const QString &password, QObject *parent = 0);
    ~User();
    User(const User &other);
    User& operator =(const User &other);

    const QString& getUsername() const { return m_username; }
    const QString& getPassword() const { return m_password; }
    Status getStatus() const { return m_status; }
    const QSet<QString>& getContacts() const { return m_contacts; }
    QTcpSocket* getSocket() const { return m_socket; }

    void addContact(const QString &username);
    bool removeContact(const QString &username);
    void clearContacts();

    bool isValid() const { return !m_username.isEmpty(); }
    void invalidate() { m_username.clear(); }
    bool matchPassword(const QString &password) const;
    bool isOnline() const { return m_status == Online; }

    static User* readUser(QXmlStreamReader &xml);
    void writeUser(QXmlStreamWriter &xml);

signals:
    void statusChanged();

public slots:
    void setUsername(const QString &username) { m_username = username; }
    void setPassword(const QString &password) { m_password = password; }
    void setStatus(Status status) { if (status == m_status) return; m_status = status; emit statusChanged(); }
    void setSocket(QTcpSocket *socket) { m_socket = socket; }
    void reset();

private:
    QString m_username;
    QString m_password;
    Status m_status;
    QSet<QString> m_contacts;
    QTcpSocket *m_socket;
};

#endif // USER_H
