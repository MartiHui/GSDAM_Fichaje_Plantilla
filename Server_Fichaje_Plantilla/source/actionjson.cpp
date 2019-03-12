#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>

#include "actionjson.h"
#include "databaseinterface.h"
#include "connectionmanager.h"

ActionJson::ActionJson(QString json, Connection *connection) :
        m_connection{connection} {
    m_json = QJsonDocument::fromJson(json.toUtf8()).object();
    setActionType();
    processRequest();
}

void ActionJson::setActionType() {
    QJsonValue action = m_json["action"];

    if (action != QJsonValue::Undefined) {
        QString actionStr = action.toString();
        if (actionStr == "FICHAR") {
            m_actionType = ActionType::PUNCH_IO;
        } else if (actionStr == "REGISTROS_INFO") {
            m_actionType = ActionType::REGISTROS_INFO;
        } else if (actionStr == "ADMIN_CONNECT") {
            m_actionType = ActionType::ADMIN_CONNECT;
        } else if (actionStr == "EMPLEADOS_INFO") {
            m_actionType = ActionType::EMPLEADOS_INFO;
        } else if (actionStr == "NEW_EMPLEADO") {
            m_actionType = ActionType::NEW_EMPLEADO;
        } else if (actionStr == "DELETE_EMPLEADO") {
            m_actionType = ActionType::DELETE_EMPLEADO;
        }
    } else {
        m_actionType = ActionType::INVALID;
    }
}

void ActionJson::processRequest() {
    switch (m_actionType) {
    case ActionType::INVALID:
        sendRequestSuccess(false);
        break;

    case ActionType::PUNCH_IO:
        punchIoEmployee();
        break;

    case ActionType::REGISTROS_INFO:
        if (m_connection->m_isAdmin) {
            sendRegistrosInfo();
        } else {
            sendRequestSuccess(false);
        }
        break;

    case ActionType::ADMIN_CONNECT:
        checkAdminCredentials();
        break;

    case ActionType::EMPLEADOS_INFO:
        if (m_connection->m_isAdmin) {
            sendEmpleadosInfo();
        } else {
            sendRequestSuccess(false);
        }
        break;

    case ActionType::NEW_EMPLEADO:
        createNewEmpleado();
        break;

    case ActionType::DELETE_EMPLEADO:
        if (m_json["data"] != QJsonValue::Undefined) {
            DatabaseInterface::getInstance()->deleteEmpleado(m_json["data"].toString());
            QJsonObject update;
            update.insert("action", QJsonValue("UPDATE"));
            ConnectionManager::getInstance()->sendMessageToAdmins(QJsonDocument{update});
        } else {
            sendRequestSuccess(false);
        }
        break;
    }
}

void ActionJson::sendRequestSuccess(bool isSuccesful) {
    QJsonObject json;

    json.insert("requestSuccessful", QJsonValue(isSuccesful));

    m_connection->sendJson(QJsonDocument{json});
}

void ActionJson::sendRequestSuccess(bool isSuccesful, QString msg) {
    QJsonObject json;

    json.insert("requestSuccessful", QJsonValue(isSuccesful));
    json.insert("message", QJsonValue(msg));

    m_connection->sendJson(QJsonDocument{json});
}

void ActionJson::punchIoEmployee() {
    bool requestResult{false};
    QString msg{""};

    QString eanCode = m_json["empleado_id"].toString();
    QString password = m_json["empleado_password"].toString();

    if (eanCode != QJsonValue::Undefined && password != QJsonValue::Undefined) {
        if (DatabaseInterface::getInstance()->doesUserExist(eanCode, password)) {
            requestResult = true;
            DatabaseInterface::getInstance()->punchIoEmployee(eanCode, msg);
        } else {
            msg = "Usuario o contraseña incorrectas.";
        }
    } else {
        msg = "Ha habido algún problema con el servidor.";
    }

    if (requestResult) {
        QJsonObject update;
        update.insert("action", QJsonValue("UPDATE"));
        ConnectionManager::getInstance()->sendMessageToAdmins(QJsonDocument{update});
    }

    sendRequestSuccess(requestResult, msg);
}

void ActionJson::sendRegistrosInfo() {
    QVector<Registro> registros;
    DatabaseInterface::getInstance()->getRegistrosInfo(registros);

    QJsonObject json;
    json.insert("action", QJsonValue("REGISTROS_INFO"));

    QJsonArray registrosJson;
    for (Registro registro : registros) {
        QJsonObject object;

        object.insert("empleado_id", QJsonValue(registro.empleadoId));
        object.insert("empleado_nombre", QJsonValue(registro.empleadoNombre));
        object.insert("empleado_apellido", QJsonValue(registro.empleadoApellido));
        object.insert("es_entrada", QJsonValue(registro.esEntrada));
        object.insert("fecha", QJsonValue(registro.fecha));

        registrosJson.append(object);
    }

    json.insert("registros", QJsonValue(registrosJson));

    m_connection->sendJson(QJsonDocument{json});
}

void ActionJson::checkAdminCredentials() {
    bool success{false};

    if (m_json["username"] != QJsonValue::Undefined &&
            m_json["password"] != QJsonValue::Undefined) {
        success = DatabaseInterface::getInstance()->
                doesAdminExist(m_json["username"].toString(), m_json["password"].toString());
        if (success) m_connection->m_isAdmin = true;
    }


    QJsonObject answer;
    answer.insert("action", QJsonValue("CONNECTION"));
    answer.insert("result", QJsonValue(success));

    m_connection->sendJson(QJsonDocument{answer});
}

void ActionJson::sendEmpleadosInfo() {
    QVector<QMap<QString, QString> > empleados;
    DatabaseInterface::getInstance()->getEmpleadosInfo(empleados);

    QJsonObject json;
    json.insert("action", QJsonValue("EMPLEADOS_INFO"));

    QJsonArray empleadosJson;
    for (auto empleado : empleados) {
        QJsonObject empleadoJson;
        empleadoJson.insert("id", QJsonValue(empleado["id"]));
        empleadoJson.insert("nombre", QJsonValue(empleado["nombre"]));
        empleadoJson.insert("apellido", QJsonValue(empleado["apellido"]));

        empleadosJson.append(QJsonValue(empleadoJson));
    }

    json.insert("empleados", QJsonValue(empleadosJson));

    m_connection->sendJson(QJsonDocument{json});
}

void ActionJson::createNewEmpleado() {
    if (m_json["nombre"] != QJsonValue::Undefined &&
            m_json["apellido"] != QJsonValue::Undefined) {
        QPair<QString, QString> credenciales = DatabaseInterface::getInstance()->createEmpleado(m_json["nombre"].toString(), m_json["apellido"].toString());

        if (credenciales.first != "") {
            QJsonObject answer;

            answer.insert("action", QJsonValue("NEW_EMPLEADO"));
            answer.insert("eanCode", QJsonValue(credenciales.first));
            answer.insert("password", QJsonValue(credenciales.second));

            m_connection->sendJson(QJsonDocument{answer});
        } else {
            sendRequestSuccess(false);
        }
    } else {
        sendRequestSuccess(false);
    }
}
