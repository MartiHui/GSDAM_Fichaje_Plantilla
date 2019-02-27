#ifndef ACTIONJSON_H
#define ACTIONJSON_H

#include <QObject>
#include <QJsonObject>

#include "connection.h"

enum class ActionType {
    INVALID,
    ADMIN_CONNECT, // Comprueba las credenciales de administrador
    PUNCH_IO, // Fichar empleado
    ACTU_EMPLEADO, // Cambiar informacion de empleado
};

// La clase ActionJson parsea un archivo JSON y actua de acuerdo a sus contenidos
class ActionJson : public QObject {
    Q_OBJECT

public:
    explicit ActionJson(QString json, Connection *connection);

private:
    QJsonObject m_json;
    Connection *m_connection;
    ActionType m_actionType;

    void setActionType();
    void processRequest(); // Actua según el ActionType del documento
    void sendRequestSuccess(bool isSuccesful);
    void punchIoEmployee();
};

#endif // ACTIONJSON_H