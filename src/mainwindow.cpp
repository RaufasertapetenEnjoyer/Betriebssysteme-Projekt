#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include "deletefile.h"
#include "fileproperties.h"
#include "createfile.h"
#include "createdirectory.h"
#include "deletedirectory.h"
#include "DirectProperties.h"
#include "BSFatStructure/BSFatSimulation.cpp"
#include <QDialog>
#include <QAction>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Dialog *dia = new Dialog(this);
    if(dia->exec() == QDialog::Accepted){
         BSFatSimulation * sim = new BSFatSimulation(8,819200);
         float f = sim->getFragmentation(sim->getCurrentDirectory(),0.1);
         ui->progressBar->setValue(f);
         Directory* d = sim->getRootDirectory();
         char* path =  d->getName();
         ui->lineEdit->setText(path);
        /*QListWidgetItem *item = new QListWidgetItem("test");
        item->setIcon(QIcon(":/Icons/directoryIcon.png"));
        ui->listWidget->addItem(item);*/
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::deleteFile(){
    DeleteFile *deleteDia = new DeleteFile(this);
    deleteDia->open();
}

void MainWindow::fileProperties(){
    FileProperties *prop = new FileProperties(this);
    prop->open();
}

void MainWindow::createFile(){
   CreateFile *file = new CreateFile(this);
    file->open();
}

void MainWindow::createDir(){
    CreateDirectory *dir = new CreateDirectory(this);
    dir->open();
}

void MainWindow::deleteDir(){
    deleteDirectory *dir = new deleteDirectory(this);
    dir->open();
}

void MainWindow::dirProperties(){
    directProperties *dir = new directProperties(this);
    dir->open();
}


void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QMenu myMenu;

    QAction *deleteFile = new QAction("Datei löschen", this);
    connect(deleteFile, SIGNAL(triggered()), this, SLOT(deleteFile()));
    myMenu.addAction(deleteFile);
    QAction *fileProp = new QAction("Eigenschaften", this);
    connect(fileProp, SIGNAL(triggered()), this, SLOT(fileProperties()));
    myMenu.addAction(fileProp);

    myMenu.exec(QCursor::pos());

}




void MainWindow::on_pushButton_2_clicked()
{
    /*QMenu myMenu;

    QAction *createFile = new QAction("Datei erstellen", this);
    connect(createFile, SIGNAL(triggered()), this, SLOT(createFile()));
    myMenu.addAction(createFile);
    QAction *createDir = new QAction("Verzeichnis erstellen", this);
    connect(createDir, SIGNAL(triggered()), this, SLOT(createDir()));
    myMenu.addAction(createDir);
    QAction *deleteDir = new QAction("Verzeichnis löschen", this);
    connect(deleteDir, SIGNAL(triggered()), this, SLOT(deleteDir()));
    myMenu.addAction(deleteDir);
    QAction *dirProp = new QAction("Eigenschaften", this);
    connect(dirProp, SIGNAL(triggered()), this, SLOT(dirProperties()));
    myMenu.addAction(dirProp);

    myMenu.exec(QCursor::pos());*/



}

