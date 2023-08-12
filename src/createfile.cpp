#include "createfile.h"
#include "ui_createfile.h"
#include <iostream>


QString fName;
QString fSize;
int sizeAsInt;
bool edit = true;
bool sys = false;
bool asc = true;
bool randAc = false;
bool valName=false;
bool valSize=false;

CreateFile::CreateFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateFile)
{
    ui->setupUi(this);
    ui->buttonBox->setEnabled(false);
    fName = "";
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
        valSize = false;

    }else{
        valSize = true;
    }
    if(valName && valSize){
        ui->buttonBox->setEnabled(true);
    }else{
        ui->buttonBox->setEnabled(false);
    }
}


void CreateFile::on_lineEdit_textChanged(const QString &arg1)
{
    fName= ui->lineEdit->text();
    if(fName != "" && nameIsNotEmpty()){
        valName = true;
    }else{
        valName = false;
    }

    if(valName && valSize){
        ui->buttonBox->setEnabled(true);
    }else{
        ui->buttonBox->setEnabled(false);
    }
}

bool CreateFile::nameIsNotEmpty(){
    for(int i=0; i<fName.size(); i++){
        if(fName.at(i) != ' '){
            return true;
        }
    }
    return false;
}

QString CreateFile::getName(){
    return fName;
}

int CreateFile::getSize(){
    return sizeAsInt;
}


void CreateFile::on_editable_stateChanged(int arg1)
{
    edit=arg1;
}


void CreateFile::on_system_stateChanged(int arg1)
{
    sys=arg1;
}


void CreateFile::on_ascii_stateChanged(int arg1)
{
    asc=arg1;
}


void CreateFile::on_checkBox_4_stateChanged(int arg1)
{
    randAc=arg1;
}
bool CreateFile::getEditable(){
    return edit;
}
bool CreateFile::getSystem(){
    return sys;
}
bool CreateFile::getAscii(){
    return asc;
}
bool CreateFile::getRandomAccess(){
    return randAc;
}

