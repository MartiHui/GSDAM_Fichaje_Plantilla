#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>

#include "employeeinfo.h"

EmployeeInfo::EmployeeInfo(QString eanCode, QString password) :
    m_eanCode{eanCode}, m_password{password}
{
}

bool EmployeeInfo::isValid(QString &reason) {
    bool valid{true};

    // EAN13 debe tener 13 caracteres
    if (m_eanCode.length() != 13) {
        reason = "El código debe tener 13 dígitos";
        valid = false;
    }

    // EAN13 debe ser todo números
    // No se puede pasar todo el QString a un int ya que un numero de 13 digitos es
    // demasiado grande
    for (int i = 0; i < m_eanCode.length(); i++) {
        if (!m_eanCode.at(i).isDigit()) {
            reason = "El código debe ser un número";
            valid = false;
        }
    }

    // El último dígito del EAN13 es un digito de validacion
    if (m_eanCode.at(m_eanCode.length()-1) != calculateCheckSumDigit()) {
        reason = "El dígito de validación no es correcto";
        valid = false;
    }

    return valid;
}

QString EmployeeInfo::toJson() {
    QJsonObject json;

    json.insert("action", QJsonValue("FICHAR"));
    json.insert("empleado_id", QJsonValue(m_eanCode));
    json.insert("empleado_password", QJsonValue(m_password));

    return QString(QJsonDocument(json).toJson(QJsonDocument::JsonFormat::Compact));
}

QString EmployeeInfo::calculateCheckSumDigit() {
    int sum{0};

    for (int i = 0; i < m_eanCode.length()-1; i++) {
        // No se usa el método at() para ir obteniendo los caracteres porque devuleve un QChar,
        // y esta clase no tiene forma de pasar a dígito
        sum += m_eanCode.mid(i, 1).toInt() * ((i%2 == 0) ? 1 : 3);
    }

    // No utilizamos el módulo porque en caso de que el resto fuera 0, el metodo devolveria 10,
    // lo cual seria incorrecto, ya que deberia devolver 0
    return QString::number(10*((sum/10)+1) - sum);
}
