#include <QtCore/QCoreApplication>
#include <QStringList>
#include <QDebug>

#include "server.h"

static const quint16 DEFAULT_PORT = 12345;

// helper function for reading a command line argument of type quint16
quint16 readShort(const QStringList &arguments, const QString &name, quint16 defaultValue = -1)
{
    if (arguments.isEmpty())
        return defaultValue;

    int index = arguments.indexOf(name);
    if (index < 0 || index + 1 >= arguments.count())
        return defaultValue;

    bool ok;
    quint16 value = arguments.at(index + 1).toShort(&ok);
    return ok ? value : defaultValue;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QStringList arguments = QCoreApplication::arguments();
#ifdef DEBUG
    qDebug() << "arguments: " << arguments;
#endif

    QHostAddress adress = QHostAddress::Any;
    quint16 port = readShort(arguments, "-port", DEFAULT_PORT);

    Server server;
    if (server.listen(adress, port)) {
        qDebug("The server is listening on interface %s, port %i",
               qPrintable(server.serverAddress().toString()), server.serverPort());
    } else {
        qCritical("Failed to start the server on interface %s, port %i: %s (error code %i)",
               qPrintable(adress.toString()), port, qPrintable(server.errorString()), server.serverError());
        return 1;
    }

    return app.exec();
}
