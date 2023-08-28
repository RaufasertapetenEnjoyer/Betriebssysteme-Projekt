//
// Created by jntre on 27.07.2023.
//

#ifndef PROJEKT_BS_INODE_H
#define PROJEKT_BS_INODE_H

class INode {
    public:
        INode(int numberOfBlocksForFile);

        int *getAddressPointers();

        int *getFirstIndirectPointers() const;

        int **getDoubleIndirectPointers() const;

        void addAddress(int address);

        void addAddressAtIndex(int address, int index);

        void deleteAddressFromIndex(int index);

        void initINode();

        void initTable(int* table);

        void detectEmptyTables();

        void findAndReplaceAddress(int oldAddress, int newAddress);

        bool checkIfAddressExists(int address);

        int* getIndexes();

        int getNumberOfBlocks();

private:
        int m_addressPointers[12];
        int* m_firstIndirectPointers;
        int** m_secondIndirectPointers;
};


#endif //PROJEKT_BS_INODE_H
