//
// Created by m.busch on 16.08.2023.
//

#ifndef BS_CDROMFILE_H
#define BS_CDROMFILE_H
#include "AbstractElementCDRom.h"

class CDRomFile: public AbstractElementCDRom{
public:
    CDRomFile(char *name, Attributes *attributes, int size, int pos);

    int getFirstBlock();

    bool isEditable();
    //void setEditable(bool isEditable);

    bool isSystem();
    void setSystem(bool isSystem);

    bool isAscii();
    void setAscii(bool isAscii);

    bool isRandAccFile();
    void setRandAccFile(bool isRandAccFile);
private:

    int m_firstBlockStatusArray;

};


#endif //BS_CDROMFILE_H
