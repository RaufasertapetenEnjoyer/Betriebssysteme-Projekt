#include "createdirectory.h"
#include "ui_createdirectory.h"

CreateDirectory::CreateDirectory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateDirectory)
{
    ui->setupUi(this);
}

CreateDirectory::~CreateDirectory()
{
    delete ui;
}
