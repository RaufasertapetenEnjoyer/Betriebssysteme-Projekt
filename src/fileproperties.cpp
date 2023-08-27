#include "fileproperties.h"
#include "ui_fileproperties.h"
#include "BSFatStructure/BSFatFile.h"
#include "INodeStructure/INodeFile.h"
#include <string>
#include <sstream>
#include <iostream>

QString name;
int sizeInt;
bool editable;
bool syst;
bool ascii;
bool randAcc;
bool validName=true;
bool validSize=true;

FileProperties::FileProperties(QWidget *parent, AbstractFile *file, std::string path, int platte) :
    QDialog(parent),
    ui(new Ui::FileProperties)
{
    ui->setupUi(this);
    ui->fileName->setText( file->getName());
    name=file->getName();
    ui->fileSize->setText(QString::number(file->getSize()));
    sizeInt = file->getSize();
    path = path + file->getName();
    ui->path->setText(QString::fromStdString(path));


    ui->checkBox->setChecked(file->isEditable());
    ui->checkBox_2->setChecked(file->isSystem());
    ui->checkBox_3->setChecked(file->isAscii());
    ui->checkBox_4->setChecked(file->isRandAccFile());
    editable=file->isEditable();
    syst=file->isSystem();
    ascii=file->isAscii();
    randAcc=file->isRandAccFile();


    std::tm * ptm = std::localtime(&file->getAttributes()->dateOfCreation);
    char buffer[32];
    // Format: Mo, 15.06.2009 20:20:00
    std::strftime(buffer, 32, "%a, %d.%m.%Y %H:%M:%S", ptm);
    QString string(buffer);
    ui->createdL->setText(string);

    ptm = std::localtime(&file->getAttributes()->dateOfLastEdit);
    char buff[32];
    // Format: Mo, 15.06.2009 20:20:00
    std::strftime(buff, 32, "%a, %d.%m.%Y %H:%M:%S", ptm);
    QString string2(buff);
    ui->editedL->setText(string2);

    QString indices("");
    if(platte == 1){
        auto* bsfat = dynamic_cast<BSFatFile*>(file);
        BSCluster* cluster = bsfat->getFirstBlock();
        while(cluster->getNextBlock() != nullptr){
            indices.append(QString::number(cluster->getIndex()));
            indices.append(", ");
            cluster = cluster->getNextBlock();
        }
        indices.append(QString::number(cluster->getIndex()));
    }else{
        auto* inode = dynamic_cast<INodeFile*>(file);
        int* indexes = inode->getINode()->getIndexes();
        int length = indexes[0];
        for(int i=1;i<=length;i++){
            indices.append(QString::number(indexes[i]));
            indices.append(", ");
        }

    }
    ui->indexL->setText(indices);
}

FileProperties::FileProperties(QWidget *parent, CDRomFile *file, std::string path, CDRomSimulation * cd) :
    QDialog(parent),
    ui(new Ui::FileProperties)
{
    ui->setupUi(this);
    ui->fileName->setText( file->getName());
    ui->fileName->setEnabled(false);
    name=file->getName();
    ui->fileSize->setText(QString::number(file->getSize()));
    ui->fileSize->setEnabled(false);
    sizeInt = file->getSize();
    path = path + file->getName();
    ui->path->setText(QString::fromStdString(path));

    ui->checkBox->setChecked(file->isEditable());
    ui->checkBox->setEnabled(false);
    ui->checkBox_2->setChecked(file->isSystem());
    ui->checkBox_2->setEnabled(false);
    ui->checkBox_3->setChecked(file->isAscii());
    ui->checkBox_3->setEnabled(false);
    ui->checkBox_4->setChecked(file->isRandAccFile());
    ui->checkBox_4->setEnabled(false);

    editable=file->isEditable();
    syst=file->isSystem();
    ascii=file->isAscii();
    randAcc=file->isRandAccFile();


    std::tm * ptm = std::localtime(&file->getAttributes()->dateOfCreation);
    char buffer[32];
    // Format: Mo, 15.06.2009 20:20:00
    std::strftime(buffer, 32, "%a, %d.%m.%Y %H:%M:%S", ptm);
    QString string(buffer);
    ui->createdL->setText(string);

    ptm = std::localtime(&file->getAttributes()->dateOfLastEdit);
    char buff[32];
    // Format: Mo, 15.06.2009 20:20:00
    std::strftime(buff, 32, "%a, %d.%m.%Y %H:%M:%S", ptm);
    QString string2(buff);
    ui->editedL->setText(string2);
    ui->buttonBox->buttons().at(0)->setText("Ok");

    QString indeces("");
    int length = ceil((double) file->getSize()/cd->getBlockSize()) + file->getFirstBlock();
    for(int i = file->getFirstBlock(); i < length - 1; i++){
        indeces.append(QString::number(i));
        indeces.append(", ");
    }
    indeces.append(QString::number(length - 1));
    std::cout<<indeces.toStdString()<<std::endl;
    ui->indexL->setText(indeces);
}

FileProperties::~FileProperties()
{
    delete ui;
}

void FileProperties::on_fileSize_textChanged(const QString &arg1)
{

    bool ok;
    sizeInt = arg1.toInt(&ok);
    if(!ok || sizeInt == 0){
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


void FileProperties::on_fileName_textChanged(const QString &arg1)
{
    name= arg1;
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

bool FileProperties::nameIsNotEmpty(){
    for(int i=0; i<name.size(); i++){
        if(name.at(i) != ' '){
            return true;
        }
    }
    return false;
}

void FileProperties::on_checkBox_stateChanged(int arg1)
{
    editable=arg1;
}


void FileProperties::on_checkBox_2_stateChanged(int arg1)
{
    syst=arg1;
}


void FileProperties::on_checkBox_3_stateChanged(int arg1)
{
    ascii=arg1;
}


void FileProperties::on_checkBox_4_stateChanged(int arg1)
{
    randAcc=arg1;
}


QString FileProperties::getName(){
    return name;
}

int FileProperties::getSize(){
    return sizeInt;
}

bool FileProperties::getEditable(){
    return editable;
}
bool FileProperties::getSystem(){
    return syst;
}
bool FileProperties::getAscii(){
    return ascii;
}
bool FileProperties::getRandomAccess(){
    return randAcc;
}


void FileProperties::on_buttonBox_accepted()
{
    //
}

