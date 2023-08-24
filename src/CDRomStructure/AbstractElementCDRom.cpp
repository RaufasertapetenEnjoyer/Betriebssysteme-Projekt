//
// Created by m.busch on 16.08.2023.
//

#include "AbstractElementCDRom.h"

/**
 * Returns the next element of the list.
 * @return AbstractElementCDRom* nextElement
 */
AbstractElementCDRom *AbstractElementCDRom::getNextElement() {
    return m_nextElement;
}

/**
 * Sets the next element to the given element.
 * @param AbstractElementCDRom* nextElement
 */
void AbstractElementCDRom::setNextElement(AbstractElementCDRom *nextElement) {
    m_nextElement = nextElement;
}

/**
 * Creates an object and initializes all parameters.
 * @param char* name
 * @param Attributes* attributes
 * @param int size
 */
AbstractElementCDRom::AbstractElementCDRom(char *name, Attributes *attributes, int size) : AbstractElement(name, attributes, size) {
    m_nextElement = nullptr;
}