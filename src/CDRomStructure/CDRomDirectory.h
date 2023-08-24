//
// Created by m.busch on 16.08.2023.
//

#ifndef BS_CDROMDIRECTORY_H
#define BS_CDROMDIRECTORY_H
#include "AbstractElementCDRom.h"
#include "CDRomFile.h"

class CDRomDirectory: public AbstractElementCDRom{
public:
    CDRomDirectory(char *name, Attributes *attributes, int size);

    CDRomDirectory* getParentDirectory();

    void setParentDirectory(CDRomDirectory* newParent);

    AbstractElementCDRom* getLastElementOfTheList();

    void setLastElementOfTheList(AbstractElementCDRom* element);

    void createChildElement(AbstractElementCDRom* element);

    AbstractElementCDRom* getList();

    bool isEditable();


private:
    AbstractElementCDRom* m_list;
    CDRomDirectory* m_parentDirectory;
};


#endif //BS_CDROMDIRECTORY_H
