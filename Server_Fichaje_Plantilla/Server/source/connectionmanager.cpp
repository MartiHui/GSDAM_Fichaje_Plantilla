#include <QWebSocketServer>
#include <QTimer>

#include "connectionmanager.h"

ConnectionManager *ConnectionManager::m_instance = NULL;

ConnectionManager* ConnectionManager::getInstance(quint16 port) {
    if (!m_instance) {
        m_instance = new ConnectionManager(port);
    }

    return m_instance;
}

ConnectionManager::ConnectionManager(quint16 port) : m_port(port) {
    QTimer::singleShot(0, this, SLOT(startServer()));
}

ConnectionManager::~ConnectionManager() {
    m_webSocketServer->close();
    qDeleteAll(m_connections.begin(), m_connections.end());
}

void ConnectionManager::startServer() {
    m_webSocketServer = new QWebSocketServer(QStringLiteral("Servidor de Fichaje de Empleados"),
                                             QWebSocketServer::NonSecureMode, this);

    if (m_webSocketServer->listen(QHostAddress::Any, m_port)) {
        qDebug() << "Servidor iniciado. Puerto: " << m_port;
    } else {
        qDebug() << "Error al iniciar el servidor. Puerto: " << m_port;
    }
}
