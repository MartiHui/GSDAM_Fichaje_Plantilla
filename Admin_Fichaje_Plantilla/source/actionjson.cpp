#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>

#include "actionjson.h"

ActionJson::ActionJson(QString json) {
    m_json = QJsonDocument::fromJson(json.toUtf8());
    setActionType();
}

void ActionJson::setActionType() {
    QJsonValue action = m_json.object()["action"];

    if (action != QJsonValue::Undefined) {
        QString actionStr = action.toString();
        if (actionStr == "CONNECTION") {
            m_actionType = ActionType::CONNECTION;
        } else if (actionStr == "REGISTROS_INFO") {
            m_actionType = ActionType::REGISTROS_INFO;
        } else if (actionStr == "UPDATE") {
            m_actionType = ActionType::UPDATE;
        } else if (actionStr == "EMPLEADOS_INFO") {
            m_actionType = ActionType::EMPLEADOS_INFO;
        } else if (actionStr == "NEW_EMPLEADO") {
            m_actionType = ActionType::NEW_EMPLEADO;
        }
    } else {
        m_actionType = ActionType::INVALID;
    }
}

ActionType ActionJson::getActionType() {
    return m_actionType;
}

bool ActionJson::getRegistrosInfo(QVector<Registro> *registros, QVector<Registro> *historial) {
    bool validJson{true};

    if (m_json.object()["registros"].isArray()) {
        QJsonArray registrosArray = m_json.object()["registros"].toArray();

        for (int i = 0; i < registrosArray.count(); i++) {
            // Si esta posicion tiene un Objeto
            if (registrosArray.at(i).type() == QJsonValue::Object) {
                QJsonObject registro = registrosArray.at(i).toObject();

                // Si tiene todos los campos que esperamos
                if (registro["empleado_id"] != QJsonValue::Undefined &&
                        registro["fecha_entrada"] != QJsonValue::Undefined &&
                        registro["fecha_salida"] != QJsonValue::Undefined) {
                    Registro r{registro["empleado_id"].toString(),
                              registro["fecha_entrada"].toString(),
                              registro["fecha_salida"].toString()};

                    if (r.fecha_salida == "") {
                        registros->push_back(r);
                    } else {
                        historial->push_back(r);
                    }
                } else {
                    validJson = false;
                    break;
                }
            } else {
                validJson = false;
                break;
            }
        }
    } else {
        validJson = false;
    }

    return validJson;
}

bool ActionJson::getEmpleadosInfo(QVector<QString> *empleados) {
    bool validJson{true};

    if (m_json.object()["empleados"].isArray()) {
        QJsonArray empleadosArray = m_json.object()["empleados"].toArray();

        for (int i = 0; i < empleadosArray.count(); i++) {
            if (empleadosArray.at(i).type() == QJsonValue::String) {
                empleados->append(empleadosArray.at(i).toString());
            } else {
                validJson = false;
                break;
            }
        }
    } else {
        validJson = false;
    }

    return validJson;
}

bool ActionJson::connectAdminSuccessful() {
    QJsonValue value = m_json.object()["result"];
    bool result{false};

    if (value.type() == QJsonValue::Bool) {
        result = value.toBool();
    }

    return result;
}

QString ActionJson::askRegistrosInfo() {
    QJsonObject ask;

    ask.insert("action", QJsonValue("REGISTROS_INFO"));

    return QJsonDocument(ask).toJson();
}

QString ActionJson::connectAdmin(QString name, QString password) {
    QJsonObject ask;

    ask.insert("action", QJsonValue("ADMIN_CONNECT"));
    ask.insert("username", QJsonValue(name));
    ask.insert("password", QJsonValue(password));

    return QJsonDocument(ask).toJson();
}

QString ActionJson::askEmpleadosInfo() {
    QJsonObject ask;

    ask.insert("action", QJsonValue("EMPLEADOS_INFO"));

    return QJsonDocument(ask).toJson();
}

QString ActionJson::newEmpleado(QString nombre, QString apellido) {
    QJsonObject ask;

    ask.insert("action", QJsonValue("NEW_EMPLEADO"));
    ask.insert("nombre", QJsonValue(nombre));
    ask.insert("apellido", QJsonValue(apellido));

    return QJsonDocument(ask).toJson();
}

QString ActionJson::deleteEmpleado(QString empleadoId) {
    QJsonObject ask;

    ask.insert("action", QJsonValue("DELETE_EMPLEADO"));
    ask.insert("data", QJsonValue(empleadoId));

    return QJsonDocument(ask).toJson();
}

QPair<QString, QString> ActionJson::getNewEmpleadoData() {
    QPair<QString, QString> data;
    data.first = m_json.object()["eanCode"].toString();
    data.second = m_json.object()["password"].toString();
    return data;
}
