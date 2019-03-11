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
    EMPLEADOS_INFO,
    UPDATE,
    NEW_EMPLEADO,
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
    bool getRegistrosInfo(QVector<Registro> *registros);
    bool getEmpleadosInfo(QVector<QString> *empleados);
    bool connectAdminSuccessful();
    ActionType getActionType();
    QPair<QString, QString> getNewEmpleadoData();

    static QString askRegistrosInfo();
    static QString connectAdmin(QString name, QString password);
    static QString askEmpleadosInfo();
    static QString newEmpleado(QString nombre, QString apellido);
    static QString deleteEmpleado(QString empleadoId);

private:
    QJsonDocument m_json;
    ActionType m_actionType;

    void setActionType();
};

#endif // ACTIONJSON_H
