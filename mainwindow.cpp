#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("База данных аэродрома"));

    lbl = new QLabel();
    msb = new QMessageBox();

    pilot = new mongo::BSONObj();

    QStringList qstrl;
    qstrl.append(tr("Mongos"));
    qstrl.append(tr("Реплика 1"));
    qstrl.append(tr("Реплика 2"));
    ui->comboBox_2->addItems(qstrl);


    ui->tableWidget->setColumnHidden(0, true);
    ui->tableWidget->setColumnWidth(1,100);
    ui->tableWidget->setColumnWidth(2,200);
    ui->tableWidget->setColumnWidth(3,200);
    ui->tableWidget->setColumnWidth(4,150);

    ui->tableWidget->setShowGrid(true);



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
    delete lbl;
    delete msb;
    delete pilot;
    delete ui;
}


void MainWindow ::run() {
  mongo::DBClientConnection c;
  c.connect("localhost");
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    // Если пользователь кликнул по ячейке, то row - номер строки, по которой он кликнул


}


void MainWindow::on_commandLinkButton_clicked()
{

}
