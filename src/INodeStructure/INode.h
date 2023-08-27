//
// Created by jntre on 27.07.2023.
//

#ifndef PROJEKT_BS_INODE_H
#define PROJEKT_BS_INODE_H

class INode {
    public:
        INode(int numberOfBlocksForFile);

        //INode();

        int *getAddressPointers();

        int **getFirstIndirectPointers() const;

        int ***getDoubleIndirectPointers() const;

        void addAddress(int address);

        void addAddressAtIndex(int address, int index);

        void deleteAddressFromIndex(int index);

        void initINode();

        void initTable(int* table);

        void detectEmptyTables();

private:
        int* addressPointers;
        int** firstIndirectPointers;
        int*** doubleIndirectPointers;
};


#endif //PROJEKT_BS_INODE_H
