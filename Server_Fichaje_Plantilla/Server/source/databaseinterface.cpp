#include <QDebug>

#include "databaseinterface.h"

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
    }
}

