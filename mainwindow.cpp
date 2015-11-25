#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("База данных аэродрома"));

    QStringList qstrl;
    qstrl.append(tr("Mongos"));
    qstrl.append(tr("Реплика 1"));
    qstrl.append(tr("Реплика 2"));
    ui->comboBox_2->addItems(qstrl);


    mongo::client::initialize();
        try {
            run();
        } catch( const mongo::DBException &e ) {
            QString error = (tr("При попытке установить соединение возникли ошибки:\n"));
            error.append(e.what());
        }


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow ::run() {
  mongo::DBClientConnection c;
  c.connect("localhost");
}
