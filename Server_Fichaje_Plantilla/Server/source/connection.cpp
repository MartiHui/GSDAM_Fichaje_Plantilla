#include <QWebSocket>

#include "connection.h"

Connection::Connection(QWebSocket *webSocket) : m_webSocket{webSocket} {
    connect(m_webSocket, SIGNAL(textMessageReceived(QString)),
            this, SLOT(onMessageReceived(QString)));
    connect(m_webSocket, SIGNAL(disconnected()),
            this, SLOT(onDisconnect()));
}

Connection::~Connection() {
    delete m_webSocket;
}

QWebSocket* Connection::getWebSocket() {
    return m_webSocket;
}

void Connection::onMessageReceived(const QString &message) {
    emit textMessageReceived(message);
}

void Connection::onDisconnect() {
    emit disconnected();
}
