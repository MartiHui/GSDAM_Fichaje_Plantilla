#ifndef ACTIONJSON_H
#define ACTIONJSON_H

#include <QObject>
#include <QJsonDocument>

#include "connection.h"

// La clase ActionJson parsea un archivo JSON y actua de acuerdo a sus contenidos
class ActionJson : public QObject {
    Q_OBJECT

public:
    explicit ActionJson(QString json, Connection *connection=nullptr);

private:
    QJsonDocument m_json;
    Connection *m_connection{nullptr};

}

#endif // ACTIONJSON_H
