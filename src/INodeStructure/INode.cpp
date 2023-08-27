//
// Created by jntre on 27.07.2023.
//

#include "math.h"
#include "INode.h"
#include "iostream"

/**
 *  eine Tabelle hat 12 Blöcke, es sind bis zu 14 Tabellen möglich -> ggf. in numberOfBlocksPerINode setzen
 */
INode::INode(int numberOfBlocksForFile) {
    addressPointers = new int[12];
    if (numberOfBlocksForFile <= 12 * 14) {
        firstIndirectPointers = nullptr;
        doubleIndirectPointers = nullptr;
        if (numberOfBlocksForFile > 12) {
            firstIndirectPointers = new int*[12];
            if (numberOfBlocksForFile > 12 * 2) {
                int*** emptyArray = new int**[12];
                for (int i = 0; i < 12; i++) {
                    emptyArray[i] = new int*[12];
                }
                doubleIndirectPointers = emptyArray;
            }
        }
        initINode();
    } else {
        return; //todo
    }
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

void INode::addAddress(int address) {
    int* currentTable = addressPointers;
    int index = 0;
    int currentTableIndex = 0;
    while (currentTable[index % 12] != -1) {
        index++;
        if(index == 12) {
            if(firstIndirectPointers != nullptr) {
                currentTable = *firstIndirectPointers;
            } else {
                firstIndirectPointers = new int*[12];
                initTable(*firstIndirectPointers);
                currentTable = *firstIndirectPointers;
            }
        }
        if(index >= 24) {
            if(index >= 12 * 14) {
                std::cout << "File cannot be larger." << std::endl;
                return;
            } else {
                if (doubleIndirectPointers == nullptr) {
                    int ***emptyArray = new int **[12];
                    for (int i = 0; i < 12; i++) {
                        emptyArray[i] = new int *[12];
                    }
                    doubleIndirectPointers = emptyArray;
                }
                currentTable = *doubleIndirectPointers[currentTableIndex];
                currentTableIndex = floor((index - 24) / 12.0);
            }
        }
    }
    currentTable[index % 12] = address;
}

void INode::addAddressAtIndex(int address, int index) {
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
    } else {
        std::cout << "File is too large." << std::endl;
    }
}

void INode::deleteAddressFromIndex(int index) {
    int* currentTable = addressPointers;

    int navigationIndex = 0;
    int compareIndex = 0;
    int tableNumber = -2;

    while(currentTable != nullptr && currentTable[navigationIndex] != -1) {
        if(compareIndex >= index) {
            currentTable[navigationIndex] = -1;
        }
        navigationIndex++;
        compareIndex++;
        if(navigationIndex >= 12) {
            navigationIndex = 0;
            if(tableNumber == -2) {
                tableNumber++;
                currentTable = *firstIndirectPointers;
            } else {
                tableNumber++;
                currentTable = *doubleIndirectPointers[tableNumber];
            }
        }
    }
    detectEmptyTables();
}

void INode::initINode() {

    int tableNumber = -2;
    int* currentTable = addressPointers;

    while (currentTable != nullptr) {
        for (int i = 0; i < 12; i++) {
            currentTable[i] = -1;
        }
        tableNumber++;
        if (tableNumber == -1) {
            currentTable = *firstIndirectPointers;
        } else {
            currentTable = *doubleIndirectPointers[tableNumber];
        }
    }
}

void INode::initTable(int *table) {
    for (int i = 0; i < 12 ; i++) {
        table[i] = -1;
    }
}

void INode::detectEmptyTables() {
    if (*firstIndirectPointers[0] == -1) {
        firstIndirectPointers = nullptr;
    }
    if (*doubleIndirectPointers[0][0] == -1) {
        doubleIndirectPointers = nullptr;
    }
}


