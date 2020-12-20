#include "chatting_window.h"
#include "ui_chatting_window.h"
#include<QPushButton>
#include<QTcpSocket>
#include<QTcpServer>
#include<QDateTime>
#include <QKeyEvent>//按键事件
#include<QColorDialog>//颜色
chatting_window::chatting_window(QWidget *parent,QString name) :
    QMainWindow(parent),
    ui(new Ui::chatting_window),
    usrname(name)
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
    //enter按键触发
    this->ui->btn_send->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Enter));
    this->ui->btn_send->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Return));//两个必须同时有
    //添加自己到右侧usr栏目
    QTableWidgetItem *usr = new QTableWidgetItem(this->usrname);
    this->ui->user_box->insertRow(0);
    this->ui->user_box->setItem(0,0,usr);
    this->ui->online_lable->setText(QString("online: %2 people").arg(this->ui->user_box->rowCount()-1));//统计人数
    this->ui->recv_box->setTextColor(Qt::gray);
    this->ui->recv_box->append(this->usrname+" enter");//消息
    this->ui->recv_box->setTextColor(Qt::black);

    //幅值功能设置
    //字体设置
    connect(ui->font,&QFontComboBox::currentFontChanged,[=](const QFont &font){
        ui->recv_box->setCurrentFont(font);
        ui->send_box->setCurrentFont(font);
        ui->send_box->setFocus();
    });
    //字号
    void (QComboBox::* fontsize)(const QString &text)=&QComboBox::currentIndexChanged;

    connect(ui->size,fontsize,[=](const QString &text){
        ui->recv_box->setFontPointSize(text.toDouble());
        ui->send_box->setFontPointSize(text.toDouble());
        ui->send_box->setFocus();
    });
    //加粗
    connect(ui->bold,&QToolButton::clicked,[=](const bool ischeck){
        if(ischeck){
            ui->send_box->setFontWeight(QFont::Bold);
         //   ui->recv_box->setFontWeight(QFont::Bold);
        }
        else{
            ui->send_box->setFontWeight(QFont::Normal);
        //    ui->recv_box->setFontWeight(QFont::Normal);
        }
    });
    //倾斜
    connect(ui->xie,&QToolButton::clicked,[=](const bool ischeck){
        ui->send_box->setFontItalic(ischeck);
      //  ui->recv_box->setFontItalic(ischeck);
    });
    //颜色
    connect(ui->color,&QToolButton::clicked,[=](){
        QColor color = QColorDialog::getColor(Qt::red,this);
        ui->send_box->setTextColor(color);
    });



}

void chatting_window::msg_recv(){
    //!消息类型!#消息发送者#消息
    QByteArray recv=this->tcp->read(1024);
    qDebug()<<recv;
    QString time = QDateTime::currentDateTime().toString();
    //接收消息类型
    int begin = recv.indexOf('!',0);
    int  end=recv.lastIndexOf('!');
    QString type = recv.mid(begin+1,end-begin-1);

     begin = recv.indexOf('#',0);
     end=recv.lastIndexOf('#');
     QString name = recv.mid(begin+1,end-begin-1);

     this->ui->recv_box->setFont(this->ui->font->font());
     this->ui->recv_box->setFontPointSize(this->ui->size->currentText().toDouble());
    if(type == "usrin"){
        //将用户加入右侧user栏目
            bool online = this->ui->user_box->findItems(name,Qt::MatchExactly).isEmpty();//判断是否在use里
            if(online){
                QTableWidgetItem *usr = new QTableWidgetItem(name);
                this->ui->user_box->insertRow(0);
                this->ui->user_box->setItem(0,0,usr);
                this->ui->online_lable->setText(QString("online: %2 people").arg(this->ui->user_box->rowCount()-1));//统计人数
            }
            this->ui->recv_box->setTextColor(Qt::gray);
            this->ui->recv_box->append(name+" enter");//消息
            this->ui->recv_box->setTextColor(Qt::black);
    }
    else if(type == "usrleave"){
        //将用户右侧user栏目删除
            bool online = this->ui->user_box->findItems(name,Qt::MatchExactly).isEmpty();//判断是否在use里
            if(!online){
                int row = this->ui->user_box->findItems(name,Qt::MatchExactly).first()->row();
                this->ui->user_box->removeRow(row);
                this->ui->online_lable->setText(QString("online: %2 people").arg(this->ui->user_box->rowCount()-1));//统计人数
            }
            this->ui->recv_box->setTextColor(Qt::gray);
            this->ui->recv_box->append(name+" leave");//消息
            this->ui->recv_box->setTextColor(Qt::black);
    }
    else if(type == "msg"){
        this->ui->recv_box->append("["+name+"]"+time+":");//时间
        QString msg = recv.mid(end+1);
        this->ui->recv_box->append(msg);//消息
        //将用户加入右侧user栏目
            bool online = this->ui->user_box->findItems(name,Qt::MatchExactly).isEmpty();//判断是否在use里
            if(online){
                QTableWidgetItem *usr = new QTableWidgetItem(name);
                this->ui->user_box->insertRow(0);
                this->ui->user_box->setItem(0,0,usr);

                this->ui->online_lable->setText(QString("online: %2 people").arg(this->ui->user_box->rowCount()-1));//统计人数
            }

    }
}
void chatting_window::msg_send(){
    //发送消息   type+usrname+msg;
      qDebug()<< this->ui->send_box->toPlainText();
      QByteArray test =this->ui->send_box->toPlainText().toUtf8();
      QByteArray msg = "!msg!";
      msg +="#"+this->usrname+"#"+test;
      this->tcp->write(msg);
      QString time = QDateTime::currentDateTime().toString();
      this->ui->recv_box->append("[ me ]"+time+":");
      this->ui->recv_box->append(test);
      this->ui->send_box->clear();
}
void chatting_window::usrin(){
    //发送hello消息   type+usrname+msg="";
     QByteArray  msg="!usrin!";
     msg +="#"+this->usrname+"#";
    this->tcp->write(msg);
}
void chatting_window::usrleave(){
    //发送leave消息   type+usrname+msg="";
   QByteArray  msg="!usrleave!";
    msg +="#"+this->usrname+"#";
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

void chatting_window::on_recv_box_textChanged()
{
    this->ui->recv_box->moveCursor(QTextCursor::End);
}
