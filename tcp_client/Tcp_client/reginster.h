#ifndef REGINSTER_H
#define REGINSTER_H

#include <QWidget>
#include<QTcpSocket>
QT_BEGIN_NAMESPACE
namespace Ui {
class Reginster;
}
QT_END_NAMESPACE
class Reginster : public QWidget
{
    Q_OBJECT

public:
    explicit Reginster(QWidget *parent ,QTcpSocket *tcp);
    void regiter(QTcpSocket *tcp);
    ~Reginster();

private:
    Ui::Reginster *ui;
};

#endif // REGINSTER_H
