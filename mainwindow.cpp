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
    qstrl.append("Mongos");
    qstrl.append("ReplSet 1");
    qstrl.append("ReplSet 2");
    ui->comboBox_2->addItems(qstrl);

    qstrl.clear();
    qstrl.append("4242");
    qstrl.append("4241");
    qstrl.append("SUAI");
    qstrl.append("kaf44");
    ui->comboBox->addItems(qstrl);

    ui->tableWidget->setColumnHidden(0, true);
    ui->tableWidget->setColumnWidth(1,100);
    ui->tableWidget->setColumnWidth(2,200);
    ui->tableWidget->setColumnWidth(3,200);
    ui->tableWidget->setColumnWidth(4,120);

    ui->tableWidget->setShowGrid(true);

    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableWidget->setMinimumWidth(660);

    ui->tableWidget_2->setMaximumWidth(350);
    ui->tableWidget_2->setShowGrid(true);

    connection = new mongo::DBClientConnection();
    mongo::client::initialize();


}

MainWindow::~MainWindow()
{
    delete lbl;
    delete msb;
    delete pilot;
    delete connection;
    delete ui;
}




void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    // Если пользователь кликнул по ячейке, то row - номер строки, по которой он кликнул

    if (connect())
    {
        QTableWidgetItem* pass = ui->tableWidget->item(row, 0);
        QString str = pass->text();
        mongo::Query query = MONGO_QUERY("passport" << str.toInt());
        std::auto_ptr<mongo::DBClientCursor> cursor = connection->query("aero.pilots", query);

        str.clear();

        ui->tableWidget_2->removeColumn(0);
        ui->tableWidget_2->insertColumn(0);

        mongo::BSONObj obj = cursor->next();

        ui->tableWidget_2->setItem(0, 0, new QTableWidgetItem(QString::fromStdString(obj.getStringField("name"))));
        ui->tableWidget_2->setItem(1, 0, new QTableWidgetItem(QString::fromStdString(obj.getStringField("surname"))));
        ui->tableWidget_2->setItem(2, 0, new QTableWidgetItem(QString::fromStdString(obj.getStringField("gender"))));
        ui->tableWidget_2->setItem(3, 0, new QTableWidgetItem(QString::number(obj.getIntField("passport"), 10)));
        ui->tableWidget_2->setItem(4, 0, new QTableWidgetItem(QString::fromStdString(obj.getStringField("squadron"))));
        ui->tableWidget_2->setItem(5, 0, new QTableWidgetItem(QString::fromStdString(obj.getStringField("rank"))));
        ui->tableWidget_2->setItem(6, 0, new QTableWidgetItem(QString::number(obj.getIntField("salary"), 10)));

        ui->tableWidget_2->horizontalHeader()->setHidden(true);


    }
}


void MainWindow::on_commandLinkButton_clicked()
{
    if (connect())
    {
        int n = ui->tableWidget->rowCount();
        for( int i = 0; i < n; i++ )
                 ui->tableWidget->removeRow(0);


        std::string sqdr;
        switch (ui->comboBox->currentIndex())
        {
        case 0:
            sqdr = "4242";
            break;
        case 1:
            sqdr = "4241";
            break;
        case 2:
            sqdr = "SUAI";
            break;
        case 3:
            sqdr = "kaf44";
            break;
        }

        mongo::Query query = MONGO_QUERY("squadron" << sqdr);
        std::auto_ptr<mongo::DBClientCursor> cursor = connection->query("aero.pilots", query);

        while (cursor->more())
        {
                mongo::BSONObj obj = cursor->next();
                ui->tableWidget->insertRow(0);
                ui->tableWidget->setItem(0, 0, new QTableWidgetItem(QString::number(obj.getIntField("passport"), 10)));
                ui->tableWidget->setItem(0, 1, new QTableWidgetItem(QString::fromStdString(obj.getStringField("squadron"))));
                ui->tableWidget->setItem(0, 2, new QTableWidgetItem(QString::fromStdString(obj.getStringField("name"))));
                ui->tableWidget->setItem(0, 3, new QTableWidgetItem(QString::fromStdString(obj.getStringField("surname"))));
                ui->tableWidget->setItem(0, 4, new QTableWidgetItem(QString::number(obj.getIntField("salary"), 10)));
        }

    }
}

bool MainWindow::connect()
{
    if (connection != 0)
        delete connection;

    QString outputText;

    std::vector<mongo::HostAndPort> rs0, rs1;
    rs0.push_back(mongo::HostAndPort("localhost", 10001));
    rs0.push_back(mongo::HostAndPort("localhost", 10002));
    rs0.push_back(mongo::HostAndPort("localhost", 10003));
    rs1.push_back(mongo::HostAndPort("localhost", 10004));
    rs1.push_back(mongo::HostAndPort("localhost", 10005));


    mongo::DBClientReplicaSet *connection2;
    mongo::DBClientConnection *connection1;

    if (ui->comboBox_2->currentText() == "Mongos")
    {
        try {
            connection1 = new mongo::DBClientConnection();
            connection1->connect("localhost:27017");
            outputText += "connected ok";
            ui->stats->setText(outputText);
        } catch ( const mongo::DBException &e ) {
            outputText += "caught ";
            outputText += e.what();
            ui->stats->setText(outputText);
            return false;
        }

        connection = connection1;
    }

    if (ui->comboBox_2->currentText() == "ReplSet 1")
    {
        try {
        connection2 = new mongo::DBClientReplicaSet("rs0", rs0);
        outputText += "connected ok";
        ui->stats->setText(outputText);
        } catch( const mongo::DBException &e ) {
            outputText += "caught ";
            outputText += e.what();
            ui->stats->setText(outputText);
            return false;
        }
        connection = connection2;
    }

    if (ui->comboBox_2->currentText() == "ReplSet 2")
    {
        try {
        connection2 = new mongo::DBClientReplicaSet("rs1", rs1);
        outputText += "connected ok";
        ui->stats->setText(outputText);
        } catch( const mongo::DBException &e ) {
            outputText += "caught ";
            outputText += e.what();
            ui->stats->setText(outputText);
            return false;
        }
        connection = connection2;
    }

    return true;
}
