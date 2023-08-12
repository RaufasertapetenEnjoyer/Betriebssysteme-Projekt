#include "fileproperties.h"
#include "ui_fileproperties.h"

/*QString name;
int sizeInt;
bool editable;
bool syst;
bool ascii;
bool randAcc;
bool validName=true;
bool validSize=true;*/

FileProperties::FileProperties(QWidget *parent, AbstractFile *file, std::string path) :
    QDialog(parent),
    ui(new Ui::FileProperties)
{
    ui->setupUi(this);
    ui->fileName->setText( file->getName());
   // name=file->getName();
    ui->fileSize->setText(QString::number(file->getSize()));
    //sizeInt = file->getSize();
    path = path + file->getName();
    ui->path->setText(QString::fromStdString(path));

    ui->checkBox->setChecked(file->isEditable());
    ui->checkBox_2->setChecked(file->isSystem());
    ui->checkBox_3->setChecked(file->isAscii());
    ui->checkBox_4->setChecked(file->isRandAccFile());
    /*editable=file->isEditable();
    syst=file->isSystem();
    ascii=file->isAscii();
    randAcc=file->isRandAccFile();*/

}

FileProperties::~FileProperties()
{
    delete ui;
}

void FileProperties::on_fileSize_textChanged(const QString &arg1)
{

    bool ok;
    /*sizeInt = arg1.toInt(&ok);
    if(!ok || sizeInt == 0){
        validSize = false;

    }else{
        validSize = true;
    }
    if(validName && validSize){
        ui->buttonBox->setEnabled(true);
    }else{
        ui->buttonBox->setEnabled(false);
    }*/
}


void FileProperties::on_fileName_textChanged(const QString &arg1)
{

}


void FileProperties::on_checkBox_stateChanged(int arg1)
{

}


void FileProperties::on_checkBox_2_stateChanged(int arg1)
{

}


void FileProperties::on_checkBox_3_stateChanged(int arg1)
{

}


void FileProperties::on_checkBox_4_stateChanged(int arg1)
{

}

