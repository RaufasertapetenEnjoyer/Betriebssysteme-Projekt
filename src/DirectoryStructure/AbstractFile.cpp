

#include "AbstractFile.h"
#include <string>


AbstractFile::AbstractFile(char *name, char *attributes, int size) : AbstractElement(name, attributes, size) {

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
