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
                  "empleado_password = ? AND empleado_is_active IS TRUE");
    query.bindValue(0, eanCode);
    query.bindValue(1, password);
    query.exec();

    return query.next();
}

void DatabaseInterface::punchIoEmployee(QString eanCode) {
    QSqlQuery query;
    query.prepare("SELECT fichaje_id, fecha_salida FROM fichajes WHERE empleado_id = ? "
                  "ORDER BY fecha_salida DESC LIMIT 1");
    query.bindValue(0, eanCode);
    query.exec();

    if (query.next() && query.value("fecha_salida").toDateTime().toString() == "") {
        QSqlQuery update;
        update.prepare("UPDATE fichajes SET fecha_salida = now() WHERE fichaje_id = ?");
        update.bindValue(0, query.value("fichaje_id").toInt());
        update.exec();
    } else {
        QSqlQuery insert;
        insert.prepare("INSERT INTO fichajes(empleado_id, fecha_entrada) VALUES(?, now())");
        insert.bindValue(0, eanCode);
        insert.exec();
    }
}

void DatabaseInterface::getRegistrosInfo(QVector<QPair<QString, QPair<QString, QString> > > &registros) {
    QSqlQuery query;
    query.prepare("SELECT empleado_id, fecha_entrada, fecha_salida FROM fichajes "
                  "ORDER BY fecha_salida DESC");
    query.exec();

    while (query.next()) {
        registros.append(qMakePair(query.value("empleado_id").toString(),
                                    qMakePair(query.value("fecha_entrada").toDateTime().toString(),
                                              query.value("fecha_salida").toDateTime().toString())));
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

void DatabaseInterface::getEmpleadosInfo(QVector<QString> &empleados) {
    QSqlQuery query;
    query.prepare("SELECT empleado_id FROM empleados WHERE empleado_is_active IS TRUE");
    query.exec();

    while (query.next()) {
        empleados.append(query.value("empleado_id").toString());
    }
}

void DatabaseInterface::deleteEmpleado(QString eanCode) {
    QSqlQuery query;
    query.prepare("UPDATE empleados SET empleado_is_active = FALSE WHERE empleado_id = ?");
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
