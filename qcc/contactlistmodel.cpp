#include "contactlistmodel.h"

#include "contact.h"

ContactListModel::ContactListModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

ContactListModel::~ContactListModel()
{
    qDeleteAll(m_contacts);
}

int ContactListModel::rowCount(const QModelIndex &) const
{
    return m_contacts.count();
}

QVariant ContactListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_contacts.size())
        return QVariant();

    if (role == Qt::DisplayRole)
        return m_contacts.at(index.row())->username();
    else if (role == Qt::DecorationRole)
        return m_contacts.at(index.row())->statusIcon();

    return QVariant();
}

void ContactListModel::add(Contact *contact)
{
    if (!contact)
        return;

    beginInsertRows(QModelIndex(), m_contacts.count(), m_contacts.count());
    m_contacts.append(contact);
    endInsertRows();
}

void ContactListModel::add(const QList<Contact*> &contacts)
{
    if (contacts.count() <= 0)
        return;

    beginInsertRows(QModelIndex(), m_contacts.count(), m_contacts.count() + contacts.count());
    m_contacts.append(contacts);
    endInsertRows();
}

bool ContactListModel::remove(const Contact *contact)
{
    if (!contact)
        return false;

    return remove(contact->username());
}

bool ContactListModel::remove(const QString &contactName)
{
    for (int i = 0; i < m_contacts.count(); i++) {
        if (m_contacts.at(i)->username() == contactName) {
            return remove(index(i));
        }
    }
    return false;
}

bool ContactListModel::remove(const QModelIndex &index)
{
    if (index.row() >= m_contacts.count())
        return false;

    beginRemoveRows(QModelIndex(), index.row(), index.row());
    delete m_contacts[index.row()];
    m_contacts.removeAt(index.row());
    endRemoveRows();

    return true;
}

void ContactListModel::clear()
{
    if (m_contacts.count() <= 0)
        return;

    beginResetModel();
    qDeleteAll(m_contacts);
    m_contacts.clear();
    endResetModel();
}

Contact* ContactListModel::contact(const QString &contactName)
{
    foreach (Contact *contact, m_contacts) {
        if (contact && contact->username() == contactName)
            return contact;
    }
    return NULL;
}

Contact* ContactListModel::contact(const QModelIndex &index)
{
    if (index.row() >= m_contacts.count())
        return NULL;
    return m_contacts.at(index.row());
}
