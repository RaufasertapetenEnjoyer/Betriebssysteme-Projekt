//
// Created by jntre on 27.07.2023.
//

#ifndef PROJEKT_BS_INODE_H
#define PROJEKT_BS_INODE_H

class INode {
    public:
        INode(int numberOfBlocksForFile, int numberOfBlocksPerINode);

        INode();

        int *getAddressPointers() const;

        void setAddressPointers(int *pAddressPointers);

        int **getFirstIndirectPointers() const;

        void setFirstIndirectPointers(int **pFirstIndirectPointers);

        int ***getDoubleIndirectPointers() const;

        void setDoubleIndirectPointers(int ***pDoubleIndirectPointers);

        void addAddress(int address, int numberOfBlocksPerINode);

        void addAddressAtIndex(int address, int numberOfBlocksPerINode, int index);


private:
        int* addressPointers;
        int** firstIndirectPointers;
        int*** doubleIndirectPointers;
};


#endif //PROJEKT_BS_INODE_H
