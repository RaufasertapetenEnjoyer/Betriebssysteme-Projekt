//
// Created by m.busch on 16.08.2023.
//

#include "CDRomDirectory.h"

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

CDRomDirectory::CDRomDirectory(char *name, Attributes *attributes, int size) : AbstractElementCDRom(name, attributes, size) {
    m_list = nullptr;
}

CDRomDirectory *CDRomDirectory::getParentDirectory() {
    return m_parentDirectory;
}

void CDRomDirectory::setParentDirectory(CDRomDirectory *newParent) {
    m_parentDirectory = newParent;
}

void CDRomDirectory::setLastElementOfTheList(AbstractElementCDRom *element) {
    AbstractElementCDRom* lastElement = getLastElementOfTheList();
    if(lastElement == nullptr){
        m_list = element;
    }else{
        lastElement->setNextElement(element);
    }
    element->setNextElement(nullptr);
}

void CDRomDirectory::createChildElement(AbstractElementCDRom *element) {
    setLastElementOfTheList(element);
}


