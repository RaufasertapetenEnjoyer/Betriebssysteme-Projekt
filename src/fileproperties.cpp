#include "fileproperties.h"
#include "ui_fileproperties.h"
#include "BSFatStructure/BSFatFile.h"
#include "INodeStructure/INodeFile.h"
#include <string>
#include <sstream>
#include <iostream>

/**
 * @brief name
 */
QString name;

/**
 * @brief size of file as int
 */
int sizeInt;

/**
 * @brief editable
 */
bool editable;

/**
 * @brief system
 */
bool syst;

/**
 * @brief ascii
 */
bool ascii;

/**
 * @brief randAcc
 */
bool randAcc;

/**
 * @brief is Name valid? does not check completely all naming conventions, only if empty String
 */
bool validName=true;

/**
 * @brief is size valid? does not check open memory, only if size is actually a number
 */
bool validSize=true;

/**
 * @brief FileProperties::FileProperties Constructor for AbstractFiles
 * @param QWidget *parent (=mainWindow)
 * @param AbstractFile * file
 * @param std::string path
 * @param int platte (bsFat=1, iNode=2)
 */
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
        ui->indexL->setText(indices);
    }else{
        auto* inode = dynamic_cast<INodeFile*>(file);
        int* indexes = inode->getINode()->getIndexes();
        int length = indexes[0];
        for(int i=1;i<=length;i++){
            indices.append(QString::number(indexes[i]));
            indices.append(", ");
        }
        std::cout<<indices.toStdString()<<std::endl;
        if(indices.length()<=75){
            ui->indexL->setText(indices);
        }else{
            QString subString1 = indices.left(75);
            ui->indexL->setText(subString1);
            QString subString2 = indices.right(indices.length()-75);
            ui->indexL2->setText(subString2);
        }
    }

}

/**
 * @brief FileProperties::FileProperties Constructor for CDRomFiles
 * @param QWidget * parent (=mainWindow)
 * @param CDRomFile * file
 * @param std::string path
 * @param CDRomSimulation * cd
 */
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

/**
 * @brief FileProperties::~FileProperties
 */
FileProperties::~FileProperties()
{
    delete ui;
}

/**
 * @brief updates size on text change
 * @param arg1
 */
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

/**
 * @brief updates name on text change
 * @param arg1
 */
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

/**
 * @brief FileProperties::nameIsNotEmpty
 * @return bool true
 */
bool FileProperties::nameIsNotEmpty(){
    for(int i=0; i<name.size(); i++){
        if(name.at(i) != ' '){
            return true;
        }
    }
    return false;
}

/**
 * @brief updates editable
 * @param arg1
 */
void FileProperties::on_checkBox_stateChanged(int arg1)
{
    editable=arg1;
}

/**
 * @brief updates system
 * @param arg1
 */
void FileProperties::on_checkBox_2_stateChanged(int arg1)
{
    syst=arg1;
}

/**
 * @brief updates ascii
 * @param arg1
 */
void FileProperties::on_checkBox_3_stateChanged(int arg1)
{
    ascii=arg1;
}

/**
 * @brief updates randomAccess
 * @param arg1
 */
void FileProperties::on_checkBox_4_stateChanged(int arg1)
{
    randAcc=arg1;
}

/**
 * @brief FileProperties::getName
 * @return QString name
 */
QString FileProperties::getName(){
    return name;
}

/**
 * @brief FileProperties::getSize
 * @return int size
 */
int FileProperties::getSize(){
    return sizeInt;
}

/**
 * @brief FileProperties::getEditable
 * @return bool editable
 */
bool FileProperties::getEditable(){
    return editable;
}

/**
 * @brief FileProperties::getSystem
 * @return bool system
 */
bool FileProperties::getSystem(){
    return syst;
}

/**
 * @brief FileProperties::getAscii
 * @return bool ascii
 */
bool FileProperties::getAscii(){
    return ascii;
}

/**
 * @brief FileProperties::getRandomAccess
 * @return bool randomAccess
 */
bool FileProperties::getRandomAccess(){
    return randAcc;
}

