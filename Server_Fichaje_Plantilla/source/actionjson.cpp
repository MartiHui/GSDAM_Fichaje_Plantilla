#include <QJsonDocument>
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
    }
}

void ActionJson::sendRequestSuccess(bool isSuccesful) {
    QJsonObject json;

    json.insert("isRequestSuccesful", QJsonValue(isSuccesful));

    m_connection->sendJson(json);
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
