#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMessageBox>
#include <mongo/client/dbclient.h>
#include <mongo/bson/bson.h>

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

private:
    Ui::MainWindow *ui;
    QLabel* lbl;
    QMessageBox* msb;

    mongo::BSONObj* pilot;

    void run();
};

#endif // MAINWINDOW_H
