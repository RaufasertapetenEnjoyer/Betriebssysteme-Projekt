
#ifndef BS_PROJEKT_BSFATSIMULATION_H
#define BS_PROJEKT_BSFATSIMULATION_H
#include "../DirectoryStructure/Directory.h"

#define RESERVED 'R'
#define CORRUPTED 'D'
#define FREE '0'
#define OCCUPIED '1'

class BSFatSimulation {
public:
    BSFatSimulation(unsigned int blockSize, unsigned int fatSize);

    void simulate();

    Directory* getRootDirectory();

    int getFreeDiskSpace();

    void defragDisk(Directory* directory);

    float getFragmentation(Directory* directory, float fragmentation);

    void createFile(char* name, char* attributes, int size);

    void createFilesForSim();

    void createDirectory();

    void updateFile();

    void updateDirectory();

    void deleteFile();

    void deleteDirectory();

private:
    Directory* currentDirectory;
    unsigned char* m_statusArray;
    unsigned int m_blockSize;
    unsigned int m_fatSize;
    unsigned int m_numberOfFiles;
};


#endif //BS_PROJEKT_BSFATSIMULATION_H
