#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QTimer>

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
        ui->ficharBtn->setEnabled(false);

        QString errorMsg;
        if (info.isValid(errorMsg)) {
            ui->userIdentification->clear();
            ui->userPassword->clear();

            m_connection->sendMessage(info.toJson());
        } else {
            changeInfoMessage(errorMsg, "red");
        }
    }
}

void MainWindow::messageReceived(QString message) {
    QJsonObject answer = QJsonDocument::fromJson(message.toUtf8()).object();

    QString msg = answer["message"].toString();
    QString color = (answer["requestSuccessful"].toBool()) ? "green" : "red";

    changeInfoMessage(msg, color);
}

void MainWindow::changeInfoMessage(QString message, QString color) {
    ui->infoLabel->setText(message);
    ui->infoLabel->setStyleSheet(QString("QLabel {border: 2px solid %1;}").arg(color));
    QTimer::singleShot(3000, []() {
        ui->infoLabel->setText("");
        ui->ficharBtn->setEnabled(true);
    });
}
