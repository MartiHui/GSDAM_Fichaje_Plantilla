#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

void MainWindow::fillRegistro(QVector<Registro> registros) {
    ui->registrosView->clear();
    int size = registros.count();

    ui->registrosView->setRowCount(size);
    for (int row = 0; row < size; row++) {
        ui->registrosView->setItem(row, 0, registros[row].id);
        ui->registrosView->setItem(row, 1, registros[row].fecha_entrada);
    }
}

void MainWindow::fillHistorial(QVector<Registro> registros) {
    ui->historialView->clear();
    int size = registros.count();

    ui->historialView->setRowCount(size);
    for (int row = 0; row < size; row++) {
        ui->historialView->setItem(row, 0, registros[row].id);
        ui->historialView->setItem(row, 1, registros[row].fecha_entrada);
        ui->historialView->setItem(row, 2, registros[row].fecha_salida);
    }
}
