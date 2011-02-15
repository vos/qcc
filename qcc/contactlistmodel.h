#ifndef CONTACTLISTMODEL_H
#define CONTACTLISTMODEL_H

#include <QAbstractListModel>
#include <QList>

class Contact;

class ContactListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ContactListModel(QObject *parent = 0);
    ~ContactListModel();

    int rowCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    Contact* contact(const QString &contactName);
    Contact* contact(const QModelIndex &index);
    void add(Contact *contact);
    void add(const QList<Contact*> &contacts);
    bool remove(const Contact *contact);
    bool remove(const QString &contactName);
    bool remove(const QModelIndex &index);

public slots:
    void clear();

private:
    QList<Contact*> m_contacts;
};

#endif // CONTACTLISTMODEL_H
