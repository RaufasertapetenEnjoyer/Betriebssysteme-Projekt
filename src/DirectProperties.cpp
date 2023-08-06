#include "DirectProperties.h"
#include "ui_directProperties.h"

directProperties::directProperties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::directProperties)
{
    ui->setupUi(this);
}

directProperties::~directProperties()
{
    delete ui;
}
