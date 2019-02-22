#include <QWebSocketServer>
#include <QTimer>
#include <QWebSocket>

#include "connectionmanager.h"
#include "connection.h"
#include "databaseinterface.h"

ConnectionManager *ConnectionManager::m_instance = NULL;

ConnectionManager* ConnectionManager::getInstance(quint16 port) {
    if (!m_instance) {
        m_instance = new ConnectionManager(port);
    }

    return m_instance;
}

ConnectionManager::ConnectionManager(quint16 port) : m_port{port} {
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

        connect(m_webSocketServer, SIGNAL(newConnection()),
                this, SLOT(webSocketConnected()));
        connect(m_webSocketServer, SIGNAL(closed()),
                this, SLOT(close()));
    } else {
        qDebug() << "Error al iniciar el servidor. Puerto: " << m_port;
    }
}

void ConnectionManager::webSocketConnected() {
    Connection *connection = new Connection(m_webSocketServer->nextPendingConnection());

    qDebug() << "Conexion iniciada. IP: " <<
                connection->getWebSocket()->peerAddress().toString();

    connect(connection, SIGNAL(textMessageReceived(QString)),
            this, SLOT(processMessage(QString)));
    connect(connection, SIGNAL(disconnected()),
            this, SLOT(webSocketDisconnected()));

    m_connections << connection;
}

void ConnectionManager::webSocketDisconnected() {
    Connection *connection = qobject_cast<Connection *>(sender());

    if (connection) {
        qDebug() << "Conexión finalizada. IP: " <<
                    connection->getWebSocket()->peerAddress().toString();

        m_connections.removeAll(connection);
        connection->deleteLater();
    }
}

void ConnectionManager::processMessage(const QString &message) {
    Connection *connection = qobject_cast<Connection *>(sender());

    // TODO: programar respuestas
}

void ConnectionManager::close() {
    DatabaseInterface::getInstance()->close();
}
