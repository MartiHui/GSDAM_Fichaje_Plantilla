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
    }
        break;

    case ActionType::UPDATE:
        updateRegistros();
        break;
    }
}

void MainWindow::updateRegistros() {
    m_connection->sendMessage(ActionJson::askRegistrosInfo());
}

void MainWindow::on_pushButton_clicked() {
    QString username = ui->username->text();
    ui->username->setText("");
    QString password = ui->userpassword->text();
    ui->userpassword->setText("");

    m_connection->sendMessage(ActionJson::connectAdmin(username, password));

    ui->pushButton->setEnabled(false);
}
