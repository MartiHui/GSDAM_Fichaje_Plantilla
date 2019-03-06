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

void MainWindow::fillEmpleados(QVector<QString> &empleados) {
    ui->employeeList->clear();

    for (QString empleado : empleados) {
        ui->employeeList->addItem(empleado);
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
        QVector<QString> empleados;

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
        ui->removeEmployee->setText("Dar de baja");
        m_connection->sendMessage(ActionJson::deleteEmpleado(
                                      ui->employeeList->currentItem()->text()));
    }
}

void MainWindow::on_createEmployee_clicked() {
    if (ui->newPassword->text() != "") {
        m_connection->sendMessage(ActionJson::newEmpleado(ui->newPassword->text()));
        ui->newPassword->setText("");
    } else {
        QMessageBox msg;
        msg.setText("No puede tener contraseña vacía.");
        msg.exec();
    }
}

void MainWindow::on_employeeList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (current) {
        ui->removeEmployee->setText("Dar de bajar " + current->text());
    }
}

void MainWindow::newEmpleado(QPair<QString, QString> empleadoData) {
    QMessageBox msg;
    msg.setText("Nuevo empleado.\nID: " + empleadoData.first +
                "\nContraseña: " + empleadoData.second);
    msg.exec();
}
