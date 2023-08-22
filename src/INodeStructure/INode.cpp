//
// Created by jntre on 27.07.2023.
//

#include "math.h"
#include "INode.h"

/**
 *  eine Tabelle hat 12 Blöcke, es sind bis zu 14 Tabellen möglich -> ggf. in numberOfBlocksPerINode setzen
 */
INode::INode(int numberOfBlocksForFile, int numberOfBlocksPerINode) {
    if (numberOfBlocksForFile <= numberOfBlocksPerINode * 14) {
        addressPointers = new int[12];
        firstIndirectPointers = nullptr;
        doubleIndirectPointers = nullptr;
        if (numberOfBlocksForFile > numberOfBlocksPerINode) {
            firstIndirectPointers = new int*[12];
            if (numberOfBlocksForFile > numberOfBlocksPerINode * 2) {
                doubleIndirectPointers = new int**[12];
            }
        }
    } else {
        return; //todo
    }
}

INode::INode() {
    addressPointers = nullptr;
    firstIndirectPointers = nullptr;
    doubleIndirectPointers = nullptr;
}

int *INode::getAddressPointers() const {
    return addressPointers;
}

void INode::setAddressPointers(int *pAddressPointers) {
    INode::addressPointers = pAddressPointers;
}

int **INode::getFirstIndirectPointers() const {
    return firstIndirectPointers;
}

void INode::setFirstIndirectPointers(int **pFirstIndirectPointers) {
    INode::firstIndirectPointers = pFirstIndirectPointers;
}

int ***INode::getDoubleIndirectPointers() const {
    return doubleIndirectPointers;
}

void INode::setDoubleIndirectPointers(int ***pDoubleIndirectPointers) {
    INode::doubleIndirectPointers = pDoubleIndirectPointers;
}

void INode::addAddress(int address, int numberOfBlocksPerINode) {
    int* currentTable = addressPointers;
    int index = 0;
    int currentTableIndex = 0;
    while (currentTable[index] != -1) {
        index++;
        if(index == 12) {
            if(firstIndirectPointers != nullptr) {
                currentTable = *firstIndirectPointers;
            } else {
                // bei Fehler log?
                break;//todo
            }
        }
        if(index >= 24) {
            currentTable = *doubleIndirectPointers[currentTableIndex];
        }
    }
    currentTable[index % 12] = address;
}

void INode::addAddressAtIndex(int address, int numberOfBlocksPerINode, int index) {
    int* table = addressPointers;
    if(index < 12) {
        table[index] = address;
    } else if (index < 24) {
        table = *firstIndirectPointers;
        table[index-12] = address;
    } else if (index < 168) {
        int numberOfTable = (int) floor((index - 24) / 12);
        table = *doubleIndirectPointers[numberOfTable];
        table[index - 12 * (2 + numberOfTable)] = address;
    }
    // bei Fehler log?
}

