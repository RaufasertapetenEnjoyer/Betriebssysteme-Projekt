//
// Created by jntre on 01.08.2023.
//

#include "INodeFile.h"
#include "INode.h"
#include "iostream"

INodeFile::INodeFile(char *name, Attributes *attributes, int size, INode *iNode) : AbstractFile(name, attributes, size),
                                                                                   iNode(iNode) {}

INode *INodeFile::getINode() const {
    return iNode;
}

void INodeFile::setINode(INode *iNode) {
    INodeFile::iNode = iNode;
}

/**
 * Ausgabe der Adressen
 */
void INodeFile::traverseINode() {
    int* currentTable = iNode->getAddressPointers();

    int index = 0;
    int statusIndex = -1;
    int tableNumber = -2;

    while(currentTable != nullptr && currentTable[index] != -1) {
        std::cout << currentTable[index] << " ";
        index++;
        if(index >= 12) {
            index = 0;
            if(tableNumber == -2) {
                tableNumber++;
                int* prevTable = currentTable;
                currentTable = *iNode->getFirstIndirectPointers();
                delete prevTable;
            } else {
                tableNumber++;
                currentTable = *iNode->getDoubleIndirectPointers()[tableNumber];
            }
        }
    }
    std::cout << "\n" << std::endl;
}
