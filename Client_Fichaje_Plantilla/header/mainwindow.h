#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "serverconnection.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void close(); // Cierra la conexion con el servidor

private:
    ServerConnection *m_connection;

private slots:
    void on_ficharBtn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
