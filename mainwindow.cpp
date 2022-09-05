#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this); //
    ui->glass->glassInit(); //
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);

    QObject::connect(ui->glass, SIGNAL(signalChangePattern(Figure*)), ui->nextFigure, SLOT(slotChangePattern(Figure*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

