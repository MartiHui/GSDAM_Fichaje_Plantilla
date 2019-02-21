#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>

class QWebSocket;

// La clase Connection almacena la información de las conexiones recibidas por el servidor
class Connection : public QObject {
    Q_OBJECT
public:
    explicit Connection(QWebSocket *webSocket);
    ~Connection();

    QWebSocket* getWebSocket();

private:
    QWebSocket *m_webSocket;
};

#endif // CONNECTION_H
