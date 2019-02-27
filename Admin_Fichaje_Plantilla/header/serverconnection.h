#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

#include <QObject>

class QWebSocket;

// La clase ServerConnection establece una conexión con el servidor
// indicado a traves de WebSocket. También gestiona el intercambio de
// mensajes con el servidor.
class ServerConnection : public QObject {
    Q_OBJECT

public:
    explicit ServerConnection(QString serverUrl);
    ~ServerConnection();
    void sendMessage(QString message);
    bool isValid();

private:
    QWebSocket *m_webSocket;

signals:
    void messageReceived(QString message);

private slots:
    void onConnected();
    void onMessageReceived(QString message);

};

#endif // SERVERCONNECTION_H
