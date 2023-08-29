#include "dialog.h"
#include "ui_dialog.h"

/**
 * @brief Dialog::Dialog
 * @param QWidget * parent (=mainWindow)
 * @param BSFatSimulation *bsFat
 * @param INodeSimulation *inSim
 */
Dialog::Dialog(QWidget *parent, BSFatSimulation * bsFat, INodeSimulation * inSim) :
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

    QString inText("");
    inText.append(inSim->getName());
    inText.append(", verfügbarer Speicherplatz: ");
    inText.append(QString::number(inSim->getFreeDiskSpace() * inSim->getBlockSize()));
    inText.append(" Kilobyte");
    ui->toolButton_2->setText(inText);
    setWindowFlags(Qt::Window
        | Qt::WindowMinimizeButtonHint
        | Qt::WindowMaximizeButtonHint);
}

Dialog::~Dialog()
{
    delete ui;
}

/**
 * @brief option 1 is selected
 */
bool op1= false;

/**
 * @brief option 2 is selected
 */
bool op2= false;


/**
 * @brief change selected on button click
 */
void Dialog::on_toolButton_clicked()
{
    ui->toolButton->setStyleSheet("border: 2px solid green");
    ui->toolButton_2->setStyleSheet("");
    op1=true;
    op2=false;
    ui->buttonBox->setDisabled(false);
}

/**
 * @brief change selected on button click
 */
void Dialog::on_toolButton_2_clicked()
{
    ui->toolButton_2->setStyleSheet("border: 2px solid green");
    ui->toolButton->setStyleSheet("");
    op2=true;
    op1=false;
    ui->buttonBox->setDisabled(false);
}

/**
 * @brief Dialog::getPlatte
 * @return chosenPlatte
 */
int Dialog::getPlatte(){
    if(op1){
        return 1;
    }else{
        return 2;
    }
}

