#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent, BSFatSimulation * bsFat) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QString bsFatText("");
    bsFatText.append(bsFat->getName());
    bsFatText.append(", verfügbarer Speicherplatz: ");
    bsFatText.append(QString::number(bsFat->getFreeDiskSpace() * bsFat->getBlockSize()));
    bsFatText.append(" Kilobyte");
    ui->toolButton->setText(bsFatText);
    setWindowFlags(Qt::Window
        | Qt::WindowMinimizeButtonHint
        | Qt::WindowMaximizeButtonHint);
}

Dialog::~Dialog()
{
    delete ui;
}


bool op1= false;
bool op2= false;



void Dialog::on_toolButton_clicked()
{
    ui->toolButton->setStyleSheet("border: 2px solid green");
    ui->toolButton_2->setStyleSheet("");
    op1=true;
    op2=false;
    ui->buttonBox->setDisabled(false);
}


void Dialog::on_toolButton_2_clicked()
{
    ui->toolButton_2->setStyleSheet("border: 2px solid green");
    ui->toolButton->setStyleSheet("");
    op2=true;
    op1=false;
    ui->buttonBox->setDisabled(false);
}

int Dialog::getPlatte(){
    if(op1){
        return 1;
    }else{
        return 2;
    }
}

