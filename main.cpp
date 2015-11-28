#include "mainwindow.h"
#include <QApplication>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    MainWindow w;
    w.show();

    return a.exec();
}

/* Поиск производится по названию эскадрильи. Выдается список пилотов и их з/п.
 * При нажатии на пилота выдается полная информация по нему, кроме квалификации.
 * Также возможен поиск по модели самолета.
 * В таком случае выдается список пилотов и количество часов, которые они налетали на данной модели самолета. */
