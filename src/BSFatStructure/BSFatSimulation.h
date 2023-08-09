
#ifndef BS_PROJEKT_BSFATSIMULATION_H
#define BS_PROJEKT_BSFATSIMULATION_H
#include "../DirectoryStructure/Directory.h"
#include "BSCluster.h"

#define RESERVED 'R'
#define CORRUPTED 'D'
#define FREE '0'
#define OCCUPIED '1'

class BSFatSimulation {
public:
    BSFatSimulation(unsigned int blockSize, unsigned int fatSize);

    Directory* getRootDirectory();

    int getFreeDiskSpace();

    void defragmentDisk(Directory *directory, int currentPosition);

    float getFragmentation(Directory* directory, float fragmentation);

    void createFile(char* name, bool editable, bool system, bool asccii, bool randAccFile, int size);

    void createFilesForSim(char** names, unsigned int length);

    void createDirectoriesForSim();

    void createDirectory(char* name, Attributes* attributes);

    void updateFile(char *name, Attributes *attributes, bool isEditable, bool isSystem, bool isAscii, bool isRamFile,
                    AbstractFile *file, int size);

    void updateDirectory();

    char* getPath();

    BSCluster* initBSCluster(int numberOfBlocks);

    void freeFileMemory(AbstractFile* file);

    void deleteFile(AbstractFile* file);

    void deleteDirectory(Directory *directory);

    Directory* getCurrentDirectory();

    void setCurrentDirectory(Directory* directory);

    unsigned char * getStatusArray();

    unsigned int getBlockSize();

    unsigned int getFatSize();

    unsigned int getNumberOfFilesThatCanBeSaved();

    unsigned int getNumberOfCurrentlySavedFiles();

private:
    Directory* m_currentDirectory;
    unsigned char* m_statusArray;
    unsigned int m_blockSize;
    unsigned int m_fatSize;
    unsigned int m_numberOfFiles;
    BSCluster *searchClusterByIndex(Directory *directory, unsigned int index);
    void simulate();
};


#endif //BS_PROJEKT_BSFATSIMULATION_H
