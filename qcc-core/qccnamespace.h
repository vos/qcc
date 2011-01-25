#ifndef QCCNAMESPACE_H
#define QCCNAMESPACE_H

namespace Qcc
{
    enum MessageType {
        ConnectionAccepted,
        ConnectionRefused,
        UserAuthentication,
        AuthenticationSuccess,
        AuthenticationFailure,
        Message,
        MessageSuccess,
        MessageFailure,
        IllegalMessage = -1
    };
}

#endif // QCCNAMESPACE_H
