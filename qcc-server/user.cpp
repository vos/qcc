#include "user.h"

User::User(QObject *parent) :
    QObject(parent), m_status(Offline)
{
}

User::User(const QString &username, const QString &password, QObject *parent) :
    QObject(parent), m_username(username), m_password(password), m_status(Offline)
{
}

User::~User()
{
}

User::User(const User &other) :
    m_username(other.m_username), m_password(other.m_password),
    m_status(other.m_status), m_contacts(other.m_contacts)
{
}

User& User::operator =(const User &other)
{
    if (this != &other) {
        m_username = other.m_username;
        m_password = other.m_password;
        m_status = other.m_status;
        m_contacts = other.m_contacts;
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

User User::readUser(QXmlStreamReader &xml)
{
    QXmlStreamAttributes attr = xml.attributes();
    QString username = attr.value("username").toString();
    QString password = attr.value("password").toString();
    User user(username, password);
    while (!xml.atEnd()) {
        switch (xml.readNext()) {
        case QXmlStreamReader::Invalid:
            return User();
        case QXmlStreamReader::StartElement:
            if (xml.name() == "contact") {
                QString contact = xml.attributes().value("username").toString();
                user.addContact(contact);
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
    return User();
}

void User::writeUser(QXmlStreamWriter &xml)
{
    xml.writeStartElement("user");
    xml.writeAttribute("username", m_password);
    xml.writeAttribute("password", m_password);
    xml.writeStartElement("contacts");
    foreach (const QString &contact, m_contacts) {
        xml.writeStartElement("contact");
        xml.writeAttribute("username", contact);
        xml.writeEndElement();
    }
    xml.writeEndElement();
    xml.writeEndElement();
}
