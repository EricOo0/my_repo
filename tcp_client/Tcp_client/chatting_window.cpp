#include "chatting_window.h"
#include "ui_chatting_window.h"
#include<QPushButton>
#include<QTcpSocket>
#include<QTcpServer>
#include<QDateTime>

chatting_window::chatting_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::chatting_window)
{

    ui->setupUi(this);
    connect(ui->btn_quit,&QPushButton::clicked,this,[=](){
       this->parent_show(parent);
    }); //关闭窗口的触发

    this->tcp = new QTcpSocket(this);
    this->tcp->connectToHost("49.234.75.120",10000);//连接服务器
    this->usrin();//进页面先给其他用户法消息 #usrin#
    connect(tcp,&QTcpSocket::readyRead,this,&chatting_window::msg_recv);//接受消息触发
    connect(ui->btn_send,&QPushButton::clicked,this,&chatting_window::msg_send);//发送消息触发


}
void chatting_window::msg_recv(){
    //#消息发送者#!消息类型!消息
    QByteArray recv=this->tcp->read(1024);
    qDebug()<<recv;
    QString time = QDateTime::currentDateTime().toString();
    //接收消息类型


    int begin = recv.indexOf('#',0);
    int end=recv.lastIndexOf('#');
     QString name = recv.mid(begin+1,end-begin-1);

     begin = recv.indexOf('!',0);
     end=recv.lastIndexOf('!');
    QString type = recv.mid(begin+1,end-begin-1);
   //这里要有个if
    qDebug()<<type;
    qDebug()<<name;
    if(type == "usrin"){
        //将用户加入右侧user栏目
            bool online = this->ui->user_box->findItems(name,Qt::MatchExactly).isEmpty();//判断是否在use里
            if(online){
                QTableWidgetItem *usr = new QTableWidgetItem(name);
                this->ui->user_box->insertRow(0);
                this->ui->user_box->setItem(0,0,usr);
                this->ui->online_lable->setText(QString("online: %2 people").arg(this->ui->user_box->rowCount()));//统计人数
            }
            this->ui->recv_box->setTextColor(Qt::gray);
            this->ui->recv_box->insertPlainText(name+" enter"+"\n");//消息
            this->ui->recv_box->setTextColor(Qt::black);
    }
    else if(type == "usrleave"){
        //将用户右侧user栏目删除
            bool online = this->ui->user_box->findItems(name,Qt::MatchExactly).isEmpty();//判断是否在use里
            if(!online){
                int row = this->ui->user_box->findItems(name,Qt::MatchExactly).first()->row();
                this->ui->user_box->removeRow(row);
                this->ui->online_lable->setText(QString("online: %2 people").arg(this->ui->user_box->rowCount()));//统计人数
            }
            this->ui->recv_box->setTextColor(Qt::gray);
            this->ui->recv_box->insertPlainText(name+" leave"+"\n");//消息
            this->ui->recv_box->setTextColor(Qt::black);
    }
    else if(type == "msg"){
        this->ui->recv_box->insertPlainText("["+name+"]"+time+":\n");//时间
        QString msg = recv.mid(end+1);
        this->ui->recv_box->insertPlainText(msg+"\n");//消息
        //将用户加入右侧user栏目
            bool online = this->ui->user_box->findItems(name,Qt::MatchExactly).isEmpty();//判断是否在use里
            if(online){
                QTableWidgetItem *usr = new QTableWidgetItem(name);
                this->ui->user_box->insertRow(0);
                this->ui->user_box->setItem(0,0,usr);

                this->ui->online_lable->setText(QString("online: %2 people").arg(this->ui->user_box->rowCount()));//统计人数
            }

    }
}
void chatting_window::msg_send(){
      qDebug()<< this->ui->send_box->toPlainText();
      QByteArray test =this->ui->send_box->toPlainText().toUtf8();
      this->tcp->write("!msg!"+test);
      QString time = QDateTime::currentDateTime().toString();
      this->ui->recv_box->insertPlainText("[ me ]"+time+":\n");
      this->ui->recv_box->insertPlainText(test+"\n");
      this->ui->send_box->clear();
}
void chatting_window::usrin(){
     QByteArray  msg="!usrin!";

   this->tcp->write(msg);
}
void chatting_window::usrleave(){
   QByteArray  msg="!usrleave!";
   this->tcp->write(msg);
}
void chatting_window::parent_show(QWidget *parent){
    this->hide();
    this->usrleave();
    this->tcp->close();
    this->tcp->destroyed();
     parent->show();
}
chatting_window::~chatting_window()
{
    delete ui;
}
