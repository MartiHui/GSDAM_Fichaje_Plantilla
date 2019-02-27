#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

#include "datastructs.h"

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
    Ui::MainWindow *ui;

    void setTablesHeader();
    void fillRegistro(QVector<Registro> registros);
    void fillHistorial(QVector<Registro> registros);

};

#endif // MAINWINDOW_H
