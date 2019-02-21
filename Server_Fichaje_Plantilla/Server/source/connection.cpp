#include <QWebSocket>

#include "connection.h"

Connection::Connection(QWebSocket *webSocket) : m_webSocket{webSocket} {

}

Connection::~Connection() {
    delete m_webSocket;
}

QWebSocket* Connection::getWebSocket() {
    return m_webSocket;
}
