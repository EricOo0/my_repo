#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    void showwindow();
    void click_on_1(QString name);
    ~Dialog();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
