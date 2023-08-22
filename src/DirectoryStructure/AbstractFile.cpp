#include "AbstractFile.h"
#include <string>

/**
 * Create the object and initialize the name, attributes and the size. Next and previous element pointing on null.
 * @param name
 * @param attributes
 * @param size
 */
AbstractFile::AbstractFile(char *name, Attributes *attributes, int size) : AbstractElement(name, attributes, size) {
    m_nextFile = nullptr;
    m_prevFile = nullptr;
}

/**
 * Destruct the file.
 */
AbstractFile::~AbstractFile() {
    //AbstractElement::~AbstractElement();
}

/**
 * Test whether the name follows the convention of legal file name or not.
 * @param char* nameToTest
 * @return bool
 */
bool AbstractFile::testConvention(char *name) {
    int dot = 0;
    int i = 0;
    while (name[i] != '\0'){
        if(i == 0 && !isalnum(name[i])){
            return false;
        }
        if(dot != 0 && !isalpha(name[i])){
            if(name[i] != '.'){
                return false;
            }
        }
        if(name[i] == '.'){
            if(name[i+1] == '.' || !isalnum(name[i+1])){
                return false;
            }
            dot++;
        }
        i++;
    }
    return true;
}

/**
 * Return the next file of the list.
 * @return AbstractFile* nextFile
 */
AbstractFile *AbstractFile::getNextFile() {
    return m_nextFile;
}

/**
 * Set the next file of the list.
 * @param AbstractFile* fileToSet
 */
void AbstractFile::setNextFile(AbstractFile *fileToSet) {
    this->m_nextFile = fileToSet;
}

/**
 * Return the previous file of the list.
 * @return
 */
AbstractFile *AbstractFile::getPrevFile() {
    return m_prevFile;
}

/**
 * Set the previous file of the list.
 * @param AbstractFile* fileToSet
 */
void AbstractFile::setPrevFile(AbstractFile *fileToSet) {
    this->m_prevFile = fileToSet;
}

/**
 * Checks whether the file is editable or not.
 * @return true if file is editable, else false
 */
bool AbstractFile::isEditable() {
    return tstBit(getAttributes()->attributes, 0);
}

/**
 * Initialize the file editable or not.
 * @param bool isEditable
 */
void AbstractFile::setEditable(bool isEditable) {
    isEditable ? setBit(getAttributes()->attributes, 0) : clrBit(getAttributes()->attributes, 0);
}

/**
 * Checks whether the file is a system file or not.
 * @return true if file is a system file, else false
 */
bool AbstractFile::isSystem() {
    return tstBit(getAttributes()->attributes, 1);
}

/**
 * Initialize the file as a system file or not.
 * @param bool isService
 */
void AbstractFile::setSystem(bool isService) {
    isService ? setBit(getAttributes()->attributes, 1) : clrBit(getAttributes()->attributes, 1);
}

/**
 * Check whether the file is a ascii file or not.
 * @return true if file is a ascii file, else false
 */
bool AbstractFile::isAscii() {
    return tstBit(getAttributes()->attributes, 2);
}

/**
 * Initialize the file as a ascii file or not.
 * @param bool isAscii
 */
void AbstractFile::setAscii(bool isAscii) {
    isAscii ? setBit(getAttributes()->attributes, 2) : clrBit(getAttributes()->attributes, 2);
}

/**
 * Return whether the file is a random-access-memory file or not.
 * @return true if file is an random-access file, else false
 */
bool AbstractFile::isRandAccFile() {
    return tstBit(getAttributes()->attributes, 3);
}

/**
 * Initialize the file as a random-access file or not.
 * @param bool isRandAccFile
 */
void AbstractFile::setRandAccFile(bool isRandAccFile) {
    isRandAccFile ? setBit(getAttributes()->attributes, 3) : clrBit(getAttributes()->attributes, 3);
}