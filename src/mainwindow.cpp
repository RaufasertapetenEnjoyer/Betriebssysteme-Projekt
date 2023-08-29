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
#include "INodeStructure/INodeSimulation.h"
#include "CDRomStructure/CDRomSimulation.h"
#include "mbr.h"
#include <QDialog>
#include <QAction>
#include <iostream>
#include <cstring>

/**
 * @brief int platte, marks the currently chosen simulation (BsFat=1) (InodeSim=2)
 */
int platte;

/**
 * @brief marks if the cd is 'opened' or not
 */
bool cd = false;

/**
 * @brief creates/starts simulations
 */
MBR * mbr = new MBR();
/**
 * @brief assigns bsFatSimulation
 */
BSFatSimulation * bsSim = mbr->getBsFat();

/**
 * @brief assigns INodeSimulatoin
 */
INodeSimulation * inSim = mbr->getINode();

/**
 * @brief creates CDRomSimulation
 */
CDRomSimulation * cdSim = new CDRomSimulation(256, 16384, "Meine CD-Rom", "Songs");

/**
 * @brief the element chosen to copy from cd
 */
AbstractElementCDRom * copyElement = nullptr;

/**
 * @brief currently selected file
 */
AbstractFile * selectedFile;

/**
 * @brief currently selected dir
 */
Directory * selectedDir;

/**
 * @brief currently selected CDFile
 */
CDRomFile * selectedCDFile;

/**
 * @brief creates MainWindow and opens opening Dialog
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Dialog *dia = new Dialog(this,bsSim,inSim);
    if(dia->exec() == QDialog::Accepted){
        platte = dia->getPlatte();
        reload();
    }


}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief (re)loads the page. fills File list, builds directory trees, calculates fragmentation
 */
void MainWindow::reload(){
    ui->listWidget->clear();
    ui->treeWidget_2->clear();
    float f;
    std::cout<<"Platte:" << platte<<std::endl;
    Directory* d;
    std::string pathAsChar = getPath();


    std::cout<<pathAsChar<<std::endl;
    ui->lineEdit->setText(QString::fromStdString(pathAsChar));
    if(cd){
        loadCD();
        CDRomDirectory * cdDir = cdSim->getRootDirectory();
        buildCDTree(cdDir, NULL);
        ui->treeWidget_2->expandAll();
        ui->treeWidget_2->sortItems(0,Qt::AscendingOrder);
        return;
    }
    float fragmentation = 0.0f;
    if(platte == 1){

        bsSim->getFragmentation(bsSim->getRootDirectory(), fragmentation);
        d = bsSim->getCurrentDirectory();
        fragmentation = (fragmentation / (float) bsSim->getNumberOfCurrentlySavedFiles())*100;
        std::cout<<"savedFiles:" << bsSim->getNumberOfCurrentlySavedFiles()<<std::endl;
        if(bsSim->getNumberOfCurrentlySavedFiles()==0){
            fragmentation = 0;
        }
        for (int i = 0; i < bsSim->getNumberOfFilesThatCanBeSaved(); i++) {
            std::cout << bsSim->getStatusArray()[i] << ", ";
        }
    }else if(platte == 2){
        inSim->getFragmentation(inSim->getRootDirectory(),fragmentation);
        d = inSim->getCurrentDirectory();
        fragmentation = (fragmentation / (float) inSim->getNumberOfCurrentlySavedFiles())*100;
        std::cout<<"savedFiles:" << inSim->getNumberOfCurrentlySavedFiles()<<std::endl;
        if(inSim->getNumberOfCurrentlySavedFiles()==0){
            fragmentation = 0;
        }
        for (int i = 0; i < inSim->getNumberOfFilesThatCanBeSaved(); i++) {
            std::cout << inSim->getStatusArray()[i] << ", ";
        }
    }


    std::cout<<"Fragmentierung:" << fragmentation<<std::endl;
    fragmentation = round(fragmentation);
    ui->progressBar->setValue(fragmentation);

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
    Directory * dir;
    if(platte==1){
        dir= bsSim->getRootDirectory();
    }else{
        dir= inSim->getRootDirectory();
    }
    buildTree(dir,NULL);
    CDRomDirectory * cdDir = cdSim->getRootDirectory();
    buildCDTree(cdDir, NULL);
    ui->treeWidget_2->expandAll();
    ui->treeWidget_2->sortItems(0,Qt::AscendingOrder);
    ui->treeWidget->expandAll();
    ui->treeWidget->sortItems(0,Qt::AscendingOrder);
}

/**
 * @brief calculates current path
 * @return std::string path
 */
std::string MainWindow::getPath(){
    std::string path = "";
    Directory* directoryCur;
    if(cd){
        CDRomDirectory * dir = cdSim->getCurrentDirectory();
        while(dir!=nullptr){
            std::string preName = dir->getName();
            path = preName + "/" + path;
            dir = dir->getParentDirectory();
        }
    }else if(platte == 1){
        directoryCur = bsSim->getCurrentDirectory();
        while (directoryCur != nullptr){
            std::string preName = directoryCur->getName();
            path = preName + "/" + path;
            directoryCur = directoryCur->getParentDirectory();
        }
    }else if(platte == 2){
        directoryCur = inSim->getCurrentDirectory();
        while (directoryCur != nullptr){
            std::string preName = directoryCur->getName();
            path = preName + "/" + path;
            directoryCur = directoryCur->getParentDirectory();
        }
    }

    return path;
}

/**
 * @brief recursively builds directory tree for Bsfat or inode simulation
 * @param Directory dir for directory to add to tree
 * @param QTreeWidgetItem for parentItem of directory
 */
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

/**
 * @brief recursively builds directory tree for cd
 * @param CDRomDirectory dir for directory to add to tree
 * @param QTreeWidgetItem for parentItem of directory
 */
void MainWindow::buildCDTree(CDRomDirectory * dir, QTreeWidgetItem *parent){
    QTreeWidgetItem * root;
    QString path;
    if(parent == NULL){
        root = new QTreeWidgetItem(ui->treeWidget_2);
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
    ui->treeWidget_2->addTopLevelItem(root);

    AbstractElementCDRom * element = dir->getList();

    while(element!=nullptr ){
        if(!instanceof<CDRomFile>(element)){
            dir = dynamic_cast<CDRomDirectory*>(element);
            buildCDTree(dir,root);
        }
            element = element->getNextElement();
    }
}

/**
 * @brief opens delete file dialog/deletes file
*/
void MainWindow::deleteFile(){
    bool success = false;
    DeleteFile *deleteDia = new DeleteFile(this);
    if(deleteDia->exec() == QDialog::Accepted){
        if(platte==1){
            success = bsSim->deleteFile(selectedFile);
        }else{
            success = inSim->deleteFile(selectedFile);
        }
        if(success){
           ui->terminal->append("File successfully deleted!");
        }else{
            ui->terminal->append("!!File could not be deleted!!");
        }

        reload();
    }
}

/**
 * @brief opens file-properties dialog/saves file
 */
void MainWindow::fileProperties(){
    FileProperties *prop;
    if(cd){
        prop = new FileProperties(this, selectedCDFile, getPath(), cdSim);
    }else{
        prop = new FileProperties(this, selectedFile, getPath(), platte);
    }
    if(prop->exec() == QDialog::Accepted){
        std::string name = prop->getName().toStdString();
        char* nameP = new char[name.length()];
        strcpy(nameP,name.c_str());

        if(cd){

        }else if(platte==1){

            std::cout<<"Update Values: "<<nameP<<" "<<prop->getSize()<<" "<<prop->getEditable()<<" "<<prop->getSystem()<<" "<<prop->getAscii()<<" "<<prop->getRandomAccess()<<std::endl;
            if(bsSim->checkIfEditIsValid(nameP,prop->getEditable(),prop->getSystem(),prop->getAscii(),prop->getRandomAccess(),selectedFile,prop->getSize())){
                bsSim->updateFile(nameP,prop->getEditable(),prop->getSystem(),prop->getAscii(),prop->getRandomAccess(),selectedFile,prop->getSize());
                ui->terminal->append("File succesfully saved!");
            }else{
                ui->terminal->append(" !! File Edit not accepted or Naming Conventions not followed! File did not save !!");
            }

        }else{
            std::cout<<"Update Values: "<<nameP<<" "<<prop->getSize()<<" "<<prop->getEditable()<<" "<<prop->getSystem()<<" "<<prop->getAscii()<<" "<<prop->getRandomAccess()<<std::endl;
            inSim->updateFile(nameP,prop->getEditable(),prop->getSystem(),prop->getAscii(),prop->getRandomAccess(),selectedFile,prop->getSize());
            ui->terminal->append("File succesfully saved!");
        }
        reload();
    }
}

/**
 * @brief opens create File dialog/saves new file
 */
void MainWindow::createFile(){
   CreateFile *file = new CreateFile(this);
    if(file->exec() == QDialog::Accepted){
        std::string name = file->getName().toStdString();
        char* nameP = new char[name.length()];
        strcpy(nameP,name.c_str());
        if(platte==1){
            bsSim->createFile(nameP, file->getEditable(), file->getSystem(), file->getAscii(), file->getRandomAccess(), file->getSize());
            ui->terminal->append("File succesfully created!");
        }else{
            inSim->createFile(nameP, file->getEditable(), file->getSystem(), file->getAscii(), file->getRandomAccess(), file->getSize());
            ui->terminal->append("File succesfully created!");
        }
        reload();
    }
}

/**
 * @brief opens create directory dialog/ saves new directory
 */
void MainWindow::createDir(){
    CreateDirectory *dir = new CreateDirectory(this);
    if(dir->exec() == QDialog::Accepted){
        std::string name = dir->getName().toStdString();
        char* nameP = new char[name.length()];
        strcpy(nameP,name.c_str());
        Attributes * attributes = new Attributes();
        attributes->dateOfCreation = time(nullptr);
        attributes->dateOfLastEdit = time(nullptr);
        attributes->attributes = new char[1];
        if(platte==1){
            bsSim->createDirectory(nameP, attributes);
        }else{
            inSim->createDirectory(nameP, attributes);
        }
        ui->terminal->append("Directory succesfully created!");
        reload();
    }
}

/**
 * @brief opens delete directory dialog/deletes dir
 */
void MainWindow::deleteDir(){
    deleteDirectory *dir = new deleteDirectory(this);
    if(dir->exec()==Dialog::Accepted){
        bool success = false;
        if(platte==1){
            Directory * toDelete = bsSim->getCurrentDirectory();
            bsSim->setCurrentDirectory(toDelete->getParentDirectory());
            success = bsSim->deleteDirectory(toDelete);
        }else{
            Directory * toDelete = inSim->getCurrentDirectory();
            inSim->setCurrentDirectory(toDelete->getParentDirectory());
            success = inSim->deleteDirectory(toDelete);
        }
        if(success){
            ui->terminal->append("Directory succesfully deleted!");
        }else{
            ui->terminal->append("!!Could not delete Directory!!");
        }
        reload();
    }
}

/**
 * @brief sets directory from cd to copy
 */
void MainWindow::copyDir(){
    if(cd){
     copyElement = cdSim->getCurrentDirectory();
    }
}

/**
 * @brief sets file from cd to copy
 */
void MainWindow::copyFile(){
    if(cd){
     copyElement = selectedCDFile;
    }
}

/**
 * @brief inserts 'copyElement' into bsFat/INode simulation
 */
void MainWindow::insert(){
    if(!cd){
        bool success = false;
        if(platte==1){
            if(instanceof<CDRomFile>(copyElement)){
                CDRomFile* file = dynamic_cast<CDRomFile*>(copyElement);
                success = bsSim->copyCDRomFile(file, cdSim->getBlockSize());
                if(success){
                    ui->terminal->append("Inserted File successfully");
                }
            }else{
                 bsSim->copyCDRomDirectory(dynamic_cast<CDRomDirectory*>(copyElement),cdSim->getBlockSize());
                 ui->terminal->append("Inserted Directory successfully");
            }

        }else{
            if(instanceof<CDRomFile>(copyElement)){
                inSim->copyCDRomFile(dynamic_cast<CDRomFile*>(copyElement),cdSim->getBlockSize());
                if(success){
                    ui->terminal->append("Inserted File successfully");
                }
            }else{
                inSim->copyCDRomDirectory(dynamic_cast<CDRomDirectory*>(copyElement),cdSim->getBlockSize());
                ui->terminal->append("Inserted Directory successfully");
            }
        }

        reload();
    }
}

/**
 * @brief opens directory properties dialog/ saves directory
 */
void MainWindow::dirProperties(){
    Directory* current ;
    if(platte==1){
        current = bsSim->getCurrentDirectory();
    }else{
        current = inSim->getCurrentDirectory();
    }

    directProperties *dir;
    if(cd){
        dir = new directProperties(this,cdSim->getCurrentDirectory(),getPath());
    }else{
        dir = new directProperties(this,current,getPath());
    }
    if(dir->exec() == QDialog::Accepted){
        if(cd){
            return;
        }
        std::string name = dir->getName().toStdString();
        char* nameP = new char[name.length()];
        strcpy(nameP,name.c_str());
        bool success = false;
        if(platte==1){
             success = bsSim->updateDirectory(nameP,dir->getEditable(),current);
        }else{
            success = inSim->updateDirectory(nameP,dir->getEditable(),current);
        }
        if(success){
            ui->terminal->append("Updated Directory successfully");
        }else{
            ui->terminal->append("!!Could not update Directory!!");
        }
        reload();
    }
}

/**
 * @brief if double clicks directory, open it. if double clicked file, open menu
 * @param QListWidgetItem * item clickedItem
 */
void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString content = item->data(Qt::UserRole).toString();
    if(cd){

        AbstractElementCDRom* el = cdSim->getCurrentDirectory()->getList();
        QString name = item->text();
        while(el!=nullptr && el->getName() != name){
            el = el->getNextElement();
        }
        if(instanceof<CDRomFile>(el)){
            selectedFile = nullptr;
            selectedCDFile = dynamic_cast<CDRomFile*>(el);
            QMenu myMenu;
            QAction *fileProp = new QAction("Eigenschaften/Bearbeiten", this);
            connect(fileProp, SIGNAL(triggered()), this, SLOT(fileProperties()));
            myMenu.addAction(fileProp);
            QAction *copy = new QAction("Kopieren", this);
            connect(copy, SIGNAL(triggered()), this, SLOT(copyFile()));
            myMenu.addAction(copy);


            myMenu.exec(QCursor::pos());
        }else{
            CDRomDirectory * dir = dynamic_cast<CDRomDirectory*>(el);
            cdSim->setCurrentDirectory(dir);
            reload();
        }
        return;
    }
    if(content == "File"){
        selectedDir=nullptr;
        selectedCDFile=nullptr;
        QString name = item->text();
        AbstractFile * file;
        if(platte==1){
            file = bsSim->getCurrentDirectory()->getFileList();
        }else{
            file = inSim->getCurrentDirectory()->getFileList();
        }

        while(file != nullptr && file->getName()!=name ){
            file= file->getNextFile();
        }
        if(file==nullptr){
            std::cout<<"Error: File clicked but not found in Simulation"<<std::endl;
            return;
        }
        std::cout<<file->getName()<<std::endl;
        selectedFile=file;
        QMenu myMenu;
        QAction *deleteFile = new QAction("Datei löschen", this);
        connect(deleteFile, SIGNAL(triggered()), this, SLOT(deleteFile()));
        myMenu.addAction(deleteFile);
        QAction *fileProp = new QAction("Eigenschaften/Bearbeiten", this);
        connect(fileProp, SIGNAL(triggered()), this, SLOT(fileProperties()));
        myMenu.addAction(fileProp);

        myMenu.exec(QCursor::pos());
    }
    else{
        std::cout<<"Directory"<<std::endl;
        selectedFile=nullptr;
        Directory* directory;
        if(platte==1){
            directory = bsSim->getCurrentDirectory()->getSubDirectoryList();
        }else{
            directory = inSim->getCurrentDirectory()->getSubDirectoryList();
        }
        if(directory==nullptr){
            std::cout<<"Error: Directory clicked but no directories found in Simulation"<<std::endl;
        }
        QString name = item->text();
        while(directory != nullptr && directory->getName()!=name){
            directory = directory->getNextDirectory();
        }
        selectedDir = directory;
        if(platte==1){
            bsSim->setCurrentDirectory(selectedDir);
        }else{
            inSim->setCurrentDirectory(selectedDir);
        }
        reload();
    }

}

/**
 * @brief opens option-menu
 */
void MainWindow::on_pushButton_2_clicked()
{
    QMenu myMenu;

    if(!cd){
        QAction *createFile = new QAction("Datei erstellen", this);
        connect(createFile, SIGNAL(triggered()), this, SLOT(createFile()));
        myMenu.addAction(createFile);
        QAction *createDir = new QAction("Verzeichnis erstellen", this);
        connect(createDir, SIGNAL(triggered()), this, SLOT(createDir()));
        myMenu.addAction(createDir);
        QAction *deleteDir = new QAction("Verzeichnis löschen", this);
        connect(deleteDir, SIGNAL(triggered()), this, SLOT(deleteDir()));
        myMenu.addAction(deleteDir);
        QAction *insert = new QAction("Einfügen",this);
        connect(insert, SIGNAL(triggered()), this, SLOT(insert()));
        insert->setEnabled(copyElement != nullptr);
        myMenu.addAction(insert);
    }else{
        QAction *copyThisDir = new QAction("Verzeichnis kopieren",this);
        connect(copyThisDir,SIGNAL(triggered()), this, SLOT(copyDir()));
        myMenu.addAction(copyThisDir);
    }
    QAction *dirProp = new QAction("Verzeichnis-Eigenschaften", this);
    connect(dirProp, SIGNAL(triggered()), this, SLOT(dirProperties()));
    myMenu.addAction(dirProp);


    myMenu.exec(QCursor::pos());
}

/**
 * @brief executes defragmentation and recalculates fragmentation
 */
void MainWindow::on_pushButton_clicked()
{
    if(cd){
        return;
    }
    int currPos = 0;
    float f;
    float fragmentation = 0.0f;
    if(platte==1){
        bsSim->defragmentDisk(bsSim->getRootDirectory(), currPos);

        bsSim->getFragmentation(bsSim->getRootDirectory(),fragmentation);
        fragmentation = (fragmentation / (float) bsSim->getNumberOfCurrentlySavedFiles())*100;
        if(bsSim->getNumberOfCurrentlySavedFiles()==0){
            fragmentation = 0;
        }
        for (int i = 0; i < bsSim->getNumberOfFilesThatCanBeSaved(); i++) {
            std::cout << bsSim->getStatusArray()[i] << ", ";
        }
    }else{
        inSim->defragmentDisk(inSim->getRootDirectory(), currPos);
        inSim->getFragmentation(inSim->getRootDirectory(),fragmentation);
        fragmentation = (fragmentation / (float) inSim->getNumberOfCurrentlySavedFiles())*100;
        if(inSim->getNumberOfCurrentlySavedFiles()==0){
            fragmentation = 0;
        }
        for (int i = 0; i < inSim->getNumberOfFilesThatCanBeSaved(); i++) {
            std::cout << inSim->getStatusArray()[i] << ", ";
        }
    }

    std::cout<<"Fragmentierung post de-:" << fragmentation<<std::endl;
    fragmentation = round(fragmentation);
    ui->progressBar->setValue(fragmentation);


}

/**
 * @brief opens parent directory, if present
 */
void MainWindow::on_pushButton_4_clicked()
{
    if(cd){
        if(cdSim->getCurrentDirectory()!= cdSim->getRootDirectory()){
            cdSim->setCurrentDirectory(cdSim->getCurrentDirectory()->getParentDirectory());
            reload();
        }
        return;
    }
    if(platte==1){
        if(bsSim->getCurrentDirectory() != bsSim->getRootDirectory()){
            bsSim->setCurrentDirectory(bsSim->getCurrentDirectory()->getParentDirectory());
            reload();
        }
    }else if(platte == 2){
        if(inSim->getCurrentDirectory() != inSim->getRootDirectory()){
            inSim->setCurrentDirectory(inSim->getCurrentDirectory()->getParentDirectory());
            reload();
        }
    }
}

/**
 * @brief opens clicked directory in list
 * @param item clicked Item
 * @param column column of tree(=0)
 */
void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    cd=false;
    ui->pushButton->setEnabled(true);
    QString path = item->data(0,Qt::UserRole).toString();
    openPath(path);

}

/**
 * @brief reads path and-if possible- opens it
 */
void MainWindow::on_pushButton_5_clicked()
{
        cd=false;
        ui->pushButton->setEnabled(true);
        QString path = ui->lineEdit->text();
        openPath(path);

}

/**
 * @brief opens the path given as QString
 * @param QString path
 */
void MainWindow::openPath(QString path){
    std::cout<<"execute open Path"<<std::endl;
    QStringList p = path.split("/");

    Directory *dir;
    if(platte==1){
        dir = bsSim->getRootDirectory();
    }else if(platte==2){
        dir= inSim->getRootDirectory();
    }
    if(p.at(0)!= dir->getName() || p.at(p.size()-1) != ""){
        std::cout<<"check cd path"<<std::endl;

        openCDPath(path);
        return;
    }
    if(p.size()!=1){
        for(int i =1; i<p.size()-1; i++){
            dir = dir->getSubDirectoryList();
            while(dir != nullptr && dir->getName()!=p.at(i)){
                dir = dir->getNextDirectory();
            }
            if(dir==nullptr){
                ui->terminal->append("path wrong or nonexistent");
                std::cout<<"path wrong or nonexistent"<<std::endl;
            }
        }
        if(dir!=nullptr){
            if(platte==1){
                bsSim->setCurrentDirectory(dir);
            }else{
                inSim->setCurrentDirectory(dir);
            }
            reload();
        }
    }else{
        if(dir->getName() == p.at(1)){
            if(platte==1){
                bsSim->setCurrentDirectory(dir);
            }else{
                inSim->setCurrentDirectory(dir);
            }
            reload();
        }else{
            ui->terminal->append("path wrong or nonexistent");
            std::cout<<"path wrong or nonexistent"<<std::endl;
        }
    }
}

/**
 * @brief opens default dialog to reset/switch simulation
 */
void MainWindow::on_pushButton_3_clicked()
{
    mbr = new MBR();
    std::cout<<"new mbr"<<std::endl;
    Dialog *dia = new Dialog(this,mbr->getBsFat(),mbr->getINode());
    if(dia->exec() == QDialog::Accepted){
           platte = dia->getPlatte();
           if(platte == 1){
                bsSim = mbr->getBsFat();
           }
           else{
                inSim = mbr->getINode();
            }
           reload();
    }
}

/**
 * @brief opens directory from cd
 * @param item clicked item
 * @param column column of tree(=0)
 */
void MainWindow::on_treeWidget_2_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    cd=true;
    ui->pushButton->setEnabled(false);
    QString path = item->data(0,Qt::UserRole).toString();
    openCDPath(path);
}

/**
 * @brief opens path for cd
 * @param path
 */
void MainWindow::openCDPath(QString path){
    QStringList p = path.split("/");

    AbstractElementCDRom * elements = cdSim->getRootDirectory();

    if(p.at(0)!= elements->getName() || p.at(p.size()-1) != ""){
        ui->terminal->append("path wrong or nonexistent");
        std::cout<<"path wrong or nonexistent"<<std::endl;
        return;
    }

    if(p.size()!=1){
        for(int i =1; i<p.size()-1; i++){
            CDRomDirectory * dir =  dynamic_cast<CDRomDirectory*>(elements);
            elements= dir->getList();
            while(elements != nullptr && elements->getName()!=p.at(i)){
                elements = elements->getNextElement();
            }
            if(elements==nullptr){
                ui->terminal->append("path wrong or nonexistent");
                std::cout<<"path wrong or nonexistent"<<std::endl;
            }
        }
        if(elements!=nullptr){
            cdSim->setCurrentDirectory(dynamic_cast<CDRomDirectory*>(elements));
            cd=true;
            ui->pushButton->setEnabled(false);
            reload();
        }
    }else{
        if(elements->getName() == p.at(1)){
            cdSim->setCurrentDirectory(dynamic_cast<CDRomDirectory*>(elements));
            cd=true;
            ui->pushButton->setEnabled(false);
            reload();
        }else{
            ui->terminal->append("path wrong or nonexistent");
            std::cout<<"path wrong or nonexistent"<<std::endl;
        }
    }

}

/**
 * @brief loads cd files/directories into file list
 */
void MainWindow::loadCD(){
    AbstractElementCDRom * element = cdSim->getCurrentDirectory()->getList();
    while(element!=nullptr ){
        if(instanceof<CDRomFile>(element)){
            QListWidgetItem *item = new QListWidgetItem(element->getName());
            item->setIcon(QIcon(":/Icons/fileIcon.png"));
            QString type("File");
            QVariant elType(type);
            item->setData(Qt::UserRole, elType);
            ui->listWidget->addItem(item);
        }else{
            QListWidgetItem *item = new QListWidgetItem(element->getName());
            item->setIcon(QIcon(":/Icons/directoryIcon.png"));
            ui->listWidget->addItem(item);
        }


        element= element->getNextElement();
}

}

