#include <QWebSocket>
#include <QJsonDocument>
#include <QDebug>

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

void Connection::onMessageReceived(const QString &message) { qDebug() << "Recibido: " << message;
    emit textMessageReceived(message);
}

void Connection::onDisconnect() {
    emit disconnected();
}

void Connection::sendJson(const QJsonDocument &json) { qDebug() << "Enviado: " << json.toJson();
    m_webSocket->sendTextMessage(QString(json.toJson()));
}
