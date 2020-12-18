#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->ation1->setIcon(QIcon(":/icon/icon/OIP.jpg"));
    ui->actionedit->setIcon(QIcon(":/icon/icon/OIP.jpg"));
    //QAction *text =   ui->menunew->addAction("text");
    //text->setIcon(QIcon(":/icon/icon/OIP.jpg"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
