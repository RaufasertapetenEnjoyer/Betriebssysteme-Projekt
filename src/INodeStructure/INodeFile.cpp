//
// Created by jntre on 01.08.2023.
//

#include "INodeFile.h"
#include "INode.h"

/**
 * Calls the constructor of AbstractFile and additionally sets the INode of the file
 * @param name
 * @param attributes
 * @param size
 * @param iNode
 */
INodeFile::INodeFile(char *name, Attributes *attributes, int size, INode *iNode) : AbstractFile(name, attributes, size){
    this->m_INode = iNode;
}

/**
 * Returns the INode that belongs to the file
 * @return INode* m_INode
 */
INode *INodeFile::getINode() const {
    return m_INode;
}

/**
 * Alternative way to set the INode-Attribute
 * @param iNode
 */
void INodeFile::setINode(INode *iNode) {
    INodeFile::m_INode = iNode;
}