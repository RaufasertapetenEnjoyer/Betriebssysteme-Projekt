#include "AbstractElement.h"
#include <cstring>


AbstractElement::AbstractElement(char *name, Attributes* attributes, int size) {
    m_name = name;
    m_attributes = attributes;
    this->m_size = size;
}

AbstractElement::~AbstractElement() {
    if(m_name != nullptr){
        delete m_name;
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
 * @return int error, error indicator: 0 = attribute has been set, 1 = name does not follow the convention
 */
int AbstractElement::setName(char *name) {
    if(testConvention(name)){
        strcpy(this->m_name, name);
        return 0;
    } else{
        return 1;
    }
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

/**
 * Sets a bit of an char array (0-index-based).
 * @param char* array, array where to set the bit
 * @param int bitToSet, bit which has to be set
 */
void AbstractElement::setBit(char *array, int bitToSet) {
    array[bitToSet/8] = array[bitToSet/8] | ((int) pow(2, (bitToSet % 8)));
}

/**
 * Clears a bit of an char array (0-index-based).
 * @param char* array, array where to clear the bit
 * @param int bitToClear, bit which has to be cleared
 */
void AbstractElement::clrBit(char *array, int bitToClear) {
    array[bitToClear/8] = array[bitToClear/8] & ~((int) pow(2, (bitToClear % 8)));
}

/**
 * Test a bit of an char array (0-index-based).
 * @param char* array, where to test the bit
 * @param int bitToTest, bit which has to be tested
 * @return int bit, the bit it self 0 or 1
 */
int AbstractElement::tstBit(char *array, int bitToTest) {
    return (array[bitToTest/8] & (int) pow(2, (bitToTest % 8))) ? 1 : 0;
}