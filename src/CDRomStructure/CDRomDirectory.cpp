//
// Created by m.busch on 16.08.2023.
//

#include "CDRomDirectory.h"
#include <QString>

/**
 * Returns the last element of the list.
 * @return AbstractElementCDRom* lastElement
 */
AbstractElementCDRom *CDRomDirectory::getLastElementOfTheList() {
    if(m_list == nullptr){
        return nullptr;
    }

    AbstractElementCDRom* elementCdRom = m_list;
    while(elementCdRom->getNextElement() != nullptr){
        elementCdRom = elementCdRom->getNextElement();
    }
    return elementCdRom;
}

/**
 * Creates the object and initializes all parameters.
 * @param char* name
 * @param Attributes* attributes
 * @param int size
 */
CDRomDirectory::CDRomDirectory(char *name, Attributes *attributes, int size) : AbstractElementCDRom(name, attributes, size) {
    m_list = nullptr;
    m_parentDirectory = nullptr;
}

/**
 * Returns the parent directory of the current directory.
 * @return CDRomDirectory* parent
 */
CDRomDirectory *CDRomDirectory::getParentDirectory() {
    return m_parentDirectory;
}

/**
 * Sets the current parent to the new parent.
 * @param CDRomDirectory* newParent
 */
void CDRomDirectory::setParentDirectory(CDRomDirectory *newParent) {
    m_parentDirectory = newParent;
}

/**
 * Sets the last element of the list to the given (add).
 * @param AbstractElementCDRom* elementToAdd
 */
void CDRomDirectory::setLastElementOfTheList(AbstractElementCDRom *element) {
    AbstractElementCDRom* lastElement = getLastElementOfTheList();
    if(lastElement == nullptr){
        m_list = element;
    }else{
        lastElement->setNextElement(element);
    }
    element->setNextElement(nullptr);
}

/**
 * Creates an child file beneath the current directory.
 * @param AbstractElementCDRom* element
 */
void CDRomDirectory::createChildElement(AbstractElementCDRom *element) {
    setLastElementOfTheList(element);
}

/**
 * Returns the first element of the list of files and directories.
 * @return AbstractElementCDRom* firstElement
 */
AbstractElementCDRom *CDRomDirectory::getList() {
    return m_list;

}

bool CDRomDirectory::isEditable() {
    return tstBit(getAttributes()->attributes, 0);
}

bool CDRomDirectory::testConvention(char *name){
    QString qName(name);
    if(qName.contains('.') || qName.contains(',') || qName.contains(':') || qName.contains('!') || qName.contains('?') ||
       qName.contains('@') || qName.contains('#') || qName.contains('$') || qName.contains('%') || qName.contains('&') ||
       qName.contains('^') || qName.contains('"') || qName.contains('/') || qName.contains('(') || qName.contains(')')){
        return false;

    }
    else{
        return true;
    }
}



