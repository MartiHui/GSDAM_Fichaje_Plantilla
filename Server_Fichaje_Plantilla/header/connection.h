#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QJsonObject>

class QWebSocket;

// La clase Connection almacena la información de las conexiones recibidas por el servidor
class Connection : public QObject {
    Q_OBJECT

public:
    explicit Connection(QWebSocket *webSocket);
    ~Connection();
    QWebSocket* getWebSocket();
    void sendJson(const QJsonObject &json);

    bool m_isAdmin{false};

private:
    QWebSocket *m_webSocket;

signals:
    void textMessageReceived(const QString &message);
    void disconnected();

private slots:
    void onMessageReceived(const QString &message);
    void onDisconnect();
};

#endif // CONNECTION_H
