
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
    AbstractElement::~AbstractElement();
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

/**
 * Get the last element of the directory list.
 * @return Directory* last directory
 */
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

void Directory::setFileList(AbstractFile *file) {
    this->m_fileList = file;
}

void Directory::setDirectoryList(Directory *directory) {
    this->m_directoryList = directory;
}

unsigned int Directory::getNumberOfFiles(){
    return m_numberOfFiles;
}

void Directory::setNumberOfFiles(unsigned int numberOfFiles){
    m_numberOfFiles = numberOfFiles;
}



