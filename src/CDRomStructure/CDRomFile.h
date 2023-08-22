//
// Created by m.busch on 16.08.2023.
//

#ifndef BS_CDROMFILE_H
#define BS_CDROMFILE_H
#include "AbstractElementCDRom.h"

class CDRomFile: public AbstractElementCDRom{
public:
    CDRomFile(char *name, Attributes *attributes, int size, int pos);

private:

    int m_firstBlockStatusArray;

};


#endif //BS_CDROMFILE_H
