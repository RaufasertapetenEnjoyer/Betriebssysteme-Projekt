

#ifndef BS_PROJEKT_BSCLUSTER_H
#define BS_PROJEKT_BSCLUSTER_H


class BSCluster {
public:
    BSCluster();
    BSCluster* getNextBlock();
    void setNextElement(BSCluster* nextElement);

    BSCluster* getPrevBlock();
    void setPrevElement(BSCluster* prevElement);

    int getIndex();
    void setIndex(int index);

private:
    /**
     * Pointer on the next block following the current.
     */
    BSCluster* m_nextElement;
    /**
     * Indicates the position of the current block in the status-array (memory).
     */
    int m_statusArrayIndex;
    /**
     * Pointer on the previous block behind the current.
     */
    BSCluster* m_prevElement;
};


#endif //BS_PROJEKT_BSCLUSTER_H
