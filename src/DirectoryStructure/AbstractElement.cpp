#include "AbstractElement.h"
#include <cstring>


AbstractElement::AbstractElement(char *name, Attributes* attributes, int size) {
    m_name = name;
    m_attributes = attributes;
    this->m_size = size;
}

AbstractElement::~AbstractElement() {
    if(m_name != nullptr){
        delete[] m_name;
    }
    if(m_attributes != nullptr){
        delete m_attributes;
    }
}

/**
 * Returns the name of the Element.
 * @return char* name of the element
 */
char *AbstractElement::getName() {
    return this->m_name;
}

/**
 * Sets the name of the element, if and only if the name follows the convention of the element.
 * @param char* name, name to set
 * @return int error, error indicator: 0 = attribute has been set, 1 = name does not follow the convention, -1 = error
 */
int AbstractElement::setName(char *name) {
    if(testConvention(name) ){
        strcpy(this->m_name, name);
    } else{
        return 1;
    }
    return 0;
}

/**
 * Returns the attributes of an element (as a char array).
 * @return char* attributes, attributes of the element
 */
Attributes* AbstractElement::getAttributes() {
    return this->m_attributes;
}

/**
 * Sets the attributes of an directory.
 * @param char* attributes
 */
void AbstractElement::setAttributes(Attributes* attributes) {
    this->m_attributes = attributes;
}

/**
 * Returns the size of an element.
 * @return int size, size of the element
 */
int AbstractElement::getSize() {
    return this->m_size;
}

/**
 * Sets the Size of a File.
 * @param sizeToSet
 */
void AbstractElement::setSize(int sizeToSet) { //größe des Gesamtspeichers beachten
    this->m_size = sizeToSet;
}