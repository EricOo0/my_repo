#include "reginster.h"
#include "ui_reginster.h"
#include<QMessageBox>
#include<QPushButton>
#include<QDebug>
Reginster::Reginster(QWidget *parent,QTcpSocket *tcp) :
    QWidget(parent),
    ui(new Ui::Reginster)
{
    ui->setupUi(this);
    connect(this->ui->btn_signup,&QPushButton::clicked,this,[=](){
       regiter(tcp);
    });//按下注册连接数据库

}
void Reginster::regiter(QTcpSocket *tcp){
    QByteArray usrname= this->ui->usrname->text().toUtf8();
    QByteArray passwd = this->ui->passwd->text().toUtf8();
    QByteArray id = this->ui->id->text().toUtf8();
    if(this->ui->usrname->text() == ""){
        QMessageBox::warning(this,"error","usrname shold not be empty");
        this->close();
        return;
    }
    tcp->connectToHost("49.234.75.120",10000);//连接服务器
    //注册消息格式#register#usrname||passwd$$id
    QByteArray msg = "#register#"+usrname+"||"+passwd+"$$"+id;
    tcp->write(msg);
    tcp->waitForReadyRead(-1);
    QByteArray recv=tcp->read(1024);
    //返回消息格式#register#success/#register#failed
    int begin = recv.indexOf('#',0);
    int end = recv.lastIndexOf('#');
    QString result = recv.mid(end+1);
    QMessageBox::information(this,"notice",result);
    this->close();
    tcp->close();

}
Reginster::~Reginster()
{
    delete ui;
}
