#include "login.h"
#include "mainwindow.h"
#include "dialog.h"
#include <QApplication>
#include<QDebug>
#include<QDataStream>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    w.show();



    return a.exec();//阻塞;
}
