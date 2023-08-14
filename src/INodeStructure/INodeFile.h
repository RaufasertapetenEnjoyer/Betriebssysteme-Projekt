//
// Created by jntre on 01.08.2023.
//

#ifndef PROJEKT_BS_INODEFILE_H
#define PROJEKT_BS_INODEFILE_H


#include "../DirectoryStructure/AbstractFile.h"
#include "INode.h"

class INodeFile: public AbstractFile {
public:
    INodeFile(char *name, Attributes *attributes, int size, INode *iNode);

    INode *getINode() const;

    void setINode(INode *iNode);

private:
        INode* iNode;

};


#endif //PROJEKT_BS_INODEFILE_H
