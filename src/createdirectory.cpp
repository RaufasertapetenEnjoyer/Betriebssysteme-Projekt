#include "createdirectory.h"
#include "ui_createdirectory.h"

QString newDirName = "";

CreateDirectory::CreateDirectory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateDirectory)
{
    ui->setupUi(this);
    ui->buttonBox->setEnabled(false);
}

CreateDirectory::~CreateDirectory()
{
    delete ui;
}

QString CreateDirectory::getName(){
    return newDirName;
}

void CreateDirectory::on_lineEdit_textChanged(const QString &arg1)
{
    newDirName = arg1;
    ui->buttonBox->setEnabled(nameIsNotEmpty());

}

bool CreateDirectory::nameIsNotEmpty(){
    if(newDirName.size()==0){
        return false;
    }
    for(int i=0; i<newDirName.size(); i++){
        if(newDirName.at(i) != ' '){
            return true;
        }
    }
    return false;
}

