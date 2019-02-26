#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "employeeinfo.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);

    m_connection = new ServerConnection("ws://localhost:1234");
    connect(m_connection, SIGNAL(messageReceived(QString)), this, SLOT(messageReceived(QString)));
}

MainWindow::~MainWindow() {
    delete m_connection;
    delete ui;
}

void MainWindow::on_ficharBtn_clicked() {
    if (!m_connection->isValid()) {
        QMessageBox msg;
        msg.setText("No estás conectado al servidor");
        msg.exec();
    } else {
        EmployeeInfo info{ui->userIdentification->text(), ui->userPassword->text()};

        QString errorMsg;
        if (info.isValid(errorMsg)) {
            ui->userIdentification->clear();
            ui->userPassword->clear();
            ui->ficharBtn->setEnabled(false);

            m_connection->sendMessage(info.toJson());
        } else {
            QMessageBox msg;
            msg.setText("El código de identificación no es correcto. " + errorMsg);
            msg.exec();
        }
    }
}

void MainWindow::messageReceived(QString message) {
    ui->ficharBtn->setEnabled(true);

    QJsonValue requestResult = QJsonDocument::fromJson(message.toUtf8()).object()["isRequestSuccesful"];
    QMessageBox msg;

    if (requestResult == QJsonValue::Undefined || !requestResult.toBool()) {
        msg.setText("Ha habido problemas con tu solicitud");
    } else {
        msg.setText("Solicitud realizada correctamente");
    }

    msg.exec();
}
