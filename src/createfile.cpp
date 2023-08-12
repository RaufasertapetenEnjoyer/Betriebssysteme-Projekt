#include "createfile.h"
#include "ui_createfile.h"
#include <iostream>


QString name;
QString fSize;
int sizeAsInt;
bool editable = false;
bool syst = false;
bool ascii = false;
bool randAcc = false;
bool validName=false;
bool validSize=false;

CreateFile::CreateFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateFile)
{
    ui->setupUi(this);
    ui->buttonBox->setEnabled(false);
    name = "";
    fSize = "0";
    sizeAsInt = 0.0;
}

CreateFile::~CreateFile()
{
    delete ui;
}



void CreateFile::on_lineEdit_2_textChanged(const QString &arg1)
{
    fSize = ui->lineEdit_2->text();
    bool ok;
    sizeAsInt = fSize.toInt(&ok);
    if(!ok || sizeAsInt == 0){
        validSize = false;

    }else{
        validSize = true;
    }
    if(validName && validSize){
        ui->buttonBox->setEnabled(true);
    }else{
        ui->buttonBox->setEnabled(false);
    }
}


void CreateFile::on_lineEdit_textChanged(const QString &arg1)
{
    name= ui->lineEdit->text();
    if(name != "" && nameIsNotEmpty()){
        validName = true;
    }else{
        validName = false;
    }

    if(validName && validSize){
        ui->buttonBox->setEnabled(true);
    }else{
        ui->buttonBox->setEnabled(false);
    }
}

bool CreateFile::nameIsNotEmpty(){
    for(int i=0; i<name.size(); i++){
        if(name.at(i) != ' '){
            return true;
        }
    }
    return false;
}

QString CreateFile::getName(){
    return name;
}

int CreateFile::getSize(){
    return sizeAsInt;
}


void CreateFile::on_editable_stateChanged(int arg1)
{
    editable=arg1;
}


void CreateFile::on_system_stateChanged(int arg1)
{
    syst=arg1;
}


void CreateFile::on_ascii_stateChanged(int arg1)
{
    ascii=arg1;
}


void CreateFile::on_checkBox_4_stateChanged(int arg1)
{
    randAcc=arg1;
}
bool CreateFile::getEditable(){
    return editable;
}
bool CreateFile::getSystem(){
    return syst;
}
bool CreateFile::getAscii(){
    return ascii;
}
bool CreateFile::getRandomAccess(){
    return randAcc;
}

