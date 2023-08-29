#include "mbr.h"

/**
 * @brief initializes simulations
 */
MBR::MBR()
{
    iNode = new INodeSimulation(512, 16384*50,"INode-Platte");
    bsFat = new BSFatSimulation(512, 16384*5,"BsFat-Platte");
}

BSFatSimulation * MBR::getBsFat(){
    return bsFat;
}

INodeSimulation * MBR::getINode(){
    return iNode;
}
