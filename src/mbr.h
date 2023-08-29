#ifndef MBR_H
#define MBR_H
#include <BSFatStructure/BSFatSimulation.h>
#include <INodeStructure/INodeSimulation.h>


class MBR
{
public:
    MBR();
    BSFatSimulation * getBsFat();
    INodeSimulation * getINode();
private:
    BSFatSimulation * bsFat;
    INodeSimulation * iNode;
};

#endif // MBR_H
