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

void MainWindow::fillRegistros(QVector<Registro> &registros) {
    ui->logList->clear();
    ui->historialView->clearContents();
    ui->registrosView->clearContents();

    QMap<int, QString> registrosAbiertos;
    QMap<int, QString> nombres;

    QString logListTemplate = "%1 - %2 ha %3";
    QString logListAction;
    for (Registro registro : registros) {
        if (registro.esEntrada) {
            logListAction = "entrado";
            registrosAbiertos.insert(registro.empleadoId, registro.fecha);

            if (!nombres.contains(registro.empleadoId)) {
                nombres.insert(registro.empleadoId,
                               registro.empleadoNombre + " " + registro.empleadoApellido);
            }
        } else {
            ui->historialView->insertRow(0);
            ui->historialView->setItem(0, 0, new QTableWidgetItem(nombres.value(registro.empleadoId)));
            ui->historialView->setItem(0, 1, new QTableWidgetItem(registrosAbiertos.value(registro.empleadoId)));
            ui->historialView->setItem(0, 2, new QTableWidgetItem(registro.fecha));

            registrosAbiertos.remove(registro.empleadoId);
            logListAction = "salido";
        }

        ui->logList->insertItem(0, QString(logListTemplate).arg(registro.fecha)
                                .arg(registro.empleadoNombre + " " + registro.empleadoApellido)
                                .arg(logListAction));
    }

    for (int registro : registrosAbiertos.keys()) {
        ui->registrosView->insertRow(0);
        ui->registrosView->setItem(0, 0, new QTableWidgetItem(nombres.value(registro)));
        ui->registrosView->setItem(0, 1, new QTableWidgetItem(registrosAbiertos.value(registro)));
    }
}

void MainWindow::fillEmpleados(QVector<QMap<QString, QString> > &empleados) {
    ui->employeeList->clearContents();

    int size = empleados.count();
    ui->employeeList->setRowCount(size);
    for (int i = 0; i < size; i++) {
        ui->employeeList->setItem(i, 0, new QTableWidgetItem(empleados[i]["id"]));
        ui->employeeList->setItem(i, 1, new QTableWidgetItem(empleados[i]["nombre"] + " " + empleados[i]["apellido"]));
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
        ui->pushButton->setEnabled(true);
        if (action.connectAdminSuccessful()) {
            ui->tabWidget->setEnabled(true);
            updateRegistros();
            updateEmpleados();
        } else {
            msg.setText("Credenciales incorrectas");
            msg.exec();
        }
        break;

    case ActionType::REGISTROS_INFO:
    {
        QVector<Registro> registros;

        if (action.getRegistrosInfo(&registros)) {
            fillRegistros(registros);
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
        m_connection->sendMessage(ActionJson::deleteEmpleado(id));
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
        msg.exec();
    }
}

void MainWindow::newEmpleado(QPair<QString, QString> empleadoData) {
    ui->createEmployee->setEnabled(true);
    QMessageBox msg;
    msg.setText("Nuevo empleado.\nID: " + empleadoData.first +
                "\nContraseña: " + empleadoData.second);
    msg.exec();
}
