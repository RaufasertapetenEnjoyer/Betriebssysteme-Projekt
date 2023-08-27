
#ifndef BS_PROJEKT_BSFATSIMULATION_H
#define BS_PROJEKT_BSFATSIMULATION_H
#include "../DirectoryStructure/Directory.h"
#include "BSCluster.h"
#include "../CDRomStructure/CDRomDirectory.h"

#define RESERVED 'R'
#define CORRUPTED 'D'
#define FREE '0'
#define OCCUPIED '1'

class BSFatSimulation {
public:
    BSFatSimulation(unsigned int blockSize, unsigned int fatSize, char* name);

    Directory* getRootDirectory();

    int getFreeDiskSpace();

    void defragmentDisk(Directory *directory, int &currentPosition);

    void getFragmentation(Directory *directory, float &fragmentation);

    void createFile(char* name, bool editable, bool system, bool asccii, bool randAccFile, int size);

    void createFilesForSim(char** names, unsigned int length);

    void createDirectoriesForSim();

    void createDirectory(char* name, Attributes* attributes);

    void updateFile(char *name, bool isEditable, bool isSystem, bool isAscii, bool isRamFile, AbstractFile *file, int size);

    void updateDirectory(char* name, bool isEditable, Directory* directory);

    const char* getPath();

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

    void updateEditableOnContent(Directory* directory);

    bool checkIfEditIsValid(char *name, bool isEditable, bool isSystem, bool isAscii, bool isRamFile, AbstractFile *file, int size);

    void copyCDRomFile(CDRomFile* cdRomFile, const int cdRomBlockSize);

    void copyCDRomDirectory(CDRomDirectory* directoryToCopy, const int cdRomBlockSize);

    char* getName();

private:
    /**
     * Current directory you are working on.
     */
    Directory* m_currentDirectory;
    /**
     * Memory representation.
     */
    unsigned char* m_statusArray;
    /**
     * Size of the blocks the system is working with.
     */
    unsigned int m_blockSize;
    /**
     * Size of the whole system memory.
     */
    unsigned int m_fatSize;
    /**
     * Number of all files that are saved in the current system.
     */
    unsigned int m_numberOfFiles;
    /**
     * Name of this BsFatSimulation.
     */
    char* m_name;

    BSCluster *searchClusterByIndex(Directory *directory, unsigned int index);
    void simulate();
};


#endif //BS_PROJEKT_BSFATSIMULATION_H
