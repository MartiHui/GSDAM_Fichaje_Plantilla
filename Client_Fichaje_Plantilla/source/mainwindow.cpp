#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "employeeinfo.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_ficharBtn_clicked() {
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

void MainWindow::close() {
    m_connection->close();
}
