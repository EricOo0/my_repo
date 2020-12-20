#ifndef CHATTING_WINDOW_H
#define CHATTING_WINDOW_H
#include<QTcpSocket>
#include <QMainWindow>
#include<QKeyEvent>
namespace Ui {
class chatting_window;
}

class chatting_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit chatting_window(QWidget *parent,QString name);
    void parent_show(QWidget *parent);
    void msg_recv();
    void msg_send();
    void usrin();
    void usrleave();
    void usrmsg();
    enum Msgtype{Usrin,Usrleave,Msg};
    QTcpSocket *tcp;
    ~chatting_window();

private slots:
    void on_recv_box_textChanged();

private:
    Ui::chatting_window *ui;
    QString usrname;
};

#endif // CHATTING_WINDOW_H
