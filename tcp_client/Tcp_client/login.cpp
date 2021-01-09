#include "login.h"
#include "ui_login.h"
#include "dialog.h"
#include "ui_dialog.h"
#include "reginster.h"
#include<QPushButton>
#include<QDialog>
#include<QDebug>
#include<QMessageBox>
#include<QTcpSocket>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{

    ui->setupUi(this);
    connect(ui->Sign_in,&QPushButton::clicked,this,&Widget::sign_in);//点击登录 跳转
    connect(ui->sign_up,&QPushButton::clicked,this,&Widget::sign_up);//点击注册 跳转
    this->ui->Sign_in->setShortcut(QKeySequence(Qt::Key_Enter));
    this->ui->Sign_in->setShortcut(QKeySequence(Qt::Key_Return));

}

Widget::~Widget()
{
    delete ui;
}

void Widget::sign_in(){
    //连接到数据库去验证消息
    this->tcp = new QTcpSocket(this);
    this->tcp->connectToHost("49.234.75.120",10000);//连接服务器

    //QMessageBox *mymessage = new QMessageBox(this);
    //用户名不能为空
    if(this->ui->usrname->text() == ""){
        QMessageBox::warning(this,"error","usrname shold not be empty");
    }
    else{
        QByteArray usrname= this->ui->usrname->text().toUtf8();
        QByteArray passwd = this->ui->passwd->text().toUtf8();
        //登录消息格式 #login#usrname||passwd
        QByteArray msg = "#login#"+usrname+"||"+passwd;
        this->tcp->write(msg);
        this->tcp->waitForReadyRead(-1);
        //回复消息格式#login#failed或者#login#success
        QByteArray recv=this->tcp->read(1024);
        qDebug()<<recv;
        int begin = recv.indexOf('#',0);
        int end = recv.lastIndexOf('#');
        QString result = recv.mid(end+1);
        if(result != "failed"){
            QString id = recv.mid(end+1);
            qDebug()<<"id:"<<id;
            if(QMessageBox::Yes ==  QMessageBox::question(this,"login",id+"log_in ?")){
                Dialog * mydialog = new Dialog(this,id);
                this->hide();
                mydialog->show();
            }
        }
        else{
            QMessageBox::warning(this,"failed","usrname or passwd is incorret");
        }
    }
    this->tcp->close();
}
void Widget::sign_up(){
    //连接到数据库去注册
    this->tcp = new QTcpSocket(this);//创建tcp连接
    Reginster * reg = new Reginster(this->parentWidget(),this->tcp);
    reg->show();
}
