//
// Created by jntre on 27.07.2023.
//

#include "math.h"
#include "INode.h"
#include "iostream"

/**
 * creates an INode and sets the tables according to the number of blocks that we want to save in it, unused tables are nullptr
 * @param numberOfBlocksForFile
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

/**
 * Returns the attribute m_addressPointers
 * @return int* array of addresses
 */
int* INode::getAddressPointers() {
    return m_addressPointers;
}

/**
 * Returns the attribute m_firstIndirectPointers
 * @return int* array of addresses
 */
int *INode::getFirstIndirectPointers() const {
    return m_firstIndirectPointers;
}

/**
 * Returns the attribute m_secondIndirectPointers
 * @return  int** array of arrays of addresses
 */
int **INode::getDoubleIndirectPointers() const {
    return m_secondIndirectPointers;
}

/**
 * adds the given address at the end of the existing entries, initializes a new table if necessary -> bigger file size
 * @param address
 */
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
            if(index >= (12 * 14)) {
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
                if (currentTableIndex < 12) {
                    currentTable = m_secondIndirectPointers[currentTableIndex];
                } else {
                    std::cout << "File cannot be larger." << std::endl;
                }
                currentTableIndex = floor((index - 24) / 12.0);
                std::cout << currentTableIndex << std::endl;
            }
        }
    }
    currentTable[index % 12] = address;
}

/**
 * adds the given address at the given index, only used for replacing entries that already have a valid address -> defragmentation
 * @param address
 * @param index ongoing index, max 168
 */
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

/**
 * deletes all addresses after the given index, deletes a table if it is empty -> smaller file size
 * @param index
 */
void INode::deleteAddressFromIndex(int index) {
    int* currentTable = m_addressPointers;

    int navigationIndex = 0;
    int compareIndex = 0;
    int tableNumber = -2;

    while(currentTable != nullptr && currentTable[navigationIndex] != -1 && tableNumber < 12) {
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
                if (tableNumber < 12) {
                    currentTable = m_secondIndirectPointers[tableNumber];
                } else {
                    currentTable = nullptr;
                }
            } else {
                currentTable = nullptr;
            }
        }
    }
    detectEmptyTables();
}

/**
 *  initially sets all entries of the active tables to -1
 */
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
        } else if(tableNumber >= 0 && tableNumber < 12 && m_secondIndirectPointers != nullptr) {
            currentTable = m_secondIndirectPointers[tableNumber];
        }else {
            currentTable = nullptr;
        }
    }
}

/**
 * sets every entry of the given table to -1
 * @param table
 */
void INode::initTable(int *table) {
    for (int i = 0; i < 12 ; i++) {
        table[i] = -1;
    }
}

/**
 * if a m_firstIndirectPointers or m_secondIndirectPointers consist only of entries with -1, which means that they are empty, they get set to nullptr
 */
void INode::detectEmptyTables() {
    if (m_firstIndirectPointers[0] == -1) {
        m_firstIndirectPointers = nullptr;
    }
    if (m_secondIndirectPointers[0][0] == -1) {
        m_secondIndirectPointers = nullptr;
    }
}

/**
 * traverses the INode looking for the given address and replaces ist with the new given address
 * @param oldAddress
 * @param newAddress
 */
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
                if (tableNumber < 12) {
                    currentTable = m_secondIndirectPointers[tableNumber];
                } else {
                    currentTable = nullptr;
                }
            } else {
                currentTable = nullptr;
            }
        }
    }
}

/**
 * traverses the INode looking for the given address
 * @param address
 * @return int* pointer on entry of address
 */
int* INode::checkIfAddressExists(int address) {
    int* currentTable = m_addressPointers;

    int navigationIndex = 0;
    int tableNumber = -2;

    while(currentTable != nullptr && currentTable[navigationIndex] != -1) {
        if (currentTable[navigationIndex] == address) {
            return &currentTable[navigationIndex];
        }
        navigationIndex++;
        if(navigationIndex >= 12) {
            navigationIndex = 0;
            if(tableNumber == -2 && m_firstIndirectPointers != nullptr) {
                tableNumber++;
                currentTable = m_firstIndirectPointers;
            } else if(m_secondIndirectPointers != nullptr){
                tableNumber++;
                if (tableNumber < 12) {
                    currentTable = m_secondIndirectPointers[tableNumber];
                } else {
                    currentTable = nullptr;
                }
            } else {
                currentTable = nullptr;
            }
        }
    }
    return nullptr;
}

/**
 * Returns an array of all the valid entries of the tables
 * @return int* indexes
 */
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
                if (tableNumber < 12) {
                    currentTable = m_secondIndirectPointers[tableNumber];
                } else {
                    currentTable = nullptr;
                }
            } else {
                currentTable = nullptr;
            }
        }
    }

    return statusIndexes;
}

/**
 * counts how many entries are set to valid addresses
 * @return int numberOfBlocks
 */
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
                if (tableNumber < 12) {
                    currentTable = m_secondIndirectPointers[tableNumber];
                } else {
                    currentTable = nullptr;
                }
            } else {
                currentTable = nullptr;
            }
        }
    }
    return counter;
}
