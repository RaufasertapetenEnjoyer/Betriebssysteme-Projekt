

#include "BSCluster.h"


BSCluster::BSCluster() {

}

/**
 * Returns the next block of the current block.
 * @return BSCluster* nextElement
 */
BSCluster *BSCluster::getNextBlock() {
    return m_nextElement;
}

/**
 * Sets the next block of the current block.
 * @param BSCluster* nextElement
 */
void BSCluster::setNextElement(BSCluster *nextElement) {
    this->m_nextElement = nextElement;
}

/**
 * Returns the previous block of the current block.
 * @return BSCluster* previous element
 */
BSCluster *BSCluster::getPrevBlock() {
    return m_prevElement;
}

/**
 * Sets the previous block of the current block.
 * @param prevElement
 */
void BSCluster::setPrevElement(BSCluster *prevElement) {
    this->m_prevElement = prevElement;
}

/**
 * Returns the index of the current block in the status-array (memory).
 * @return int index
 */
int BSCluster::getIndex() {
    return m_statusArrayIndex;
}

/**
 * Sets the index of the current block in the status-array (memory).
 * @param int index
 */
void BSCluster::setIndex(int index) {
    m_statusArrayIndex = index;
}

