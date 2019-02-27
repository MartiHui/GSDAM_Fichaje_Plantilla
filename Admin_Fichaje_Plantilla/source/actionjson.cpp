#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

#include "actionjson.h"

ActionJson::ActionJson(QString json) {
    m_json = QJsonDocument::fromJson(json.toUtf8());
    setActionType();
}

void ActionJson::setActionType() {
    QJsonValue action = m_json["action"];

    if (action != QJsonValue::Undefined) {
        QString actionStr = action.toString();
        if (actionStr == "CONNECTION") {
            m_actionType = ActionType::CONNECTION;
        } else if (actionStr == "INFO") {
            m_actionType = ActionType::INFO;
        }
    } else {
        m_actionType = ActionType::INVALID;
    }
}

bool ActionJson::getRegistrosInfo(QVector<Registro> *registros, QVector<Registro> *historial) {
    bool validJson{true};

    if (m_json.isArray()) {
        QJsonArray registrosArray = m_json.array();

        for (int i = 0; i < registrosArray.count(); i++) {
            // Si esta posicion tiene un Objeto
            if (registrosArray.at(i).type() == QVariantMap) {
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

QString ActionJson::askRegistrosInfo() {
    QJsonObject ask;
    ask.insert("action", QJsonValue("REGISTROS_INFO"));
    return QJsonDocument(ask).toJson();
}
