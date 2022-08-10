#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    if ( translator.load("tetris_ru.qm",":/Translation")==true){
        QApplication::installTranslator(&translator);}

    MainWindow w;
    w.show();
    return a.exec();
}
