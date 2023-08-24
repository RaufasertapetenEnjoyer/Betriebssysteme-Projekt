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
    void setCurrentDirectory(CDRomDirectory* directoryToSet);

private:
    /**
     * Size of one block of the CD-ROM in kilobyte.
     */
    int m_blockSize;
    /**
     * Size of the whole memory of the CD-ROM in kilobyte.
     */
    int m_romSize;
    /**
     * Number of block that can be saved on the CD-Rom.
     */
    int m_numberOfBlocks;
    /**
     * Volume descriptor of the CD-ROM. Containing the name, a description of the CD-ROM, the time of creation and the time it was last written on.
     */
    VolumeDescriptor* m_volumeDescriptor;
    /**
     * Representation of the memory.
     */
    char* m_statusArray;
    /**
     * Pointer on the current directory.
     */
    CDRomDirectory* m_currentDirectory;

    void createChildFile(char* name, bool editable, bool system, bool ascii, bool randAccFile, int size);
    void createChildDirectory(char* name);
    int getIndexOfNextFreeBlock();
    void simulate();
    void createFilesForSim(char **fileNames, int length);
    void createDirectoriesForSim(char** directoryNames, int length);
};


#endif //BS_CDROMSIMULATION_H
