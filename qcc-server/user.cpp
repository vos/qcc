#include "user.h"

#include <QTcpSocket>

User::User(QObject *parent) :
    QObject(parent), m_status(Offline), m_socket(NULL)
{
}

User::User(const QString &username, const QString &password, QObject *parent) :
    QObject(parent), m_username(username), m_password(password), m_status(Offline), m_socket(NULL)
{
}

User::User(const User &other) :
    m_username(other.m_username), m_password(other.m_password),
    m_status(other.m_status), m_contacts(other.m_contacts), m_socket(other.m_socket)
{
}

User& User::operator =(const User &other)
{
    if (this != &other) {
        m_username = other.m_username;
        m_password = other.m_password;
        m_status = other.m_status;
        m_contacts = other.m_contacts;
        m_socket = other.m_socket;
    }
    return *this;
}

void User::addContact(const QString &username)
{
    m_contacts.insert(username);
}

bool User::removeContact(const QString &username)
{
    return m_contacts.remove(username);
}

void User::clearContacts()
{
    m_contacts.clear();
}

bool User::matchPassword(const QString &password) const
{
    return m_password == password;
}

User* User::readUser(QXmlStreamReader &xml)
{
    QXmlStreamAttributes attr = xml.attributes();
    QString username = attr.value("username").toString();
    QString password = attr.value("password").toString();
    User *user = new User(username, password);
    while (!xml.atEnd()) {
        switch (xml.readNext()) {
        case QXmlStreamReader::Invalid:
            return NULL;
        case QXmlStreamReader::StartElement:
            if (xml.name() == "contact") {
                if (xml.readNext() == QXmlStreamReader::Characters) {
                    QString contactName = xml.text().toString();
                    user->addContact(contactName);
                }
            }
            break;
        case QXmlStreamReader::EndElement:
            if (xml.name() == "user")
                return user;
            else
                break;
        default:
            break;
        }
    }
    return NULL;
}

void User::writeUser(QXmlStreamWriter &xml)
{
    xml.writeStartElement("user");
    xml.writeAttribute("username", m_username);
    xml.writeAttribute("password", m_password);
    xml.writeStartElement("contacts");
    foreach (const QString &contactName, m_contacts)
        xml.writeTextElement("contact", contactName);
    xml.writeEndElement();
    xml.writeEndElement();
}

void User::reset()
{
    setStatus(Offline);
    m_socket = NULL;
}
