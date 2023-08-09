#include "AbstractFile.h"
#include <string>



AbstractFile::AbstractFile(char *name, Attributes *attributes, int size) : AbstractElement(name, attributes, size) {
    m_nextFile = nullptr;
    m_prevFile = nullptr;
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

AbstractFile *AbstractFile::getNextFile() {
    return m_nextFile;
}

void AbstractFile::setNextFile(AbstractFile *fileToSet) {
    this->m_nextFile = fileToSet;
}

AbstractFile *AbstractFile::getPrevFile() {
    return m_prevFile;
}

void AbstractFile::setPrevFile(AbstractFile *fileToSet) {
    this->m_prevFile = fileToSet;
}

AbstractFile::~AbstractFile() {
    //AbstractElement::~AbstractElement();
}

bool AbstractFile::isEditable() {
    return tstBit(getAttributes()->attributes, 0);
}

void AbstractFile::setEditable(bool isEditable) {
    isEditable ? setBit(getAttributes()->attributes, 0) : clrBit(getAttributes()->attributes, 0);
}

bool AbstractFile::isSystem() {
    return tstBit(getAttributes()->attributes, 1);
}

void AbstractFile::setSystem(bool isService) {
    isService ? setBit(getAttributes()->attributes, 1) : clrBit(getAttributes()->attributes, 1);
}

bool AbstractFile::isAscii() {
    return tstBit(getAttributes()->attributes, 2);
}

void AbstractFile::setAscii(bool isAscii) {
    isAscii ? setBit(getAttributes()->attributes, 2) : clrBit(getAttributes()->attributes, 2);
}

bool AbstractFile::isRandAccFile() {
    return tstBit(getAttributes()->attributes, 3);
}

void AbstractFile::setRandAccFile(bool isRandAccFile) {
    isRandAccFile ? setBit(getAttributes()->attributes, 3) : clrBit(getAttributes()->attributes, 3);
}
