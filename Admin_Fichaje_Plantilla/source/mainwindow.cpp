#include <QDebug>
#include <QTimer>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "actionjson.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setEnabled(false);

    m_connection = new ServerConnection("ws://localhost:1234");
    connect(m_connection, SIGNAL(messageReceived(QString)),
            this, SLOT(messageReceived(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fillRegistro(QVector<Registro> &registros) {
    ui->registrosView->clearContents();
    int size = registros.count();

    ui->registrosView->setRowCount(size);
    for (int row = 0; row < size; row++) {
        ui->registrosView->setItem(row, 0, new QTableWidgetItem(registros[row].id));
        ui->registrosView->setItem(row, 1, new QTableWidgetItem(registros[row].fecha_entrada));
    }
}

void MainWindow::fillHistorial(QVector<Registro> &registros) {
    ui->historialView->clearContents();
    int size = registros.count();

    ui->historialView->setRowCount(size);
    for (int row = 0; row < size; row++) {
        ui->historialView->setItem(row, 0, new QTableWidgetItem(registros[row].id));
        ui->historialView->setItem(row, 1, new QTableWidgetItem(registros[row].fecha_entrada));
        ui->historialView->setItem(row, 2, new QTableWidgetItem(registros[row].fecha_salida));
    }
}

void MainWindow::fillEmpleados(QVector<QMap<QString, QString> > &empleados) {
    ui->employeeList->clearContents();

    int size = empleados.count();
    ui->employeeList->setRowCount(size);
    for (int i = 0; i < size; i++) {
        ui->employeeList->setItem(i, 0, new QTableWidgetItem(empleados[i]["id"]));
        ui->employeeList->setItem(i, 1, new QTableWidgetItem(empleados[i]["nombre"]));
        ui->employeeList->setItem(i, 2, new QTableWidgetItem(empleados[i]["apellido"]));
    }
}

void MainWindow::messageReceived(QString message) {
    ActionJson action{message};
    QMessageBox msg;

    switch (action.getActionType()) {
    case ActionType::INVALID:
        msg.setText("Ha habido algún problema");
        msg.exec();
        break;

    case ActionType::CONNECTION:
        if (action.connectAdminSuccessful()) {
            ui->tabWidget->setEnabled(true);
            ui->pushButton->setEnabled(true);
            updateRegistros();
            updateEmpleados();
        } else {
            msg.setText("Credenciales incorrectas");
            msg.exec();
        }
        break;

    case ActionType::REGISTROS_INFO:
    {
        QVector<Registro> registrosAbiertos;
        QVector<Registro> registroHistorial;

        if (action.getRegistrosInfo(&registrosAbiertos, &registroHistorial)) {
            fillRegistro(registrosAbiertos);
            fillHistorial(registroHistorial);
        }

        break;
    }

    case ActionType::UPDATE:
        updateRegistros();
        updateEmpleados();
        break;

    case ActionType::EMPLEADOS_INFO:
    {
        QVector<QMap<QString, QString> > empleados;

        if (action.getEmpleadosInfo(&empleados)) {
            fillEmpleados(empleados);
        }

        break;
    }

    case ActionType::NEW_EMPLEADO:
        updateEmpleados();
        newEmpleado(action.getNewEmpleadoData());
    }
}

void MainWindow::updateRegistros() {
    m_connection->sendMessage(ActionJson::askRegistrosInfo());
}

void MainWindow::updateEmpleados() {
    m_connection->sendMessage(ActionJson::askEmpleadosInfo());
}

void MainWindow::on_pushButton_clicked() {
    QString username = ui->username->text();
    ui->username->setText("");
    QString password = ui->userpassword->text();
    ui->userpassword->setText("");

    m_connection->sendMessage(ActionJson::connectAdmin(username, password));

    ui->pushButton->setEnabled(false);
}

void MainWindow::on_removeEmployee_clicked() {
    if (ui->employeeList->currentItem()) {
        QString id = ui->employeeList->item
                (ui->employeeList->currentItem()->row(), 0)->text();
        m_connection->sendMessage(ActionJson::deleteEmpleado(id);
    }
}

void MainWindow::on_createEmployee_clicked() {
    if (ui->empleadoNombre->text() != "" && ui->empleadoApellido->text() != "") {
        m_connection->sendMessage(ActionJson::newEmpleado(ui->empleadoNombre->text(),
                                                          ui->empleadoApellido->text()));
        ui->empleadoNombre->setText("");
        ui->empleadoApellido->setText("");
        ui->createEmployee->setEnabled(false);
    } else {
        QMessageBox msg;
        msg.setText("Introduce nombre y apellido para crear un nuevo usuario");
    }
}

void MainWindow::newEmpleado(QPair<QString, QString> empleadoData) {
    ui->createEmployee->setEnabled(true);
    QMessageBox msg;
    msg.setText("Nuevo empleado.\nID: " + empleadoData.first +
                "\nContraseña: " + empleadoData.second);
    msg.exec();
}
