#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("База данных аэродрома"));

    // Информационное окно с сообщением об ошибке
    msb = new QMessageBox();
    msb->setWindowTitle(tr("Сообщение об ошибке"));
    msb->setText(tr("Отсутсвует соединение с сервером!"));

    QStringList qstrl;
    qstrl.append("Mongos");
    qstrl.append("ReplSet 1");
    qstrl.append("ReplSet 2");
    ui->comboBox_2->addItems(qstrl); // Добавляем варианты

    qstrl.clear();
    qstrl.append("4242");
    qstrl.append("4241");
    qstrl.append("SUAI");
    qstrl.append("kaf44");
    ui->comboBox->addItems(qstrl);

    // Скрываем поля: столбец 0 (Паспорт) является служебным
    // Столбцы Модель и Налёт (5 и 6) используются в режиме поиска по модели
    ui->tableWidget->setColumnHidden(0, true);
    ui->tableWidget->setColumnHidden(5, true);
    ui->tableWidget->setColumnHidden(6, true);

    // Задаём ширину столбцов
    ui->tableWidget->setColumnWidth(1,100);
    ui->tableWidget->setColumnWidth(2,200);
    ui->tableWidget->setColumnWidth(3,200);
    ui->tableWidget->setColumnWidth(4,120);
    ui->tableWidget->setColumnWidth(5,120);
    ui->tableWidget->setColumnWidth(6,120);

    // Показываем сетку
    ui->tableWidget->setShowGrid(true);

    // Настройки выделения: выделять можно только строки целеком
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Минимальная ширина
    ui->tableWidget->setMinimumWidth(660);

    // Для второй таблицы - размеры и включение сетки
    ui->tableWidget_2->setMinimumWidth(220);
    ui->tableWidget_2->setMaximumWidth(300);
    ui->tableWidget_2->setShowGrid(true);

    // По умолчанию используем поиск по номеру эскадрильи
    ui->radioButton->setChecked(true);
    ui->label_5->hide();
    ui->lineEdit->hide();
    squadron = true;

    connection = new mongo::DBClientConnection(); // Создаём соединение
    mongo::client::initialize();
}

// =================================

MainWindow::~MainWindow()
{
    delete msb;
    delete connection;
    delete ui;
}

// =================================

void MainWindow::on_radioButton_clicked()
{
    // Чистим таблицу - удаляем строки
    int n = ui->tableWidget->rowCount();
    for( int i = 0; i < n; i++ )
             ui->tableWidget->removeRow(0);

    // Чистим вторую таблицу, удаляя столбец
    ui->tableWidget_2->removeColumn(0);

    ui->label_5->hide(); // Прячем элементы
    ui->lineEdit->hide();

    ui->label->show(); // Показываем элементы
    ui->comboBox->show();

    squadron = true; // Поднимаем флаг

    // Изменяем таблицу
    ui->tableWidget->setColumnHidden(1, false);
    ui->tableWidget->setColumnHidden(4, false);
    ui->tableWidget->setColumnHidden(5, true);
    ui->tableWidget->setColumnHidden(6, true);
}

// =================================

void MainWindow::on_radioButton_2_clicked()
{
    int n = ui->tableWidget->rowCount();
    for( int i = 0; i < n; i++ )
             ui->tableWidget->removeRow(0);

    ui->tableWidget_2->removeColumn(0);

    ui->label_5->show();
    ui->lineEdit->show();

    ui->label->hide();
    ui->comboBox->hide();

     squadron = false;

     ui->tableWidget->setColumnHidden(1, true);
     ui->tableWidget->setColumnHidden(4, true);
     ui->tableWidget->setColumnHidden(5, false);
     ui->tableWidget->setColumnHidden(6, false);
}

// =================================

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    // Если пользователь кликнул по ячейке, то row - номер строки, по которой он кликнул
    if (connect()) // Если есть соединение
    {
        QTableWidgetItem* pass = ui->tableWidget->item(row, 0); // Получаем 0ю ячейку из выбранной строки
        QString str = pass->text(); // Считываем текст из ячейки - это номер паспорта
        mongo::Query query = MONGO_QUERY("passport" << str.toInt()); // Формируем запрос
        std::auto_ptr<mongo::DBClientCursor> cursor = connection->query("aero.pilots", query); // Отправляем запрос

        str.clear(); // Очищаем строку

        ui->tableWidget_2->removeColumn(0);
        ui->tableWidget_2->insertColumn(0);

        mongo::BSONObj obj = cursor->next(); // Считываем информацию через курсор

        // Добавляем данные в таблицу
        ui->tableWidget_2->setItem(0, 0, new QTableWidgetItem(QString::fromStdString(obj.getStringField("name"))));
        ui->tableWidget_2->setItem(1, 0, new QTableWidgetItem(QString::fromStdString(obj.getStringField("surname"))));
        ui->tableWidget_2->setItem(2, 0, new QTableWidgetItem(QString::fromStdString(obj.getStringField("gender"))));
        ui->tableWidget_2->setItem(3, 0, new QTableWidgetItem(QString::number(obj.getIntField("passport"), 10)));
        ui->tableWidget_2->setItem(4, 0, new QTableWidgetItem(QString::fromStdString(obj.getStringField("squadron"))));
        ui->tableWidget_2->setItem(5, 0, new QTableWidgetItem(QString::fromStdString(obj.getStringField("rank"))));
        ui->tableWidget_2->setItem(6, 0, new QTableWidgetItem(QString::number(obj.getIntField("salary"), 10)));

        ui->tableWidget_2->horizontalHeader()->setHidden(true);  // Скрываем автоматически добавляемый заголовок
    }
    else // Если соединения нет
    {
        msb->show(); // Показываем сообщение об ошибке
    }
}

// =================================

void MainWindow::on_commandLinkButton_clicked()
{
    if (connect())
    {
        int n = ui->tableWidget->rowCount();
        for( int i = 0; i < n; i++ )
                 ui->tableWidget->removeRow(0);


        if (squadron) // Поиск по номеру эскадрильи
        {
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

                mongo::Query query = MONGO_QUERY("squadron" << sqdr); // Формируем запрос
                std::auto_ptr<mongo::DBClientCursor> cursor = connection->query("aero.pilots", query); // Отправляем запрос

                while (cursor->more()) // Считываем информацию из курсора
                {
                        mongo::BSONObj obj = cursor->next(); // Получаем объект
                        ui->tableWidget->insertRow(0); // Добавляем строку
                        // Заносим в строку информацию
                        ui->tableWidget->setItem(0, 0, new QTableWidgetItem(QString::number(obj.getIntField("passport"), 10)));
                        ui->tableWidget->setItem(0, 1, new QTableWidgetItem(QString::fromStdString(obj.getStringField("squadron"))));
                        ui->tableWidget->setItem(0, 2, new QTableWidgetItem(QString::fromStdString(obj.getStringField("name"))));
                        ui->tableWidget->setItem(0, 3, new QTableWidgetItem(QString::fromStdString(obj.getStringField("surname"))));
                        ui->tableWidget->setItem(0, 4, new QTableWidgetItem(QString::number(obj.getIntField("salary"), 10)));
                }
        }
        else // Иначе поиск по модели самолёта
        {
                std::string model =ui->lineEdit->text().toStdString(); //  Получаем название модели и превращаем его в стандартную строку
                mongo::BSONObj res; // Объект - результат
                // Формируем запрос
                mongo::BSONArray pipeline = BSON_ARRAY(
                            BSON("$unwind" << "$skill"
                                ) <<
                            BSON("$match" <<
                                 BSON("skill.model" << model
                                     )
                                ) <<
                            BSON("$project" <<
                                BSON("passport" << 1 <<
                                     "name" << 1 <<
                                     "surname" << 1 <<
                                     "skill.model" << 1 <<
                                     "skill.time" << 1
                                     )
                                )

                            );
                // Отправляем запрос
                connection->runCommand("aero", BSON("aggregate" << "pilots" << "pipeline" << pipeline), res);

                // Преобразуем полученный результат в строку
                QString result = QString::fromStdString(res.toString());
                // Убираем из строки всё лишнее
                result = result.replace("\"", " ");
                result = result.replace("\[", " ");
                result = result.replace("]", " ");
                result = result.replace(QRegExp("[:,'(){}]"), " ");
                result = result.replace(QRegExp("(result | _id | ObjectId | name | surname | skill | model | time | passport)"), " ");
                result = result.simplified();

                // разделим исходную строку на подстроки
                // получаем список строк, каждая из которых - слово
                // параметр QString::SkipEmptyParts запрещает создание пустых строк
                QStringList qsl = result.split(QRegularExpression("\\s"), QString::SkipEmptyParts);
                n = qsl.size();
                // Выводим результат, добавляя строки в таблицу
                for (int i = 0; i<(n-12); i+=6)
                {
                        ui->tableWidget->insertRow(0);
                        ui->tableWidget->setItem(0, 0, new QTableWidgetItem(qsl[i+3]));
                        ui->tableWidget->setItem(0, 2, new QTableWidgetItem(qsl[i+2])); // Имя
                        ui->tableWidget->setItem(0, 3, new QTableWidgetItem(qsl[i+1])); // Фамилия
                        ui->tableWidget->setItem(0, 5, new QTableWidgetItem(qsl[i+4])); // Модель
                        ui->tableWidget->setItem(0, 6, new QTableWidgetItem(qsl[i+5])); // Налёт
                }
        }
    }
    else
    {
        msb->show();
    }
}

// =================================

bool MainWindow::connect()
{
    if (connection != 0) // Если соединение есть, удаляем его
        delete connection;

    QString outputText; // Строка состояния

    std::vector<mongo::HostAndPort> rs0, rs1;
    rs0.push_back(mongo::HostAndPort("localhost", 10001));
    rs0.push_back(mongo::HostAndPort("localhost", 10002));
    rs0.push_back(mongo::HostAndPort("localhost", 10003));
    rs1.push_back(mongo::HostAndPort("localhost", 10004));
    rs1.push_back(mongo::HostAndPort("localhost", 10005));

    mongo::DBClientReplicaSet *connection2; // Соединение с набором реплик
    mongo::DBClientConnection *connection1; // Соединение с mongos

    if (ui->comboBox_2->currentText() == "Mongos") // Если выбрано соединение с mongos
    {
        try {
            connection1 = new mongo::DBClientConnection(); // Создаём соединение
            connection1->connect("localhost:27017"); // Пытаемся утсановить его с портом 27017
            outputText += "connected ok"; // Всё хорошо
            ui->stats->setText(outputText); // Выводим сообщение
        } catch ( const mongo::DBException &e ) {
            outputText += "caught "; // Если возникает ошибка
            outputText += e.what(); // Выводим текст ошибки
            ui->stats->setText(outputText);
            return false; // Возвращаем false
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

// =================================



