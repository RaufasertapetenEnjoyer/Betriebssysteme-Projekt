#include "deletedirectory.h"
#include "ui_deletedirectory.h"

deleteDirectory::deleteDirectory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::deleteDirectory)
{
    ui->setupUi(this);
}

deleteDirectory::~deleteDirectory()
{
    delete ui;
}
