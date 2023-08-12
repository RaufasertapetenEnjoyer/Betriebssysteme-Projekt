
#ifndef BS_PROJEKT_BSFATFILE_H
#define BS_PROJEKT_BSFATFILE_H
#include "../DirectoryStructure/AbstractFile.h"
#include "BSCluster.h"

class BSFatFile : public AbstractFile{
public:
    BSFatFile(char* name, Attributes* attributes, int size);
    BSCluster* getFirstBlock();
    void setFirstBlock(BSCluster* block);
    BSCluster* getLastBlock();

private:
    /**
     * Pointer on the first block of the file (double-linked list).
     */
    BSCluster* m_bsCluster;
};


#endif //BS_PROJEKT_BSFATFILE_H
