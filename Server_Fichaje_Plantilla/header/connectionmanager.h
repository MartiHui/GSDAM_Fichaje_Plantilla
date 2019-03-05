#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>
#include <QList>

class QWebSocketServer;
class Connection;

// La clase ConnectionManager administra las conexiones que recibe el servidor
// mediante protocolo WebSocket, y responde a las peticiones que reciba de dichas
// conexiones. Toda el intercambio de mensajes se realizará mediante archibos JSON.
class ConnectionManager : public QObject {
    Q_OBJECT

public:
    // Devuelve un puntero a la única instancia activa posible de la clase
    static ConnectionManager* getInstance(quint16 port=0);
    void sendMessageToAdmins(const QJsonDocument &message);

private:
    explicit ConnectionManager(quint16 port);
    ~ConnectionManager();

    static ConnectionManager* m_instance; // La única instancia de la clase
    quint16 m_port; // Puerto en el que inicializar el servidor
    QWebSocketServer *m_webSocketServer;
    QList<Connection *> m_connections; // Lista con todas las conexiones activas

private slots:
    void startServer();
    void webSocketConnected();
    void webSocketDisconnected();
    // Analiza el mensaje recibido de una conexion y actua de forma acorde
    void processMessage(const QString &message);
    void close();
};

#endif // CONNECTIONMANAGER_H
