#include "Directory.h"
#include "AbstractFile.h"
#include <string>
#include <cstring>


Directory::Directory(char* name, Attributes* attributes) : AbstractElement(testConvention(name) ? name : new char[12] {'N', 'e','u','e','r','O','r','d','n', 'e', 'r', '\0'}, attributes, 0) {
    this->m_numberOfFiles = 0;//Konstruktor nochmal genauer anschauen
    m_fileList = nullptr;
    m_directoryList = nullptr;
    this->m_numberOfFiles = 0;
}

Directory::~Directory(){
    //AbstractElement::~AbstractElement();

    delete m_fileList;
    delete m_directoryList;
    delete m_nextDirectory;
    delete m_prevDirectory;
    delete m_parentDirectory;

};

/**
 * Returns the parent directory of the current directory.
 * Returns null if the current directory is the root-directory
 * @return Directory* parentDirectory
 */
Directory *Directory::getParentDirectory() {
    return this->m_parentDirectory;
}

/**
 * Sets the parent directory of the current directory
 * @param Directory* newParent
 */
void Directory::setParentDirectory(Directory *newParent) {
    m_parentDirectory = newParent;
}

/**
 * Test whether the name follows the convention of legal directory name or not.
 * @param char* nameToTest
 * @return true, if and only if there are only digits and letters
 */
bool Directory::testConvention(char *nameToTest) {
    int i = 0;
    while(nameToTest[i] != '\0'){
        if (!isalnum(nameToTest[i])){
            return false;
        }
        i++;
    }
    return true;
}

/**
 * Creates a subdirectory and adds it to the double-linked list.
 * @param char* name, name of the directory
 * @param char* attributes, attributes of the directory
 */
void Directory::createChildDirectory(char *name,Attributes *attributes) {
    Directory* directoryToAdd = new Directory(name, attributes);

    Directory* directory = getLastDirectoryOfTheList();
    if(directory == nullptr){
        directoryToAdd->setNextDirectory(nullptr);
        directoryToAdd->setPreviousDirectory(nullptr);
        m_directoryList = directoryToAdd;
    } else{
        directory->m_nextDirectory = directoryToAdd;
        directoryToAdd->m_prevDirectory = directory;
        directoryToAdd->m_nextDirectory = nullptr;
    }
    directoryToAdd->setParentDirectory(this);
}

/**
 * Adds an AbstractFile and adds it to the double-linked list.
 * @param File*
 */
void Directory::createChildFile(AbstractFile *file) {
    setLastFileOfTheList(file);
}

Directory *Directory::getLastDirectoryOfTheList() {
    if(m_directoryList != nullptr){
        Directory* directory = m_directoryList;
        while (directory->m_nextDirectory != nullptr){
            directory = directory->m_nextDirectory;
        }
        return directory;
    } else{
        return nullptr;
    }
}

void Directory::setLastDirectoryOfTheList(Directory* directoryToSet) {
    Directory* directory = m_directoryList;
    while (directory->m_nextDirectory != nullptr){
        directory = directory->m_nextDirectory;
    }
    directory->m_nextDirectory = directoryToSet;
    directoryToSet->m_prevDirectory = directory;
}

AbstractFile *Directory::getLastFileOfTheList(AbstractFile *fileToSet) {
    AbstractFile* file = m_fileList;
    while (file->getNextFile() != nullptr){
        file = file->getNextFile();
    }
    return file;
}

void Directory::setLastFileOfTheList(AbstractFile* fileToSet) {
    if(m_fileList != nullptr){
        AbstractFile* file = m_fileList;
        while (file->getNextFile() != nullptr){
            file = file->getNextFile();
        }
        file->setNextFile(fileToSet);
        fileToSet->setNextFile(nullptr);
        fileToSet->setPrevFile(file);
    }else{
        m_fileList = fileToSet;
        fileToSet->setNextFile(nullptr);
    }
}

int Directory::deleteChildDirectroy(char *name) {
    Directory* directory = m_directoryList;
    while (directory != nullptr){
        if(strcmp(name, directory->getName()) == 0){
            Directory* prevDi = directory->m_prevDirectory;
            Directory* nextDi = directory->m_nextDirectory;
            prevDi->m_nextDirectory = nextDi;
            nextDi->m_prevDirectory = prevDi;
            delete directory;
            return 0;
        }
        directory->m_nextDirectory = directory->m_nextDirectory;
    }
    return 1;
}

int Directory::deleteChildElement(char *name) {
    AbstractFile* file = m_fileList;
    while (file != nullptr){
        if(strcmp(name, file->getName()) == 0){
            AbstractFile* prevDi = file->getPrevFile();
            AbstractFile* nextDi = file->getNextFile();
            prevDi->setNextFile(nextDi);
            nextDi->setPrevFile(prevDi);
            delete file;
            return 0;
        }
        file->setNextFile(file->getNextFile());
    }
    return 1;
}

bool Directory::checkIfDirectoryNameExists(char *name) {
    Directory* directory = m_directoryList;
    while (directory != nullptr){
         if(strcmp(name, directory->getName()) == 0){
             return true;
         }
        directory = directory->m_nextDirectory;
    }
    return false;
}

bool Directory::checkIfFileNameExists(char *name) {
    AbstractFile* file = m_fileList;
    while (file != nullptr){
        if(strcmp(name, file->getName()) == 0){
            return true;
        }
        file = file->getNextFile();
    }
    return false;
}

AbstractFile *Directory::getFileList() {
    return m_fileList;
}

Directory *Directory::getSubDirectoryList() {
    return m_directoryList;
}

Directory *Directory::getNextDirectory() {
    return m_nextDirectory;
}

void Directory::setNextDirectory(Directory *directoryToSet) {
    this->m_nextDirectory = directoryToSet;
}

Directory *Directory::getPrevDirectory() {
    return m_prevDirectory;
}

void Directory::setPreviousDirectory(Directory *directoryToSet) {
    this->m_prevDirectory = directoryToSet;
}





