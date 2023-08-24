
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
void Directory::createChildDirectory(char *name, Attributes *attributes) {
    Directory* directoryToAdd = new Directory(name, attributes);
    directoryToAdd->setBit(directoryToAdd->getAttributes()->attributes,0);

    Directory* directory = m_directoryList;
    if(directory == nullptr){
        directoryToAdd->setNextDirectory(nullptr);
        directoryToAdd->setPreviousDirectory(nullptr);
        m_directoryList = directoryToAdd;
    } else{
        directory = getLastDirectoryOfTheList();
        directory->m_nextDirectory = directoryToAdd;
        directoryToAdd->m_prevDirectory = directory;
        directoryToAdd->m_nextDirectory = nullptr;
    }

    directoryToAdd->setParentDirectory(this);
}

/**
 * Adds an AbstractFile and adds it to the double-linked list.
 * @param AbstractFile* fileToAdd
 */
void Directory::createChildFile(AbstractFile *file) {
    setLastFileOfTheList(file);
}

/**
 * Get the last element of the directory list.
 * @return Directory* lastDirectory
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

/**
 * Adds a directory to the end of the double linked list of directories.
 * @param Directory* directoryToSet
 */
void Directory::setLastDirectoryOfTheList(Directory* directoryToSet) {
    Directory* directory = m_directoryList;
    while (directory->m_nextDirectory != nullptr){
        directory = directory->m_nextDirectory;
    }
    directory->m_nextDirectory = directoryToSet;
    directoryToSet->m_prevDirectory = directory;
    directoryToSet->setNextDirectory(nullptr);
}

/**
 * Returns the last file of the double linked list of list.
 * @return Abstractfile* lastFile
 */
AbstractFile *Directory::getLastFileOfTheList() {
    AbstractFile* file = m_fileList;
    while (file->getNextFile() != nullptr){
        file = file->getNextFile();
    }
    return file;
}

/**
 * Adds a file to the end of the double linked list of files.
 * @param AbstractFile* fileToSet
 */
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

/**
 * Checks whether a given name matches any name of directories under the current directory.
 * @param char* name
 * @return true if the name exists, else false
 */
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

/**
 * Checks whether a given name matches any name of files under the current directory.
 * @param char* name
 * @return true if the nam exists, else false
 */
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

/**
 * Get the first file of the file list.
 * @return AbstractFile* firstFile
 */
AbstractFile *Directory::getFileList() {
    return m_fileList;
}

/**
 * Get the first directory of the subdirectory list.
 * @return Directory* firstDirectory
 */
Directory *Directory::getSubDirectoryList() {
    return m_directoryList;
}

/**
 * Return the next directory of the subdirectory list.
 * @return Directory* nextDirectory
 */
Directory *Directory::getNextDirectory() {
    return m_nextDirectory;
}

/**
 * Set the next directory of the subdirectory list.
 * @param Directory* directoryToSet
 */
void Directory::setNextDirectory(Directory *directoryToSet) {
    this->m_nextDirectory = directoryToSet;
}

/**
 * Return the previous directory of the list.
 * @return Director* previousDirectory
 */
Directory *Directory::getPrevDirectory() {
    return m_prevDirectory;
}

/**
 * Set the previous directory of the subdirectory list.
 * @param Directory* directoryToSet
 */
void Directory::setPreviousDirectory(Directory *directoryToSet) {
    this->m_prevDirectory = directoryToSet;
}

/**
 * Set the first element of the file list.
 * @param AbstractFile* file
 */
void Directory::setFileList(AbstractFile *file) {
    this->m_fileList = file;
}

/**
 * Set the first element of the subdirectory list.
 * @param directory
 */
void Directory::setDirectoryList(Directory *directory) {
    this->m_directoryList = directory;
}

/**
 * Return the number of files currently saved in the directory.
 * @return unsigned int numberOfFiles
 */
unsigned int Directory::getNumberOfFiles(){
    return m_numberOfFiles;
}

/**
 * Set the number of files currently saved in the directory.
 * @param numberOfFiles
 */
void Directory::setNumberOfFiles(unsigned int numberOfFiles){
    m_numberOfFiles = numberOfFiles;
}

/**
 * Checks whether the parent directories are editable.
 * @return true if all parents are editable, else false
 */
bool Directory::checkIfParentsAreEditable() {
    Directory* directory = this;
    while (directory != nullptr){
        if(!directory->isEditable()){
            return false;
        }
        directory = directory->getParentDirectory();
    }
    return true;
}

/**
 * Checks whether the directory is editable or not.
 * @return true if its editable, else false
 */
bool Directory::isEditable() {
    return tstBit(getAttributes()->attributes,0);
}

/**
 * Initialize a file as editable or not.
 * @param bool editable
 */
void Directory::setEditable(bool editable) {
    editable ? setBit(getAttributes()->attributes, 0) : clrBit(getAttributes()->attributes, 0);
}
