#include "createfile.h"
#include "ui_createfile.h"
#include <iostream>

/**
 * @brief fName
 */
QString fName;

/**
 * @brief fSize
 */
QString fSize;

/**
 * @brief sizeAsInt
 */
int sizeAsInt;

/**
 * @brief edit
 */
bool edit = true;

/**
 * @brief sys
 */
bool sys = false;

/**
 * @brief asc
 */
bool asc = true;

/**
 * @brief randAc
 */
bool randAc = false;

/**
 * @brief valName
 */
bool valName=false;

/**
 * @brief valSize
 */
bool valSize=false;


/**
 * @brief initializes
 * @param parent
 */
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


/**
 * @brief change size on textChange
 * @param arg1
 */
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

/**
 * @brief change name on textChange
 * @param arg1
 */
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

/**
 * @brief check if name is not empty string
 * @return bool isNotEmpty
 */
bool CreateFile::nameIsNotEmpty(){
    for(int i=0; i<fName.size(); i++){
        if(fName.at(i) != ' '){
            return true;
        }
    }
    return false;
}

/**
 * @brief CreateFile::getName
 * @return QString name
 */
QString CreateFile::getName(){
    return fName;
}

/**
 * @brief CreateFile::getSize
 * @return int size
 */
int CreateFile::getSize(){
    return sizeAsInt;
}

/**
 * @brief set editable on checkbox change
 * @param arg1
 */
void CreateFile::on_editable_stateChanged(int arg1)
{
    edit=arg1;
}

/**
 * @brief set system on checkbox change
 * @param arg1
 */
void CreateFile::on_system_stateChanged(int arg1)
{
    sys=arg1;
}

/**
 * @brief set ascii on checkbox change
 * @param arg1
 */
void CreateFile::on_ascii_stateChanged(int arg1)
{
    asc=arg1;
}

/**
 * @brief set randomAccess on checkbox change
 * @param arg1
 */
void CreateFile::on_checkBox_4_stateChanged(int arg1)
{
    randAc=arg1;
}

/**
 * @brief CreateFile::getEditable
 * @return bool editable
 */
bool CreateFile::getEditable(){
    return edit;
}

/**
 * @brief CreateFile::getSystem
 * @return bool system
 */
bool CreateFile::getSystem(){
    return sys;
}

/**
 * @brief CreateFile::getAscii
 * @return bool ascii
 */
bool CreateFile::getAscii(){
    return asc;
}

/**
 * @brief CreateFile::getRandomAccess
 * @return bool randomAccess
 */
bool CreateFile::getRandomAccess(){
    return randAc;
}

