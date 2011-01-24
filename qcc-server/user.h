#ifndef USER_H
#define USER_H

#include <QObject>

class User : public QObject
{
    Q_OBJECT

public:
    enum Status { Offline, Online };

    explicit User(QObject *parent = 0);
    User(const QString &username, const QString &password, QObject *parent = 0);

    const QString& getUsername() const { return m_username; }
    const QString& getPassword() const { return m_password; }
    const Status getStatus() const { return m_status; }

signals:
    void statusChanged();

public slots:
    void setUsername(const QString &username) { m_username = username; }
    void setPassword(const QString &password) { m_password = password; }
    void setStatus(const Status status) { if (status == m_status) return; m_status = status; emit statusChanged(); }

private:
    QString m_username;
    QString m_password;
    Status m_status;

};

#endif // USER_H
