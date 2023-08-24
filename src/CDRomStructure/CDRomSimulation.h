//
// Created by m.busch on 16.08.2023.
//

#ifndef BS_CDROMSIMULATION_H
#define BS_CDROMSIMULATION_H

#define FREE '0'
#define OCCUPIED '1'

#include <cstdlib>
#include "CDRomDirectory.h"
#include "CDRomFile.h"
#include <ctime>

struct VolumeDescriptor{
    char* name;
    char* description;
    time_t creationTime;
    time_t lastUpdate;
};

class CDRomSimulation {
public:
    CDRomSimulation(int blockSize, int romSize, char* name, char* description);
    int getBlockSize();
    void setBlockSize(int blockSize);
    VolumeDescriptor* getVolumeDescriptor();
    CDRomDirectory* getRootDirectory();
    CDRomDirectory* getCurrentDirectory();
    void setCurrentDirectory(CDRomDirectory* dir);

private:
    int m_blockSize;
    int m_romSize;
    int m_numberOfBlocks;
    VolumeDescriptor* m_volumeDescriptor;
    char* m_statusArray;
    CDRomDirectory* m_currentDirectory;
    void createChildFile(char* name, bool editable, bool system, bool ascii, bool randAccFile, int size);
    void createChildDirectory(char* name);
    int getIndexOfNextFreeBlock();
    void simulate();
    void createFilesForSim(char **fileNames, int length);
    void createDirectoriesForSim(char** directoryNames, int length);
};


#endif //BS_CDROMSIMULATION_H
