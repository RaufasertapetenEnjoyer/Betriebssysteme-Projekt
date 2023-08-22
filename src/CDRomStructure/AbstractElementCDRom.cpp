//
// Created by m.busch on 16.08.2023.
//

#include "AbstractElementCDRom.h"

bool AbstractElementCDRom::testConvention(char *name) {
    return true;
}

AbstractElementCDRom *AbstractElementCDRom::getNextElement() {
    return m_nextElement;
}

void AbstractElementCDRom::setNextElement(AbstractElementCDRom *nextElement) {
    m_nextElement = nextElement;
}

AbstractElementCDRom::AbstractElementCDRom(char *name, Attributes *attributes, int size) : AbstractElement(name, attributes, size) {
    m_nextElement = nullptr;
}
