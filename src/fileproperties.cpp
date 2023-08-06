#include "fileproperties.h"
#include "ui_fileproperties.h"

FileProperties::FileProperties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileProperties)
{
    ui->setupUi(this);
}

FileProperties::~FileProperties()
{
    delete ui;
}
