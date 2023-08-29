#include "createdirectory.h"
#include "ui_createdirectory.h"

/**
 * @brief newDirName
 */
QString newDirName = "";

/**
 * @brief CreateDirectory::CreateDirectory
 * @param parent
 */
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

/**
 * @brief CreateDirectory::getName
 * @return QString name
 */
QString CreateDirectory::getName(){
    return newDirName;
}

/**
 * @brief updates name on textChanged
 * @param arg1
 */
void CreateDirectory::on_lineEdit_textChanged(const QString &arg1)
{
    newDirName = arg1;
    ui->buttonBox->setEnabled(nameIsNotEmpty());

}

/**
 * @brief checks if name is not empty String
 * @return bool isNotEmpty
 */
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

