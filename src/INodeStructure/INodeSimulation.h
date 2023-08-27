//
// Created by jntre on 01.08.2023.
//

#ifndef PROJEKT_BS_INODESIMULATION_H
#define PROJEKT_BS_INODESIMULATION_H


#include "../DirectoryStructure/Directory.h"
//#include "../DirectoryStructure/AbstractSimulation.h"
#include "../BSFatStructure/BSFatFile.h"
#include "../BSFatStructure/BSCluster.h"
#include "INodeFile.h"

#define RESERVED 'R'
#define CORRUPTED 'D'
#define FREE '0'
#define OCCUPIED '1'

class INodeSimulation{
public:
    INodeSimulation(unsigned int blockSize, unsigned int totalSize, char* name);

    Directory* getRootDirectory();

    int getFreeDiskSpace();

    void defragmentDisk(Directory *directory, int currentPosition);

    float getFragmentation(Directory* directory, float fragmentation);

    void createFile(char* name, bool editable, bool system, bool ascii, bool randAccFile, int size);

    void createFilesForSim(char** names, unsigned int length);

    void createDirectoriesForSim();

    void createDirectory(char* name, Attributes* attributes);

    void updateFile(char *name, bool isEditable, bool isSystem, bool isAscii, bool isRamFile, AbstractFile *file, int size);

    void updateDirectory(char* name, bool isEditable, Directory* directory);

    const char* getPath();

//    BSCluster* initBSCluster(int numberOfBlocks);

    void freeFileMemory(AbstractFile* file);

    void deleteFile(AbstractFile* file);

    void deleteDirectory(Directory *directory);

    Directory* getCurrentDirectory();

    void setCurrentDirectory(Directory* directory);

    unsigned char * getStatusArray();

    unsigned int getBlockSize();

    unsigned int getTotalSize();

    unsigned int getNumberOfFilesThatCanBeSaved();

    unsigned int getNumberOfCurrentlySavedFiles();

    //static?
    unsigned int getINodeSize();

    unsigned int getNumberOfINodes();

    INodeFile* convertFatToINode(BSFatFile* bsFatFile, int bsBlockSize);

    void copyFileFromROM(BSFatFile* bsFatFile, Directory* directory);

private:
    Directory* m_currentDirectory;
    char* m_name;
    unsigned int m_blockSize;
    unsigned int m_totalSize;
    unsigned char* m_statusArray;
    unsigned int m_numberOfFiles;
    unsigned int m_numberOfBlocksPerINode;
    //static?
    unsigned int m_iNodeSize;
    void simulate();

};


#endif //PROJEKT_BS_INODESIMULATION_H
