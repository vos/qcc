#ifndef USER_H
#define USER_H

#include <QObject>
#include <QSet>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

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

    void addContact(const QString &username);
    bool removeContact(const QString &username);
    void clearContacts();

    bool isValid() const { return !m_username.isEmpty(); }
    void invalidate() { m_username.clear(); }
    bool matchPassword(const QString &password) const;

    static User readUser(QXmlStreamReader &xml);
    void writeUser(QXmlStreamWriter &xml);

signals:
    void statusChanged();

public slots:
    void setUsername(const QString &username) { m_username = username; }
    void setPassword(const QString &password) { m_password = password; }
    void setStatus(Status status) { if (status == m_status) return; m_status = status; emit statusChanged(); }

private:
    QString m_username;
    QString m_password;
    Status m_status;
    QSet<QString> m_contacts;

};

#endif // USER_H
