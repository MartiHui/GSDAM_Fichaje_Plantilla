#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>

#include "actionjson.h"
#include "databaseinterface.h"

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
        //if (m_connection->m_isAdmin) {
            sendRegistrosInfo();
        //} else {
         //   sendRequestSuccess(false);
        //}
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
