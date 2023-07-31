
#ifndef BS_ELEMENT_H
#define BS_ELEMENT_H

#include <cstdlib>

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

    void setBit(char* array, int bitToSet);
    void clrBit(char* array, int bitToClear);
    int tstBit(char* array, int bitToTest);

    virtual bool testConvention(char* name) = 0;

private:
    char* m_name;
    Attributes* m_attributes;
    int m_size;
    const int m_lengthAttributesArray = 1;
};


#endif //BS_ELEMENT_H
