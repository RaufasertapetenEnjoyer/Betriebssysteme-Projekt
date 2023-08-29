#include "deletefile.h"
#include "ui_deletefile.h"

/**
 * @brief DeleteFile::DeleteFile
 * @param parent
 */
DeleteFile::DeleteFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeleteFile)
{
    ui->setupUi(this);
}

DeleteFile::~DeleteFile()
{
    delete ui;
}
