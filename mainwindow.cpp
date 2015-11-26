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
    ui->tableWidget->setColumnWidth(4,150);

    ui->tableWidget->setShowGrid(true);

    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);


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


}


void MainWindow::on_commandLinkButton_clicked()
{
    if (connect())
    {
        mongo::Query query = MONGO_QUERY("squadron" << 111);
        std::auto_ptr<mongo::DBClientCursor> cursor = connection->query("aero.pilots", query);

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
            connection1->connect("localhost:10007");
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

    outputText += "\n\n";
    return true;
}
