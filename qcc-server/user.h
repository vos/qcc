#ifndef USER_H
#define USER_H

#include "abstractuser.h"

#include <QSet>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

QT_BEGIN_NAMESPACE
class QTcpSocket;
QT_END_NAMESPACE

class User : public AbstractUser
{
    Q_OBJECT

public:
    explicit User(QObject *parent = 0);
    User(const QString &username, const QString &password, QObject *parent = 0);

    inline const QString& password() const { return m_password; }
    inline const QSet<QString>& contacts() const { return m_contacts; }
    inline QTcpSocket* socket() const { return m_socket; }

    inline void addContact(const QString &username) { m_contacts.insert(username); }
    inline bool containsContact(const QString &username) const { return m_contacts.contains(username); }
    inline bool removeContact(const QString &username) { return m_contacts.remove(username); }
    inline void clearContacts() { m_contacts.clear(); }

    inline bool matchPassword(const QString &password) const { return m_password == password; };

    static User* readUser(QXmlStreamReader &xml);
    void writeUser(QXmlStreamWriter &xml);

public slots:
    void setPassword(const QString &password) { m_password = password; }
    void setSocket(QTcpSocket *socket) { m_socket = socket; }
    void reset();

private:
    QString m_password;
    QSet<QString> m_contacts;
    QTcpSocket *m_socket;
};

#endif // USER_H
