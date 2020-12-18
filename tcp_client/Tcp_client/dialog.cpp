#include "dialog.h"
#include "ui_dialog.h"
#include<chatting_window.h>
#include<QToolButton>
#include<QSize>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{

    ui->setupUi(this);
    this->setWindowTitle("author:wzf");
    this->setWindowIcon(QPixmap(":/icon/icon/OIP.jpg"));
    //用Qtoolbutton设置头像
    //QToolButton *portrait = new QToolButton;
    //portrait->setText("wzf");
    //portrait->setIcon(QPixmap(":/icon/icon/OIP.jpg"));
    //portrait->setIconSize(QSize(50,50));
    //portrait->setAutoRaise(true);//设置透明按键
    //portrait->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);//名字按钮一起出现
    //ui->Layout_2->addWidget(portrait);//加入到垂直布局中
    //准备一个list
    QList<QString> indexlist;
    indexlist<<"1"<<"2"<<"3"<<"4"<<"5"<<"6";
    //保存按钮容器
    QVector<QToolButton *> btnvec;
    for(int i=0;i<6;i++){
        QToolButton *portrait = new QToolButton;
         portrait->setText(indexlist[i]);
         portrait->setIcon(QPixmap(":/icon/icon/OIP.jpg"));
         portrait->setIconSize(QSize(50,50));
         portrait->setAutoRaise(true);//设置透明按键
         portrait->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
         ui->Layout_2->addWidget(portrait);
         btnvec.push_back(portrait);
    }

    connect(btnvec[0],&QToolButton::clicked,this,[=](){
        this->click_on_1(btnvec[0]->text());
    });//lambda函数传参，使打开的聊天室窗口名字正确

}
void Dialog::click_on_1(QString name){
    this->hide();
    chatting_window *mychat_room = new chatting_window(this);
    mychat_room->setWindowTitle(name);
    mychat_room->show();

}
Dialog::~Dialog()
{
    delete ui;
}

