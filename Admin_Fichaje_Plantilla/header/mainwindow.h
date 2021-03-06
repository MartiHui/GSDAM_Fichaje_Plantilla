#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

#include "datastructs.h"
#include "serverconnection.h"

class QListWidgetItem;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    ServerConnection *m_connection;
    Ui::MainWindow *ui;

    void setTablesHeader();
    void fillRegistros(QVector<Registro> &registros);
    void fillEmpleados(QVector<QMap<QString, QString> > &empleados);
    void updateRegistros();
    void updateEmpleados();
    void newEmpleado(QPair<QString, QString> empleadoData);

private slots:
    void messageReceived(QString message);

    void on_pushButton_clicked();
    void on_removeEmployee_clicked();
    void on_createEmployee_clicked();
};

#endif // MAINWINDOW_H
