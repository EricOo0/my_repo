#ifndef CHATTING_WINDOW_H
#define CHATTING_WINDOW_H
#include<QTcpSocket>
#include <QMainWindow>

namespace Ui {
class chatting_window;
}

class chatting_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit chatting_window(QWidget *parent = nullptr);
    void parent_show(QWidget *parent);
    void msg_recv();
    void msg_send();
    void usrin();
    void usrleave();
    void usrmsg();
    enum Msgtype{Usrin,Usrleave,Msg};
    QTcpSocket *tcp;
    ~chatting_window();

private:
    Ui::chatting_window *ui;
};

#endif // CHATTING_WINDOW_H
