#ifndef ACTIONJSON_H
#define ACTIONJSON_H

#include <QObject>
#include <QJsonDocument>
#include <QVector>

#include "datastructs.h"

enum class ActionType {
    INVALID,
    CONNECTION,
    REGISTROS_INFO,
};

// La clase ActionJson sirve tanto para crear archivos JSON que enviar al servidor
// como para parsear los archivos JSON recibidos del servidor y actuar de acuerdo
// a su contenido
class ActionJson : public QObject {
    Q_OBJECT

public:
    explicit ActionJson(QString json);
    // Obtiene todos los objetos de tipo Registro del JSON y los introduce en el
    // vector registros si no tienen fecha de salida, de lo contrario, en el vector
    // historial. La función devuelve true si el JSON es válido
    bool getRegistrosInfo(QVector<Registro> *registros, QVector<Registro> *historial);
    static QString askRegistrosInfo();
    ActionType getActionType();

private:
    QJsonDocument m_json;
    ActionType m_actionType;

    void setActionType();
};

#endif // ACTIONJSON_H
