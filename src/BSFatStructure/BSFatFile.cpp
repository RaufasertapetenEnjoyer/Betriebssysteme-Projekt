
#include "BSFatFile.h"


BSFatFile::BSFatFile(char *name, char *attributes, int size) : AbstractFile(name, attributes, size) {

}

BSCluster *BSFatFile::getFirstBlock() {
    return m_bsCluster;
}

void BSFatFile::setFirstBlock(BSCluster *block) {
    m_bsCluster = block;
}
