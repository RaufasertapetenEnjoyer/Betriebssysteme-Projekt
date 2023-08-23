//
// Created by m.busch on 16.08.2023.
//

#include "CDRomFile.h"

CDRomFile::CDRomFile(char *name, Attributes *attributes, int size, int pos) : AbstractElementCDRom(name, attributes, size) {
    m_firstBlockStatusArray = pos;
}

int CDRomFile::getFirstBlock() {
    return m_firstBlockStatusArray;
}
