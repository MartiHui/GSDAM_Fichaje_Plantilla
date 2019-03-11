#include <QDebug>
#include <QTime>

#include "databaseinterface.h"
#include "eangenerator.h"

DatabaseInterface *DatabaseInterface::m_instance = NULL;

DatabaseInterface* DatabaseInterface::getInstance() {
    if (!m_instance) {
        m_instance = new DatabaseInterface();
    }

    return m_instance;
}

void DatabaseInterface::close() {
    m_database.close();
    m_instance = NULL;
}

DatabaseInterface::DatabaseInterface() {
    m_database = QSqlDatabase::addDatabase("QPSQL");

    m_database.setHostName(HOST_NAME);
    m_database.setPort(PORT);
    m_database.setDatabaseName(DATABASE_NAME);
    m_database.setUserName(USERNAME);
    m_database.setPassword(PASSWORD);

    if (!m_database.open()) {
        qDebug() << "Problemas al conectarse con la base de datos.";
        qDebug() << m_database.lastError();
    } else {
        qDebug() << "Conectado a la base de datos.";
    }

    qsrand(static_cast<uint>(QTime::currentTime()));
}

bool DatabaseInterface::doesUserExist(QString eanCode, QString password) {
    QSqlQuery query;
    query.prepare("SELECT empleado_id FROM empleados WHERE empleado_id = ? AND "
                  "empleado_password = ? AND empleado_activo IS TRUE");
    query.bindValue(0, eanCode);
    query.bindValue(1, password);
    query.exec();

    return query.next();
}

void DatabaseInterface::punchIoEmployee(QString eanCode, QString &response) {
    QSqlQuery query;
    query.prepare("SELECT fichaje_es_entrada, empleado_nombre, empleado_apellido FROM fichajes "
                  "INNER JOIN empleados ON (fichajes.empleado_id = empleados.empleado_id) "
                  "WHERE fichajes.empleado_id = ? ORDER BY fichaje_fecha DESC LIMIT 1");
    query.bindValue(0, eanCode);
    query.exec();

    query.prepare("INSERT INTO fichajes(empleado_id, fichaje_es_entrada) "
                  "VALUES(?, ?)");
    query.bindValue(0, eanCode);
    if (query.next() && query.value("fichaje_es_entrada").toBool()) {
        query.bindValue(1, false);
        response = QString("Has entrado correctamente. Bienvenido %1 %2")
                .arg(query.value("empleado_nombre").toString())
                .arg(query.value("empleado_apellido").toString());
    } else {
        query.bindValue(1, true);
        response = QString("Has salido correctamente. Hasta luego %1 %2")
                .arg(query.value("empleado_nombre").toString())
                .arg(query.value("empleado_apellido").toString());
    }

    query.exec();
}

void DatabaseInterface::getRegistrosInfo(QVector<ActionJson::Registro> &registros) {
    QSqlQuery query;
    query.prepare("SELECT empleado_id, empleado_nombre, empleado_apellido, fichaje_es_entrada, fichaje_fecha "
                  "FROM fichajes INNER JOIN empleados ON (fichajes.empleado_id = empleados.empleado_id) "
                  "ORDER BY fichaje_fecha DESC");
    query.exec();

    while (query.next()) {
        registros.push_back(ActionJson::Registro{query.value("empleado_id").toString(),
                            query.value("empleado_nombre").toString(),
                            query.value("empleado_apellido").toString(),
                            query.value("fichaje_es_entrada").toBool(),
                            query.value("fichaje_fecha").toDateTime().toString()});
    }
}

bool DatabaseInterface::doesAdminExist(QString username, QString password) {
    QSqlQuery query;
    query.prepare("SELECT admin_id FROM admins WHERE admin_user = ? AND "
                  "admin_password = ? AND admin_activo IS TRUE LIMIT 1");
    query.bindValue(0, username);
    query.bindValue(1, password);
    query.exec();

    return query.next();
}

void DatabaseInterface::getEmpleadosInfo(QVector<QMap<QString, QString> > &empleados) {
    QSqlQuery query;
    query.prepare("SELECT empleado_id, empleado_nombre, empleado_apellido FROM empleados "
                  "WHERE empleado_activo IS TRUE");
    query.exec();

    while (query.next()) {
        QMap<QString, QString> empleado;
        empleado["id"] = query.value("empleado_id").toString();
        empleado["nombre"] = query.value("empleado_nombre").toString();
        empleado["apellido"] = query.value("empleado_apellido").toString();

        empleados.append(empleado;
    }
}

void DatabaseInterface::deleteEmpleado(QString eanCode) {
    QSqlQuery query;
    query.prepare("UPDATE empleados SET empleado_activo = FALSE WHERE empleado_id = ?");
    query.bindValue(0, eanCode);
    query.exec();
}

QPair<QString, QString> createEmpleado(QString nombre, QString apellido) {
    QSqlQuery query;

    query.prepare("SELECT COUNT(*) FROM empleados");
    query.exec();
    query.next();
    QString productCode = QString::number(query.value(0).toInt()+1);

    QString eanCode = EanGenerator::getEanCode(productCode);
    QString password = QString::number(qrand() % 9999);

    if (eanCode != "") {
        query.prepare("INSERT INTO empleados(empleado_id, empleado_password, "
                      "empleado_nombre, empleado_apellido) VALUES(?, ?, ?, ?)");
        query.bindValue(0, eanCode);
        query.bindValue(1, password);
        query.bindValue(2, nombre);
        query.bindValue(3, apellido);

        query.exec();
    }

    return QPair{eanCode, password};
}
