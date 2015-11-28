#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMessageBox>
#include <mongo/client/dbclient.h>
#include <mongo/bson/bson.h>
#include <mongo/client/dbclientcursor.h>
#include <mongo/client/dbclientinterface.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_tableWidget_cellClicked(int row, int column);
    void on_commandLinkButton_clicked();
    void on_radioButton_2_clicked();
    void on_radioButton_clicked();

private:
    Ui::MainWindow *ui;
    QMessageBox* msb;
    mongo::DBClientBase* connection;

    bool connect();
    bool squadron;
};

#endif // MAINWINDOW_H
