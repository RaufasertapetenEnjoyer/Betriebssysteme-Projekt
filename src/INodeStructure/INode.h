//
// Created by jntre on 27.07.2023.
//

#ifndef PROJEKT_BS_INODE_H
#define PROJEKT_BS_INODE_H

/**
 * An INode consists of meta-data, a first table with addresses, a pointer to a second table with addresses and a third table with tables in it.
 * The meta-data is outsourced to the file-object, so there is only three attributes left here.
 * We assume the Inode is 16 blocks big, 2 blocks are meta-data, 12 are for the first table and 1 block for each table-pointer.
 * Therefore a table is always 12 blocks what results in a maximum of 168 Blocks per INode
 */
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
    /**
     *  initial table of the INode, that stores addresses of the statusArray
     */
    int m_addressPointers[12];
    /**
     *  first additional table of the INode, that stores addresses of the statusArray
     */
    int* m_firstIndirectPointers;
    /**
     *  second additional table of the INode, that stores another 12 tables that contain addresses of the statusArray
     */
    int** m_secondIndirectPointers;
};


#endif //PROJEKT_BS_INODE_H
