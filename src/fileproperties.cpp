#include "fileproperties.h"
#include "ui_fileproperties.h"

FileProperties::FileProperties(QWidget *parent, AbstractFile *file) :
    QDialog(parent),
    ui(new Ui::FileProperties)
{
    ui->setupUi(this);
    ui->fileName->setText( file->getName());
    ui->fileSize->setText(QString::number(file->getSize()));
    Attributes * attribute = file->getAttributes();

}

FileProperties::~FileProperties()
{
    delete ui;
}
