#include "mbr.h"

/**
 * @brief initializes simulations
 */
MBR::MBR()
{
    iNode = new INodeSimulation(512, 16384*15,"INode-Platte");
    bsFat = new BSFatSimulation(512, 16384*5,"BsFat-Platte");
}
/**
 * @brief MBR::getBsFat
 * @return BSFatSimulation *bsFat
 */
BSFatSimulation * MBR::getBsFat(){
    return bsFat;
}

/**
 * @brief MBR::getINode
 * @return INodeSimulation * iNode
 */
INodeSimulation * MBR::getINode(){
    return iNode;
}
