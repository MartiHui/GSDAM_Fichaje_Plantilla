#include <QDebug>
#include <QTimer>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "actionjson.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_connection = new ServerConnection("ws://localhost:1234");
    connect(m_connection, SIGNAL(messageReceived(QString)),
            this, SLOT(messageReceived(QString)));

    setTablesHeader();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setTablesHeader() {
    QStringList registro;
    registro << "ID" << "Fecha de entrada";
    ui->registrosView->setHorizontalHeaderLabels(registro);
    ui->registrosView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QStringList historial;
    historial << "ID" << "Fecha de entrada" << "Fecha de salida";
    ui->historialView->setHorizontalHeaderLabels(historial);
    ui->historialView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::fillRegistro(QVector<Registro> &registros) {
    ui->registrosView->clear();
    int size = registros.count();

    ui->registrosView->setRowCount(size);
    for (int row = 0; row < size; row++) {
        ui->registrosView->setItem(row, 0, new QTableWidgetItem(registros[row].id));
        ui->registrosView->setItem(row, 1, new QTableWidgetItem(registros[row].fecha_entrada));
    }
}

void MainWindow::fillHistorial(QVector<Registro> &registros) {
    ui->historialView->clear();
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

    switch (action.getActionType()) {
    case ActionType::INVALID:

        break;

    case ActionType::CONNECTION:

        break;

    case ActionType::REGISTROS_INFO:
        QVector<Registro> registrosAbiertos;
        QVector<Registro> registroHistorial;

        if (action.getRegistrosInfo(&registrosAbiertos, &registroHistorial)) {
            fillRegistro(registrosAbiertos);
            fillHistorial(registroHistorial);
        }
        break;
    }
}

void MainWindow::on_pushButton_clicked()
{
    m_connection->sendMessage(ActionJson::askRegistrosInfo());
}
