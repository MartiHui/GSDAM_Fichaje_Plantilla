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

private:
    ServerConnection *m_connection;
    Ui::MainWindow *ui;

    void changeInfoMessage(QString message, QString color);

private slots:
    void on_ficharBtn_clicked();
    void messageReceived(QString message);

};

#endif // MAINWINDOW_H
