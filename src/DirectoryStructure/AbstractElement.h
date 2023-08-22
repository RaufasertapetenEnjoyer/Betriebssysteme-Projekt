
#ifndef BS_ELEMENT_H
#define BS_ELEMENT_H

#include <cstdlib>
#include "cmath"

/**
 * Profs whether a pointer of type T is also convertable to a pointer of type Base.
 * @tparam Base typename , the type you want to check whether it is an instance of
 * @tparam T typename , the current type the pointer is in
 * @param T* ptr, the pointer you want to check
 * @return true, if the type of the pointer is also of type Base
 */
template<typename Base, typename T>
inline bool instanceof(const T* ptr){
    return dynamic_cast<const Base*>(ptr) != nullptr;
}

struct Attributes{
    time_t dateOfCreation;
    time_t dateOfLastEdit;
    char* attributes;
    //int size;
};


class AbstractElement {
public:
    AbstractElement(char* name, Attributes* attributes, int size);
    virtual ~AbstractElement();

    char* getName();
    int setName(char* name);

    Attributes* getAttributes();
    void setAttributes(Attributes* attributes);

    int getSize();
    void setSize(int sizeToSet);

    virtual bool testConvention(char* name) = 0;

    void setBit(char *array, int bitToSet);

    void clrBit(char *array, int bitToClear);

    int tstBit(char *array, int bitToTest);

private:
    /**
     * Name of the element.
     */
    char* m_name;
    /**
     * Attributes of the element. Contains an char array with certain space for flags, the date of creation and the date of the last edit.
     */
    Attributes* m_attributes;
    /**
     * Size of the element
     */
    int m_size;
};

#endif //BS_ELEMENT_H
