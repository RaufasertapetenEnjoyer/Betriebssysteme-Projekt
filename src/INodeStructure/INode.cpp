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
    if (numberOfBlocksForFile <= 12 * 14) {
        m_firstIndirectPointers = nullptr;
        m_secondIndirectPointers = nullptr;
        if (numberOfBlocksForFile > 12) {
            m_firstIndirectPointers = new int[12];
            if (numberOfBlocksForFile > 24) {
                int** emptyArray = new int*[12];
                for (int i = 0; i < 12; i++) {
                    int* anotherNewTable = new int[12];
                    emptyArray[i] = anotherNewTable;
                }
                m_secondIndirectPointers = emptyArray;
            }
        }
        initINode();
    } else {
        return;
    }
}

int* INode::getAddressPointers() {
    return m_addressPointers;
}

int *INode::getFirstIndirectPointers() const {
    return m_firstIndirectPointers;
}

int **INode::getDoubleIndirectPointers() const {
    return m_secondIndirectPointers;
}

void INode::addAddress(int address) {
    int* currentTable = m_addressPointers;
    int index = 0;
    int currentTableIndex = 0;
    while (currentTable[index % 12] != -1) {
        index++;
        if(index == 12) {
            if(m_firstIndirectPointers == nullptr) {
                int* newTable = new int[12];
                m_firstIndirectPointers = newTable;
                initTable(m_firstIndirectPointers);
            }
                currentTable = m_firstIndirectPointers;
        }
        if(index >= 24) {
            if(index >= 12 * 14) {
                std::cout << "File cannot be larger." << std::endl;
                return;
            } else {
                if (m_secondIndirectPointers == nullptr) {
                    m_secondIndirectPointers = new int*[12];
                    for (int i = 0; i < 12; i++) {
                        int* anotherNewTable = new int[12];
                        m_secondIndirectPointers[i] = anotherNewTable;
                    }
                }
                currentTable = m_secondIndirectPointers[currentTableIndex];
                currentTableIndex = floor((index - 24) / 12.0);
            }
        }
    }
    currentTable[index % 12] = address;
}

void INode::addAddressAtIndex(int address, int index) {
    int* table = m_addressPointers;
    if(index < 12) {
        table[index] = address;
    } else if (index < 24 && m_firstIndirectPointers != nullptr) {
        table = m_firstIndirectPointers;
        table[index-12] = address;
    } else if (index < 168 && m_secondIndirectPointers != nullptr) {
        int numberOfTable = (int) floor((index - 24) / 12);
        table = m_secondIndirectPointers[numberOfTable];
        table[index - 12 * (2 + numberOfTable)] = address;
    } else if (index >= 168){
        std::cout << "File is too large." << std::endl;
    }
}

void INode::deleteAddressFromIndex(int index) {
    int* currentTable = m_addressPointers;

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
            if(tableNumber == -2 && m_firstIndirectPointers != nullptr) {
                tableNumber++;
                currentTable = m_firstIndirectPointers;
            } else if(m_secondIndirectPointers != nullptr){
                tableNumber++;
                currentTable = m_secondIndirectPointers[tableNumber];
            } else {
                currentTable = nullptr;
            }
        }
    }
    detectEmptyTables();
}

void INode::initINode() {

    int tableNumber = -2;
    int* currentTable = m_addressPointers;

    while (currentTable != nullptr) {
        for (int i = 0; i < 12; i++) {
            currentTable[i] = -1;
        }
        tableNumber++;
        if (tableNumber == -1 && m_firstIndirectPointers != nullptr) {
            currentTable = m_firstIndirectPointers;
        } else if(m_secondIndirectPointers != nullptr) {
            currentTable = m_secondIndirectPointers[tableNumber];
        }else{
            currentTable = nullptr;
        }
    }
}

void INode::initTable(int *table) {
    for (int i = 0; i < 12 ; i++) {
        table[i] = -1;
    }
}

void INode::detectEmptyTables() {
    if (m_firstIndirectPointers[0] == -1) {
        m_firstIndirectPointers = nullptr;
    }
    if (m_secondIndirectPointers[0][0] == -1) {
        m_secondIndirectPointers = nullptr;
    }
}

void INode::findAndReplaceAddress(int oldAddress, int newAddress) {
    int* currentTable = m_addressPointers;

    int navigationIndex = 0;
    int tableNumber = -2;

    while(currentTable != nullptr && currentTable[navigationIndex] != -1) {
        if (currentTable[navigationIndex] == oldAddress) {
            currentTable[navigationIndex] = newAddress;
            return;
        }
        navigationIndex++;
        if(navigationIndex >= 12) {
            navigationIndex = 0;
            if(tableNumber == -2 && m_firstIndirectPointers != nullptr) {
                tableNumber++;
                currentTable = m_firstIndirectPointers;
            } else if(m_secondIndirectPointers != nullptr){
                tableNumber++;
                currentTable = m_secondIndirectPointers[tableNumber];
            } else {
                currentTable = nullptr;
            }
        }
    }
}

bool INode::checkIfAddressExists(int address) {
    int* currentTable = m_addressPointers;

    int navigationIndex = 0;
    int tableNumber = -2;

    while(currentTable != nullptr && currentTable[navigationIndex] != -1) {
        if (currentTable[navigationIndex] == address) {
            return true;
        }
        navigationIndex++;
        if(navigationIndex >= 12) {
            navigationIndex = 0;
            if(tableNumber == -2 && m_firstIndirectPointers != nullptr) {
                tableNumber++;
                currentTable = m_firstIndirectPointers;
            } else if(m_secondIndirectPointers != nullptr){
                tableNumber++;
                currentTable = m_secondIndirectPointers[tableNumber];
            } else {
                currentTable = nullptr;
            }
        }
    }
    return false;
}

int *INode::getIndexes() {
    int numberOfBlocks = getNumberOfBlocks();
    int* statusIndexes = new int[numberOfBlocks + 1];
    statusIndexes[0] = numberOfBlocks;
    int* currentTable = m_addressPointers;

    int navigationIndex = 0;
    int index = 0;
    int tableNumber = -2;

    while(currentTable != nullptr && currentTable[navigationIndex] != -1) {
        statusIndexes[index + 1] = currentTable[navigationIndex];
        index++;
        navigationIndex++;
        if(navigationIndex >= 12) {
            navigationIndex = 0;
            if(tableNumber == -2 && m_firstIndirectPointers != nullptr) {
                tableNumber++;
                currentTable = m_firstIndirectPointers;
            } else if (m_secondIndirectPointers != nullptr){
                tableNumber++;
                currentTable = m_secondIndirectPointers[tableNumber];
            } else {
                currentTable = nullptr;
            }
        }
    }

    return statusIndexes;
}

int INode::getNumberOfBlocks() {
    int counter = 0;
    int* currentTable = m_addressPointers;

    int navigationIndex = 0;
    int tableNumber = -2;

    while(currentTable != nullptr && currentTable[navigationIndex] != -1) {
        counter++;
        navigationIndex++;
        if(navigationIndex >= 12) {
            navigationIndex = 0;
            if(tableNumber == -2 && m_firstIndirectPointers != nullptr) {
                tableNumber++;
                currentTable = m_firstIndirectPointers;
            } else if (m_secondIndirectPointers != nullptr){
                tableNumber++;
                currentTable = m_secondIndirectPointers[tableNumber];
            } else {
                currentTable = nullptr;
            }
        }
    }
    return counter;
}
