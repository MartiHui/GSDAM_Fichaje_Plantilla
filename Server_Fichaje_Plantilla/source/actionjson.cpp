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
        if (m_json["data"] != QJsonValue::Undefined) {
            QString eanCode = DatabaseInterface::getInstance()->createEmpleado(m_json["data"].toString());
            if (eanCode != "") {
                QJsonObject newData;
                newData.insert("action", QJsonValue("NEW_EMPLEADO"));
                newData.insert("eanCode", QJsonValue(eanCode));
                newData.insert("password", QJsonValue(m_json["data"].toString()));
                m_connection->sendJson(QJsonDocument{newData});
            } else {
                sendRequestSuccess(false);
            }
        } else {
            sendRequestSuccess(false);
        }
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

    json.insert("isRequestSuccesful", QJsonValue(isSuccesful));

    m_connection->sendJson(QJsonDocument{json});
}

void ActionJson::punchIoEmployee() {
    bool requestResult{false};
    QString eanCode = m_json["empleado_id"].toString();
    QString password = m_json["empleado_password"].toString();

    if (eanCode != QJsonValue::Undefined && password != QJsonValue::Undefined) {
        if (DatabaseInterface::getInstance()->doesUserExist(eanCode, password)) {
            requestResult = true;
            DatabaseInterface::getInstance()->punchIoEmployee(eanCode);
        }
    }

    if (requestResult) {
        QJsonObject update;
        update.insert("action", QJsonValue("UPDATE"));
        ConnectionManager::getInstance()->sendMessageToAdmins(QJsonDocument{update});
    }

    sendRequestSuccess(requestResult);
}

void ActionJson::sendRegistrosInfo() {
    QVector<QPair<QString, QPair<QString, QString> > > registros;
    DatabaseInterface::getInstance()->getRegistrosInfo(registros);

    QJsonObject json;
    json.insert("action", QJsonValue("REGISTROS_INFO"));

    QJsonArray registrosJson;
    for (auto registro : registros) {
        QJsonObject object;

        object.insert("empleado_id", QJsonValue(registro.first));
        object.insert("fecha_entrada", QJsonValue(registro.second.first));
        object.insert("fecha_salida", QJsonValue(registro.second.second));

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
    QVector<QString> empleados;
    DatabaseInterface::getInstance()->getEmpleadosInfo(empleados);

    QJsonObject json;
    json.insert("action", QJsonValue("EMPLEADOS_INFO"));

    QJsonArray empleadosJson;
    for (QString empleado : empleados) {
        empleadosJson.append(QJsonValue(empleado));
    }

    json.insert("empleados", QJsonValue(empleadosJson));

    m_connection->sendJson(QJsonDocument{json});
}
