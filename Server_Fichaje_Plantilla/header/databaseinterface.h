#ifndef DATABASEINTERFACE_H
#define DATABASEINTERFACE_H

#include <QObject>
#include <QtSql>

// La clase DatabaseInterface sirve para conectarse a la base de datos y realizar
// todas las consultas necesarias
class DatabaseInterface : public QObject {
    Q_OBJECT

public:
    // Devuelve un puntero a la única instancia activa posible de la clase
    static DatabaseInterface* getInstance();
    void close();

    // Comprueba si la combinacion de identificacion y contraseña existen
    // en la base de datos.
    bool doesUserExist(QString eanCode, QString password);
    void punchIoEmployee(QString eanCode);
    void getRegistrosInfo(QVector<QPair<QString, QPair<QString, QString> > > &registros);
    bool doesAdminExist(QString username, QString password);
    void getEmpleadosInfo(QVector<QString> &empleados);

private:
    explicit DatabaseInterface();

    static DatabaseInterface* m_instance;
    QSqlDatabase m_database;

    // Información relacionada con la conexión a la base de datos
    QString const HOST_NAME{"127.0.0.1"};
    int const PORT{5432};
    QString const DATABASE_NAME{"Fichaje_Plantilla"};
    QString const USERNAME{"usuario"};
    QString const PASSWORD{"usuario"};
};

#endif // DATABASEINTERFACE_H
