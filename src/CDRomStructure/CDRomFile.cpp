//
// Created by m.busch on 16.08.2023.
//

#include "CDRomFile.h"

CDRomFile::CDRomFile(char *name, Attributes *attributes, int size, int pos) : AbstractElementCDRom(name, attributes, size) {
    m_firstBlockStatusArray = pos;
}

/**
 * Returns the index of the first block.
 * @return int firstBlock
 */
int CDRomFile::getFirstBlock() {
    return m_firstBlockStatusArray;
<<<<<<< HEAD
}

/**
 * Checks whether the file is editable or not.
 * @return true if file is editable, else false
 */
bool CDRomFile::isEditable() {
    return tstBit(getAttributes()->attributes, 0);
}

/**
 * Initialize the file editable or not.
 * @param bool isEditable
 */
/*void CDRomFile::setEditable(bool isEditable) {
    isEditable ? setBit(getAttributes()->attributes, 0) : clrBit(getAttributes()->attributes, 0);
}*/

/**
 * Checks whether the file is a system file or not.
 * @return true if file is a system file, else false
 */
bool CDRomFile::isSystem() {
    return tstBit(getAttributes()->attributes, 1);
}

/**
 * Initialize the file as a system file or not.
 * @param bool isService
 */
void CDRomFile::setSystem(bool isService) {
    isService ? setBit(getAttributes()->attributes, 1) : clrBit(getAttributes()->attributes, 1);
}

/**
 * Check whether the file is a ascii file or not.
 * @return true if file is a ascii file, else false
 */
bool CDRomFile::isAscii() {
    return tstBit(getAttributes()->attributes, 2);
}

/**
 * Initialize the file as a ascii file or not.
 * @param bool isAscii
 */
void CDRomFile::setAscii(bool isAscii) {
    isAscii ? setBit(getAttributes()->attributes, 2) : clrBit(getAttributes()->attributes, 2);
}

/**
 * Return whether the file is a random-access-memory file or not.
 * @return true if file is an random-access file, else false
 */
bool CDRomFile::isRandAccFile() {
    return tstBit(getAttributes()->attributes, 3);
}

/**
 * Initialize the file as a random-access file or not.
 * @param bool isRandAccFile
 */
void CDRomFile::setRandAccFile(bool isRandAccFile) {
    isRandAccFile ? setBit(getAttributes()->attributes, 3) : clrBit(getAttributes()->attributes, 3);
}

}

