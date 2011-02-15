#ifndef ABSTRACTUSER_H
#define ABSTRACTUSER_H

#include <QObject>

class AbstractUser : public QObject
{
    Q_OBJECT

public:
    enum Status {
        Offline,
        Online
    };

    static QString statusString(Status type);

    explicit AbstractUser(QObject *parent = 0);
    AbstractUser(const QString &username, QObject *parent = 0);

    inline const QString& username() const { return m_username; }
    inline Status status() const { return m_status; }
    inline QString statusString() const { return AbstractUser::StatusNames[m_status]; }

    inline bool isValid() const { return !m_username.isEmpty(); }
    inline void invalidate() { m_username.clear(); }
    inline bool isOnline() const { return m_status == Online; }

signals:
    void statusChanged();

public slots:
    inline void setUsername(const QString &username) { m_username = username; }
    void setStatus(Status status);
    virtual void reset();

protected:
    QString m_username;
    Status m_status;

private:
    static const char *StatusNames[];
};

#endif // ABSTRACTUSER_H
