#ifndef QCCNAMESPACE_H
#define QCCNAMESPACE_H

namespace Qcc
{
    enum MessageType {
        ConnectionAccepted,
        ConnectionRefused,
        UserRegister,          // QString(username), QString(passwort|SHA-1), QString(name), QString(email)
        RegisterSuccess,
        RegisterFailure,       // QString(reason)
        UserAuthentication,    // QString(username), QString(passwort|SHA-1)
        AuthenticationSuccess,
        AuthenticationFailure, // QString(reason)
        RequestAuthorization,  // QString(username)
        AuthorizationAccepted, // QString(username), qint32(status)
        AuthorizationRejected, // QString(username)
        RequestContactList,
        ContactList,           // QString(username), qint32(status), ...
        RemoveContact,         // QString(username)
        ContactRemoved,        // QString(username)
        Message,               // QString(receiver), QString(message), message-id?
        MessageSuccess,        // message-id?
        MessageFailure,        // message-id?
        IllegalMessage = -1
    };
}

#endif // QCCNAMESPACE_H
