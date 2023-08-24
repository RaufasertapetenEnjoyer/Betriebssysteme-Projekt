#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "BSFatStructure/BSFatSimulation.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr, BSFatSimulation* bsFat = nullptr);
    ~Dialog();
    int getPlatte();

private slots:

    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
