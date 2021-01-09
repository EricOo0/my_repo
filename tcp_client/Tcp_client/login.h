#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QTcpSocket>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    void sign_in();
    void sign_up();
    void msg_recv();
    QTcpSocket *tcp;
    int flag;
    ~Widget();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
