#include <QWebSocket>

#include "serverconnection.h"

ServerConnection::ServerConnection(QString serverUrl) {
    m_webSocket = new QWebSocket();
    connect(m_webSocket, SIGNAL(connected()), this, SLOT(onConnected()));
    m_webSocket->open(QUrl(url));
}

ServerConnection::~ServerConnection() {
    m_webSocket->close();
    delete m_webSocket;
}

void ServerConnection::onConnected() {
    connect(m_webSocket, SIGNAL(textMessageReceived(QString)),
            this, SLOT(onMessageReceived(QString)));
}

void ServerConnection::onMessageReceived(QString message) {
    emit messageReceived(message);
}

void ServerConnection::sendMessage(QString message) {
    m_webSocket->sendTextMessage(message);
}
