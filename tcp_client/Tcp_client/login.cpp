#include "login.h"
#include "ui_login.h"
#include "dialog.h"
#include "ui_dialog.h"
#include<QPushButton>
#include<QDialog>
#include<QDebug>
#include<QMessageBox>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->Sign_in,&QPushButton::clicked,this,&Widget::slot1);//点击登录 跳转
    this->ui->Sign_in->setShortcut(QKeySequence(Qt::Key_Enter));
    this->ui->Sign_in->setShortcut(QKeySequence(Qt::Key_Return));

}

Widget::~Widget()
{
    delete ui;
}

void Widget::slot1(){
    //QMessageBox *mymessage = new QMessageBox(this);
    if(this->ui->usrname->text() == ""){
        QMessageBox::warning(this,"error","usrname shold not be empty");
    }
    else{
        QString name=this->ui->usrname->text();
        if(QMessageBox::Yes ==  QMessageBox::question(this,"login","log_in ?")){
            qDebug()<<"login!";
            Dialog * mydialog = new Dialog(this,name);
            this->hide();
            mydialog->show();
        }
    }
}
