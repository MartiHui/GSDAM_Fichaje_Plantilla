#include "employeeinfo.h"

EmployeeInfo::EmployeeInfo(QString eanCode, QString password) :
    m_eanCode{eanCode}, m_password{password}
{
}

bool EmployeeInfo::isValid() {

}

QString EmployeeInfo::toJson() {

}
