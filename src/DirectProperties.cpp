#include "DirectProperties.h"
#include "ui_directProperties.h"

/**
 * @brief dirName
 */
QString dirName;

/**
 * @brief dirEditable
 */
bool dirEditable = false;

/**
 * @brief directProperties::directProperties constructor for direcory
 * @param QWidget *parent (=mainWindow)
 * @param Directoy *dir
 * @param std::string path
 */
directProperties::directProperties(QWidget *parent, Directory *dir, std::string path) :
    QDialog(parent),
    ui(new Ui::directProperties)
{
    ui->setupUi(this);
    ui->nameEd->setText(dir->getName());
    dirName = dir->getName();
    ui->path->setText(QString::fromStdString(path));
    Directory* d = dir->getSubDirectoryList();
    int amountDirs=0;
    while(d!=nullptr){
        amountDirs++;
        d= d->getNextDirectory();
    }
    ui->amountDirs->setText(QString::number(amountDirs));
    int amountFiles = dir->getNumberOfFiles();
    ui->amountFiles->setText(QString::number(amountFiles));
    dirEditable = dir->isEditable();
    ui->checkBox->setChecked(dirEditable);
    std::tm * ptm = std::localtime(&dir->getAttributes()->dateOfCreation);
    char buffer[32];
    // Format: Mo, 15.06.2009 20:20:00
    std::strftime(buffer, 32, "%a, %d.%m.%Y %H:%M:%S", ptm);
    QString string(buffer);
    ui->createdL->setText(string);

    ptm = std::localtime(&dir->getAttributes()->dateOfLastEdit);
    char buff[32];
    // Format: Mo, 15.06.2009 20:20:00
    std::strftime(buff, 32, "%a, %d.%m.%Y %H:%M:%S", ptm);
    QString string2(buff);
    ui->editedL->setText(string2);
}

/**
 * @brief directProperties::directProperties Constructor for CDRomDirectory
 * @param QWidget* parent (=mainWindow)
 * @param CDRomDirectory *dir
 * @param std::string path
 */
directProperties::directProperties(QWidget *parent, CDRomDirectory *dir, std::string path) :
    QDialog(parent),
    ui(new Ui::directProperties)
{
    ui->setupUi(this);
    ui->nameEd->setText(dir->getName());
    ui->nameEd->setEnabled(false);
    dirName = dir->getName();
    ui->path->setText(QString::fromStdString(path));
    int amountFiles = 0;
    int amountDirs=0;
    AbstractElementCDRom * el = dir->getList();
    while(el != nullptr){
        if(instanceof<CDRomFile>(el)){
            amountFiles++;
        }else{
            amountDirs++;
        }
        el = el->getNextElement();
    }
    ui->amountDirs->setText(QString::number(amountDirs));
    ui->amountFiles->setText(QString::number(amountFiles));
    dirEditable = dir->isEditable();
    ui->checkBox->setChecked(dirEditable);
    ui->checkBox->setEnabled(false);
    ui->buttonBox->buttons().at(0)->setText("Ok");

    std::tm * ptm = std::localtime(&dir->getAttributes()->dateOfCreation);
    char buffer[32];
    // Format: Mo, 15.06.2009 20:20:00
    std::strftime(buffer, 32, "%a, %d.%m.%Y %H:%M:%S", ptm);
    QString string(buffer);
    ui->createdL->setText(string);

    ptm = std::localtime(&dir->getAttributes()->dateOfLastEdit);
    char buff[32];
    // Format: Mo, 15.06.2009 20:20:00
    std::strftime(buff, 32, "%a, %d.%m.%Y %H:%M:%S", ptm);
    QString string2(buff);
    ui->editedL->setText(string2);
}
directProperties::~directProperties()
{
    delete ui;
}

/**
 * @brief change name on textchange
 * @param arg1
 */
void directProperties::on_nameEd_textChanged(const QString &arg1)
{
    dirName= arg1;
}

/**
 * @brief change edit on checkboxsignal
 * @param arg1
 */
void directProperties::on_checkBox_stateChanged(int arg1)
{
    dirEditable = arg1;
}

/**
 * @brief directProperties::getName
 * @return QString name
 */
QString directProperties::getName(){
    return dirName;
}

/**
 * @brief directProperties::getEditable
 * @return bool editable
 */
bool directProperties::getEditable(){
    return dirEditable;
}
