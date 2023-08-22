//
// Created by m.busch on 16.08.2023.
//

#ifndef BS_ABSTRACTELEMENTCDROM_H
#define BS_ABSTRACTELEMENTCDROM_H


#include "../DirectoryStructure/AbstractElement.h"

class AbstractElementCDRom: public AbstractElement{
public:
    AbstractElementCDRom(char *name, Attributes *attributes, int size);
    bool testConvention(char *name) override;
    AbstractElementCDRom* getNextElement();
    void setNextElement(AbstractElementCDRom* nextElement);
private:
    AbstractElementCDRom* m_nextElement;
};


#endif //BS_ABSTRACTELEMENTCDROM_H
