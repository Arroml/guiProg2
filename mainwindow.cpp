#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionLoadFile, &QAction::triggered, this, &MainWindow::onLoadFile);
    connect(ui->actionSaveFile, &QAction::triggered, this, &MainWindow::onSaveFile);
    connect(ui->actionQuit, &QAction::triggered, this, &QCoreApplication::quit);
}

void MainWindow::onLoadFile(){
    qDebug() << "Load file clicked";
}

void MainWindow::onSaveFile(){
    qDebug() << "Save File clicked!";
}


MainWindow::~MainWindow()
{
    delete ui;
}
