//
// Created by jntre on 01.08.2023.
//

#include "INodeFile.h"
#include "INode.h"
#include "iostream"

INodeFile::INodeFile(char *name, Attributes *attributes, int size, INode *iNode) : AbstractFile(name, attributes, size){
    this->iNode = iNode;
}

INode *INodeFile::getINode() const {
    return iNode;
}

void INodeFile::setINode(INode *iNode) {
    INodeFile::iNode = iNode;
}