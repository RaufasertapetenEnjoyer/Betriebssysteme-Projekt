#include "BSFatFile.h"
#include "../DirectoryStructure/AbstractElement.h"


BSFatFile::BSFatFile(char *name, Attributes *attributes, int size) : AbstractFile(name, attributes, size) {}

BSCluster *BSFatFile::getFirstBlock() {
    return m_bsCluster;
}

void BSFatFile::setFirstBlock(BSCluster *block) {
    m_bsCluster = block;
}

BSCluster* BSFatFile::getLastBlock() {
    BSCluster* cluster = this->getFirstBlock();
    while (cluster->getNextBlock() != nullptr){
        cluster = cluster->getNextBlock();
    }
    return cluster;
}
