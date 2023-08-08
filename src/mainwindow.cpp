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

BSFatSimulation * sim = new BSFatSimulation(512,16384*4);
AbstractFile * selectedFile;
Directory * selectedDir;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Dialog *dia = new Dialog(this);
    if(dia->exec() == QDialog::Accepted){
           reload();
    }


}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reload(){
    ui->listWidget->clear();
    float f = sim->getFragmentation(sim->getRootDirectory(),0.0);
    std::cout<<"Fragmentierung:" << f<<std::endl;
    ui->progressBar->setValue(f);
    Directory* d = sim->getCurrentDirectory();
    //d = d->getSubDirectoryList();
    char* path =  d->getName();
    ui->lineEdit->setText(path);

    for (int i = 0; i < sim->getNumberOfFilesThatCanBeSaved(); i++) {
            std::cout << sim->getStatusArray()[i] << ", ";
        }
        std::cout << std::endl;
    AbstractFile * file = d->getFileList();
    while(file != nullptr){
        QListWidgetItem *item = new QListWidgetItem(file->getName());
        item->setIcon(QIcon(":/Icons/fileIcon.png"));
        QString type("File");
        QVariant elType(type);
        item->setData(Qt::UserRole, elType);
        ui->listWidget->addItem(item);
        file=file->getNextFile();
    }
    Directory * directory =d->getSubDirectoryList();
    while(directory != nullptr){
        QListWidgetItem *item = new QListWidgetItem(directory->getName());
        item->setIcon(QIcon(":/Icons/directoryIcon.png"));
        ui->listWidget->addItem(item);
        directory = directory->getNextDirectory();
    }
    ui->listWidget->sortItems();

    ui->treeWidget->clear();
    Directory * dir = sim->getRootDirectory();
    buildTree(dir,NULL);
    ui->treeWidget->expandAll();
    ui->treeWidget->sortItems(0,Qt::AscendingOrder);
}

void MainWindow::buildTree(Directory* dir,QTreeWidgetItem *parent){

    QTreeWidgetItem * root;
    QString path;
    if(parent == NULL){
        root = new QTreeWidgetItem(ui->treeWidget);
        path = dir->getName();
        path += "/";
        root->setData(0,Qt::UserRole,QVariant(path));
    }
    else{
        path = parent->data(0,Qt::UserRole).toString();
        path += dir->getName();
        path += "/";
        root = new QTreeWidgetItem(parent);
        root->setData(0,Qt::UserRole,QVariant(path));
    }
    root->setText(0,dir->getName());
    root->setIcon(0,QIcon(":/Icons/directoryIcon.png"));
    ui->treeWidget->addTopLevelItem(root);

    dir=dir->getSubDirectoryList();

    while(dir!=nullptr){
        buildTree(dir,root);
        dir=dir->getNextDirectory();
    }

}

void MainWindow::deleteFile(){
    DeleteFile *deleteDia = new DeleteFile(this);
    if(deleteDia->exec() == QDialog::Accepted){
        //sim->deleteFile();
        std::cout<<"delete File: "<<selectedFile->getName()<<std::endl;
        reload();
    }
}

void MainWindow::fileProperties(){
    FileProperties *prop = new FileProperties(this, selectedFile);
    if(prop->exec() == QDialog::Accepted){
        std::cout<<"save File: "<<selectedFile->getName()<<std::endl;
        reload();
    }
}

void MainWindow::createFile(){
   CreateFile *file = new CreateFile(this);
    if(file->exec() == QDialog::Accepted){

    }
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
    QString content = item->data(Qt::UserRole).toString();
    if(content == "File"){
        selectedDir=nullptr;
        QString name = item->text();
        AbstractFile * file = sim->getCurrentDirectory()->getFileList();//to change to just current Directory
        if(file==nullptr){
            std::cout<<"Error: File clicked but no files found in Simulation"<<std::endl;
        }
        while(file != nullptr && file->getName()!=name ){
            file= file->getNextFile();
        }
        std::cout<<file->getName()<<std::endl;
        selectedFile=file;
        QMenu myMenu;
        QAction *deleteFile = new QAction("Datei löschen", this);
        connect(deleteFile, SIGNAL(triggered()), this, SLOT(deleteFile()));
        myMenu.addAction(deleteFile);
        QAction *fileProp = new QAction("Eigenschaften", this);
        connect(fileProp, SIGNAL(triggered()), this, SLOT(fileProperties()));
        myMenu.addAction(fileProp);

        myMenu.exec(QCursor::pos());
    }
    else{
        std::cout<<"Directory"<<std::endl;
        selectedFile=nullptr;
        Directory* directory = sim->getCurrentDirectory()->getSubDirectoryList();
        if(directory==nullptr){
            std::cout<<"Error: Directory clicked but no directories found in Simulation"<<std::endl;
        }
        QString name = item->text();
        while(directory != nullptr && directory->getName()!=name){
            directory = directory->getNextDirectory();
        }
        selectedDir = directory;

        sim->setCurrentDirectory(selectedDir);
        reload();
    }

}




void MainWindow::on_pushButton_2_clicked()
{
    QMenu myMenu;

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

    myMenu.exec(QCursor::pos());



}


void MainWindow::on_pushButton_clicked()
{
    float f = sim->getFragmentation(sim->getRootDirectory(),0.0);
    std::cout<<"Fragmentierung:" << f<<std::endl;
    ui->progressBar->setValue(f);

    for (int i = 0; i < sim->getNumberOfFilesThatCanBeSaved(); i++) {
            std::cout << sim->getStatusArray()[i] << ", ";
        }
        std::cout << std::endl;
}


void MainWindow::on_pushButton_4_clicked()
{
    if(sim->getCurrentDirectory() != sim->getRootDirectory()){
        sim->setCurrentDirectory(sim->getCurrentDirectory()->getParentDirectory());

        reload();
    }
}


void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QString path = item->data(0,Qt::UserRole).toString();
    openPath(path);

}


void MainWindow::on_pushButton_5_clicked()
{
    QString path = ui->lineEdit->text();
    openPath(path);
}

void MainWindow::openPath(QString path){
    QStringList p = path.split("/");

    Directory *dir = sim->getRootDirectory();
    if(p.at(0)!= dir->getName() || p.at(p.size()-1) != ""){
        std::cout<<"path wrong or nonexistent"<<std::endl;
        return;
    }
    if(p.size()!=1){
        for(int i =1; i<p.size()-1; i++){
            dir = dir->getSubDirectoryList();
            while(dir != nullptr && dir->getName()!=p.at(i)){
                dir = dir->getNextDirectory();
            }
            if(dir==nullptr){
                std::cout<<"path wrong or nonexistent"<<std::endl;
            }
        }
        if(dir!=nullptr){
            sim->setCurrentDirectory(dir);
            reload();
        }
    }else{
        if(dir->getName() == p.at(1)){
            sim->setCurrentDirectory(dir);
            reload();
        }else{
            std::cout<<"path wrong or nonexistent"<<std::endl;
        }
    }
}
