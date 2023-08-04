
#ifndef BS_PROJEKT_BSFATSIMULATION_H
#define BS_PROJEKT_BSFATSIMULATION_H
#include "Directory.h"
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

    bool isEditable(Attributes* attributes);

    void setEditable(Attributes *attributes, bool isEditable);

    bool isSystem(Attributes* attributes);

    void setSystem(Attributes *attributes, bool isSystem);

    bool isAsccii(Attributes* attributes);

    void setAsccii(Attributes *attributes, bool isAsccii);

    bool isRandAccFile(Attributes* attributes);

    void setRandAccFile(Attributes *attributes, bool isRandAccFile);

    void createFilesForSim(char** names, unsigned int length);

    void createDirectoriesForSim();

    void createDirectory(char* name, Attributes* attributes);

    void updateFile();

    void updateDirectory();

    void deleteFile();

    void deleteDirectory();

    Directory* getCurrentDirectory();

    void setCurrentDirectory(Directory* directory);

    unsigned char * getStatusArray();

    unsigned int getBlockSize();

    unsigned int getFatSize();

    unsigned int getNumberOfFilesThatCanBeSaved();

    unsigned int getNumberOfCurrentlySavedFiles();

    void setBit(char *array, int bitToSet);

    void clrBit(char *array, int bitToClear);

    int tstBit(char *array, int bitToTest);

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
