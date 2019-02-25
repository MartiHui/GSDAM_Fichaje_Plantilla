#ifndef EMPLOYEEINFO_H
#define EMPLOYEEINFO_H

#include <QObject>

// La clase EmployeeInfo almacena la información del empleado: un código EAN13 que
// le identifica en la base de datos y una contraseña. La clase también tiene la
// funcionalidad de validar el código EAN13 y transformar los datos en un JSON
class EmployeeInfo : public QObject {
  Q_OBJECT

public:
    EmployeeInfo(QString eanCode, QString password);
    bool isValid(QString &reason); // Valida si m_eanCode es un EAN13 válido
    QString toJson(); // Devuelve un JSON con el EAN13 y contraseña del empleado

private:
    QString m_eanCode; // El codigo EAN13 que identifica al empleado
    QString m_password;

    // Calcula cual deberia ser el digito de validacion siguiendo el procedimiento
    // indicado por la INternational ISBN Agency
    // https://en.wikipedia.org/wiki/International_Standard_Book_Number#ISBN-13_check_digit_calculation
    QString calculateCheckSumDigit();
};

#endif // EMPLOYEEINFO_H
