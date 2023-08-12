
#include "BSFatSimulation.h"
#include "BSCluster.h"
#include "BSFatFile.h"
#include "iostream"
#include <cmath>
#include <cstring>
#include <time.h>


/**
 * Bei File gibt es 4 verscheidene Flags die gesetzt werden können, das sind die Flags editable (Index 0),
 * Systemfile (Index 1), Asccii-File (index 2) und  Random-Accsses-File (Index 3)
 */

/**
 * Simulates the file-system with some directory and files.
 */
void BSFatSimulation::simulate() {
    for (int i = 0; i < 40; i++) {
        if((rand() % 2) + 1 == 1){
            m_statusArray[rand() % getNumberOfFilesThatCanBeSaved()] = CORRUPTED;
        }else{
            m_statusArray[rand() % getNumberOfFilesThatCanBeSaved()] = RESERVED;
        }
    }

    char* root =  {"root"};
    auto* directory = new Directory(root, new Attributes());
    std::cout << directory->getName() << '\n' << std::endl;
    directory->setParentDirectory(nullptr);
    m_currentDirectory = directory;

    //char** fileNamesForRoot = new char*[] {"programm1.c", "2programm.c", "prog3.c.cpp", "p4rogramm1.c", "program5m.c", "pr6ogramm.c", "progra7mm.c", "programm8.c", "progr9amm.c", "program.cpp"};
    char* fileNamesForRoot[] = {"programm1.c", "2programm.c", "prog3.c.cpp", "p4rogramm1.c", "program5m.c", "pr6ogramm.c", "progra7mm.c", "programm8.c", "progr9amm.c", "program.cpp"};
    createFilesForSim(fileNamesForRoot, 10);
    createDirectoriesForSim();
    //TODO

}

/**
 * Returns a pointer on the root-directory.
 * @return Directory* root
 */
Directory *BSFatSimulation::getRootDirectory() {
    if(m_currentDirectory->getParentDirectory() == nullptr){
        return m_currentDirectory;
    }else{
        Directory* directory = m_currentDirectory;

        while (directory->getParentDirectory() != nullptr){
            directory = directory->getParentDirectory();
        }
        return directory;
    }
}

/**
 * Calculates the free disk space as a number of blocks (marked with the Flag FREE).
 * @return int diskSpace
 */
int BSFatSimulation::getFreeDiskSpace() {
    int freeCounter = 0;
    for (int i = 0; i < getNumberOfFilesThatCanBeSaved(); i++) {
        if(m_statusArray[i] == FREE){
            freeCounter++;
        }
    }
    return freeCounter;
}

/**
 * Defragment the whole file-system (recursive traverse algorithm).
 * @param Directory* directory
 */
void BSFatSimulation::defragmentDisk(Directory *directory, int &currentPosition) {
    if(directory == nullptr){
        return;
    }

    auto* file = dynamic_cast<BSFatFile*>(directory->getFileList());

    while (file != nullptr){
        BSCluster* current = file->getFirstBlock();

        while(current != nullptr){
            if(m_statusArray[currentPosition] == FREE){
                m_statusArray[currentPosition] = RESERVED;
                m_statusArray[current->getIndex()] = FREE;
                current->setIndex(currentPosition);
                m_statusArray[currentPosition] = OCCUPIED;
            }else{
                while(m_statusArray[currentPosition] == CORRUPTED || m_statusArray[currentPosition] == RESERVED){
                    currentPosition++;
                }

                if(m_statusArray[currentPosition] == FREE){
                    m_statusArray[currentPosition] = RESERVED;
                    m_statusArray[current->getIndex()] = FREE;
                    current->setIndex(currentPosition);
                    m_statusArray[currentPosition] = OCCUPIED;
                }else{
                    if(currentPosition != current->getIndex()){
                        BSCluster* secondCurrent = searchClusterByIndex(getRootDirectory(), currentPosition);
                        int saver = current->getIndex();
                        current->setIndex(secondCurrent->getIndex());
                        secondCurrent->setIndex(saver);
                    }
                }
            }
            currentPosition++;
            current = current->getNextBlock();
        }
        file = dynamic_cast<BSFatFile*>(file->getNextFile());
    }

    Directory* subDirectory = directory->getSubDirectoryList();
    while(subDirectory != nullptr){
        defragmentDisk(subDirectory, currentPosition);
        subDirectory = subDirectory->getNextDirectory();
    }
}

/**
 * Creates a file beneath the current directory.
 * @param char* name
 * @param Attributes* attributes
 * @param int size
 */
void BSFatSimulation::createFile(char* name, bool editable, bool system, bool ascii, bool randAccFile, int size) {
    int numberOfBlocks = ceil((double) size / (double) m_blockSize);

    if(getFreeDiskSpace() < numberOfBlocks){
        std::cout << "Disk is full, the file can't be created!";
    }else {
        auto *attributes = new Attributes();
        attributes->dateOfCreation = time(nullptr);
        attributes->dateOfLastEdit = time(nullptr);
        attributes->size = size;

        auto *file = new BSFatFile(name, attributes, size);

        file->setEditable(editable);
        file->setSystem(system);
        file->setAscii(ascii);
        file->setRandAccFile(randAccFile);

        file->setFirstBlock(initBSCluster(numberOfBlocks));

        m_numberOfFiles++;

        AbstractFile* file1 = dynamic_cast<AbstractFile*>(file);
        m_currentDirectory->createChildFile( file1);

        Directory* directory = m_currentDirectory;
        while (directory->getParentDirectory() != nullptr){
            directory->setNumberOfFiles(directory->getNumberOfFiles() + 1);
            directory = directory->getParentDirectory();
        }
    }
}

BSFatSimulation::BSFatSimulation(unsigned int blockSize, unsigned int fatSize) {
    m_statusArray = new unsigned char[fatSize / blockSize];
    m_blockSize = blockSize;
    m_fatSize = fatSize;
    m_numberOfFiles = 0;

    for (int i = 0; i < (fatSize/blockSize); i++) {
        m_statusArray[i] = FREE;
    }

    simulate();
    m_currentDirectory = getRootDirectory();
    std::cout << m_currentDirectory->getName() << std::endl;
}

/**
 * Creates some files with random size under the current directory.
 * @param char** names, name array
 * @param length, length of the array
 */
void BSFatSimulation::createFilesForSim(char** names, unsigned int length) {
    for (int i = 0; i < length; i++) {
        createFile(names[i], true, false, false, true, (int) (rand() % 2100 + 1));
    }
}

/**
 * Calculates the fragmentation of the whole disk (recursive).
 * @param Directory* directory (start from root)
 * @param float fragmentation (start with 0.0)
 * @return
 */
float BSFatSimulation::getFragmentation(Directory* directory) {
    if(directory == nullptr){
        return 0.0;
    }



    auto* file = dynamic_cast<BSFatFile*>(directory->getFileList());
    float sumOfFrag = 0.0;

    while(file != nullptr) {
        BSCluster* currentCluster = file->getFirstBlock();

        int minIndex = currentCluster->getIndex();
        int maxIndex = currentCluster->getIndex();
        int numberOfBlocks = 0;

        while(currentCluster != nullptr) {
            if (currentCluster->getIndex() > maxIndex) {
                maxIndex = currentCluster->getIndex();
            } else if (currentCluster->getIndex() < minIndex) {
                minIndex = currentCluster->getIndex();
            }
            currentCluster = currentCluster->getNextBlock();
            numberOfBlocks++;
        }
        int length = maxIndex - minIndex + 1;
        sumOfFrag += (float)(length - numberOfBlocks) / length;
        file = dynamic_cast<BSFatFile*>(file->getNextFile());
    }

    Directory* subDirectory = directory->getSubDirectoryList();
    while(subDirectory != nullptr){

        sumOfFrag += getFragmentation(subDirectory);
        subDirectory = subDirectory->getNextDirectory();
    }

    return (sumOfFrag / (float) m_numberOfFiles);
}

/**
 * Creates an subdirectory under the current directory.
 * @param char* name, nam of the directory
 * @param Attributes* attributes
 */
void BSFatSimulation::createDirectory(char* name, Attributes* attributes) {
    m_currentDirectory->createChildDirectory(name, attributes);
}

/**
 * Returns the current directory.
 * @return Directory* directory
 */
Directory *BSFatSimulation::getCurrentDirectory() {
    return m_currentDirectory;
}

/**
 * Sets the current directory.
 * @param Directory* directoryToSet
 */
void BSFatSimulation::setCurrentDirectory(Directory *directory) {
    m_currentDirectory = directory;
}

/**
 * Returns the status array.
 * @return char* statusArray
 */
unsigned char * BSFatSimulation::getStatusArray() {
    return m_statusArray;
}

/**
 * Returns the block size of the bsFat.
 * @return unsigned int blockSize
 */
unsigned int BSFatSimulation::getBlockSize() {
    return m_blockSize;
}

/**
 * Returns the fat size of the bsFat.
 * @return unsigned int fatSize
 */
unsigned int BSFatSimulation::getFatSize() {
    return m_fatSize;
}

/**
 * Returns the number of blocks that potentially can be saved.
 * @return unsigned int numberOfBlocks
 */
unsigned int BSFatSimulation::getNumberOfFilesThatCanBeSaved() {
    return m_fatSize / m_blockSize;
}

/**
 * Returns the number of files that are currently saved in the bsFat.
 * @return unsigned int numberOfFiles
 */
unsigned int BSFatSimulation::getNumberOfCurrentlySavedFiles() {
    return m_numberOfFiles;
}

/**
 * Creates some directories for the simulation.
 */
void BSFatSimulation::createDirectoriesForSim() {
    m_currentDirectory = getRootDirectory();

    char* directoryNames[] = {"Michael", "Jan", "Simon", "Betriebssysteme", "Algo", "SWT", "Mathe1", "Mathe2", "Datenbanken", "Projekt"};

    //user-directory Michael
    //      with three files in it (main.c, hallo.txt and Presentation.docx)
    //      with three subdirectories (Betriebssysteme, Algo and SWT)
        //Subdirectory Betriebssysteme
        //      with one subdirectory (Projekt)
        //      with one file in it
            //Subdirectory Projekt is empty
        //Subdirectory Algo is empty
        //Subdirectory SWT
        //      with two files in it (
    char* fileNamesMichael[] =  {"main.c", "hallo.txt", "Presentation.docx"};
    char* fileNameBetriebssysteme[] = {"BSSimulation.h"};
    char* fileNamesSWT[] = {"AbstractAnimal.java", "Dog.java"};

    m_currentDirectory->createChildDirectory(directoryNames[0], new Attributes());
    std::cout<<m_currentDirectory->getSubDirectoryList()->getName()<<std::endl;
    m_currentDirectory = m_currentDirectory->getSubDirectoryList();


    //Michael/
    m_currentDirectory->createChildDirectory(directoryNames[3], new Attributes());
    m_currentDirectory->createChildDirectory(directoryNames[4], new Attributes());
    m_currentDirectory->createChildDirectory(directoryNames[5], new Attributes());

    createFilesForSim(fileNamesMichael, 3);

    //Michael/Betriebssysteme
    m_currentDirectory = m_currentDirectory->getSubDirectoryList();
    createFilesForSim(fileNameBetriebssysteme, 1);
    m_currentDirectory->createChildDirectory(directoryNames[9], new Attributes());

    //Michael/SWT
    m_currentDirectory = m_currentDirectory->getNextDirectory()->getNextDirectory();
    createFilesForSim(fileNamesSWT, 2);


    m_currentDirectory = getRootDirectory();


    //user-directory Jan
    //      with two subdirectories (Mathe1 and Mathe2)
        //Subdirectory Mathe1 is empty
        //Subdirectory Mateh2
        //      with two files
    char* fileNamesMathe2[] = {"dgl.docx", "Mathe_abgabe.pdf"};
    m_currentDirectory->createChildDirectory(directoryNames[1], new Attributes());

    //Jan/
    m_currentDirectory = m_currentDirectory->getSubDirectoryList()
            ->getNextDirectory();
    m_currentDirectory->createChildDirectory(directoryNames[6], new Attributes());
    m_currentDirectory->createChildDirectory(directoryNames[7], new Attributes());

    //Jan/Mathe2
    m_currentDirectory = m_currentDirectory->getSubDirectoryList()->getNextDirectory();
    createFilesForSim(fileNamesMathe2, 2);


    m_currentDirectory = getRootDirectory();


    //user-directory Simon
    //      with one subdirectory (com)
        //Subdirectory com
        //      with one subdirectory (de)
            //Subdirectory de
            //      with one subdirectory (bs)
                //Subdirectory bs
                //      with one subdirectory (BsProjekt)
                    //Subdirectory BsProjekt
                    //      with three files
    char* fileNamesBsProjekt[] = {"main.cpp", "icon1.png", "mainWindow.qt"};
    char* directoryNamesSimon[] = {"comic", "de", "bs", "bsProjekt"};
    m_currentDirectory->createChildDirectory(directoryNames[2], new Attributes());

    //Simon/
    m_currentDirectory = m_currentDirectory->getSubDirectoryList()
            ->getNextDirectory()
            ->getNextDirectory();
    m_currentDirectory->createChildDirectory(directoryNamesSimon[0], new Attributes());

    //Simon/com
    m_currentDirectory = m_currentDirectory->getSubDirectoryList();
    m_currentDirectory->createChildDirectory(directoryNamesSimon[1], new Attributes());

    //Simon/com/de
    m_currentDirectory = m_currentDirectory->getSubDirectoryList();
    m_currentDirectory->createChildDirectory(directoryNamesSimon[2], new Attributes());

    //Simon/com/de/bs
    m_currentDirectory = m_currentDirectory->getSubDirectoryList();
    m_currentDirectory->createChildDirectory(directoryNamesSimon[3], new Attributes());

    //Simon/com/de/bs/bsProjekt
    m_currentDirectory = m_currentDirectory->getSubDirectoryList();
    createFilesForSim(fileNamesBsProjekt,3);


    m_currentDirectory = getRootDirectory();
}

/**
 * Searches and returns the cluster with the given index.
 * @param Directory* directory
 * @param unsigned int index
 * @return BsCluster* cluster
 */
BSCluster * BSFatSimulation::searchClusterByIndex(Directory *directory, unsigned int index) {
    if(directory == nullptr){
        return nullptr;
    }

    auto* file = dynamic_cast<BSFatFile*>(directory->getFileList());
    BSCluster* bsCluster;
    while(file != nullptr){
        bsCluster = file->getFirstBlock();

        while(bsCluster != nullptr){
            if(bsCluster->getIndex() == index){
                return bsCluster;
            }
            bsCluster = bsCluster->getNextBlock();
        }
        file = dynamic_cast<BSFatFile*>(file->getNextFile());
    }

    Directory* subDirectory = directory->getSubDirectoryList();
    /*if(subDirectory == nullptr){
        std::cout<<directory->getName()<<"==null"<<std::endl;
    }*/
    BSCluster* found;
    while(subDirectory != nullptr || subDirectory != 0x0){
        found = searchClusterByIndex(subDirectory, index);
        if(found != nullptr){
            return found;
        }
        subDirectory = subDirectory->getNextDirectory();
    }
    return found;
}

/**
 * Deletes the given file from the current directory.
 * @param AbstractFile* file
 */
void BSFatSimulation::deleteFile(AbstractFile *file) {
    if(file->getNextFile() == nullptr && file->getPrevFile() != nullptr){
        AbstractFile* prevFile = file->getPrevFile();
        prevFile->setNextFile(nullptr);

        file->setNextFile(nullptr);
        file->setPrevFile(nullptr);

        freeFileMemory(file);

        delete file;
    }else if(file->getNextFile() != nullptr && file->getPrevFile() == nullptr){
        AbstractFile* nextFile = file->getNextFile();
        nextFile->setPrevFile(nullptr);

        file->setNextFile(nullptr);

        m_currentDirectory->setFileList(nextFile);

        freeFileMemory(file);

        delete file;
    }else if(file->getNextFile() != nullptr && file->getPrevFile() != nullptr){
        AbstractFile* nextFile = file->getNextFile();
        AbstractFile* prevFile = file->getPrevFile();
        nextFile->setPrevFile(prevFile);
        prevFile->setNextFile(nextFile);

        file->setNextFile(nullptr);
        file->setPrevFile(nullptr);

        freeFileMemory(file);

        delete file;
    }else if(file->getNextFile() == nullptr && file->getPrevFile() == nullptr){
        m_currentDirectory->setFileList(nullptr);
        freeFileMemory(file);

        delete file;
    }
    m_numberOfFiles--;
}

/**
 * Deletes the given directory under the current directory, if and only if the directory is empty.
 * @param directory
 */
void BSFatSimulation::deleteDirectory(Directory *directory) {
    if(directory->getSubDirectoryList() == nullptr && directory->getFileList() == nullptr){
        if(directory->getNextDirectory() == nullptr && directory->getPrevDirectory() != nullptr){
            Directory* preDirectory = directory->getPrevDirectory();
            preDirectory->setNextDirectory(nullptr);

            directory->setPreviousDirectory(nullptr);

            delete directory;
        }else if(directory->getNextDirectory() != nullptr && directory->getPrevDirectory() == nullptr){
            Directory* nextDirectory = directory->getNextDirectory();
            nextDirectory->setPreviousDirectory(nullptr);

            m_currentDirectory->setDirectoryList(nextDirectory);

            directory->setNextDirectory(nullptr);

            delete directory;
        }else if(directory->getNextDirectory() != nullptr && directory->getPrevDirectory() != nullptr){
            Directory* nextDirectory = directory->getNextDirectory();
            Directory* prevDirectory = directory->getPrevDirectory();
            nextDirectory->setPreviousDirectory(prevDirectory);
            prevDirectory->setNextDirectory(nextDirectory);

            directory->setPreviousDirectory(nullptr);
            directory->setNextDirectory(nullptr);

            delete directory;
        }
    }
}

/**
 * Updates the attributes of a given file.
 * @param char* name
 * @param bool isEditable
 * @param bool isSystem
 * @param bool isAscii
 * @param bool isRamFile
 * @param AbstractFile* file
 * @param int size
 */
void BSFatSimulation::updateFile(char *name, bool isEditable, bool isSystem, bool isAscii, bool isRamFile,
                                 AbstractFile *file, int size) {//evtl. ausgaben oder ähnliches nötig
    if(file->tstBit(file->getAttributes()->attributes, 0)){
        if(!file->isSystem()){
            if(strcmp(file->getName(), name) != 0){
                file->setName(name);
            }
            if(isSystem != file->isSystem()){
                file->tstBit(file->getAttributes()->attributes, 1) ? file->clrBit(file->getAttributes()->attributes, 1) : file->setBit(file->getAttributes()->attributes, 1);
            }
            int numberOfBlocks = ceil((double) size / (double) m_blockSize);
            if(numberOfBlocks <= getFreeDiskSpace() && size != file->getSize()){
                BSFatFile* bsFatFile = dynamic_cast<BSFatFile*>(file);
                freeFileMemory(file);
                bsFatFile->setFirstBlock(initBSCluster(numberOfBlocks));
            }
        }
        //nur attribute können gesetzt werden
        if(isEditable){
            file->clrBit(file->getAttributes()->attributes, 0);
        }
        if(isAscii != file->isAscii()){
            file->tstBit(file->getAttributes()->attributes, 2) ? file->clrBit(file->getAttributes()->attributes, 2) : file->setBit(file->getAttributes()->attributes, 2);
        }
        if (isRamFile != file->isRandAccFile()){
            file->tstBit(file->getAttributes()->attributes, 3) ? file->clrBit(file->getAttributes()->attributes, 3) : file->setBit(file->getAttributes()->attributes, 3);
        }
        file->getAttributes()->dateOfLastEdit = time(nullptr);
    }
    //keine Bearbeitung möglich
}

void BSFatSimulation::updateDirectory() {

}

/**
 * Initialize the cluster structures for a file, occupies the needed memory and returns a pointer to the first cluster.
 * @param int numberOfBlocks
 * @return BSCluster* firstCluster
 */
BSCluster *BSFatSimulation::initBSCluster(int numberOfBlocks) {
    if(numberOfBlocks <= getFreeDiskSpace() && numberOfBlocks != 0){
        auto **bsClusters = new BSCluster *[numberOfBlocks];

        for (int i = 0; i < numberOfBlocks; i++) {
            bsClusters[i] = new BSCluster();
        }

        bsClusters[0]->setPrevElement(nullptr);
        bsClusters[numberOfBlocks - 1]->setNextElement(nullptr);

        if (numberOfBlocks > 2) {
            for (int i = 1; i < numberOfBlocks - 1; i++) {
                bsClusters[i - 1]->setNextElement(bsClusters[i]);
                bsClusters[i]->setPrevElement(bsClusters[i - 1]);
                bsClusters[i]->setNextElement(bsClusters[i + 1]);
                bsClusters[i + 1]->setPrevElement(bsClusters[i]);
            }

            bsClusters[numberOfBlocks - 1]->setPrevElement(bsClusters[numberOfBlocks - 2]);

        } else if (numberOfBlocks == 2) {
            bsClusters[1]->setPrevElement(bsClusters[0]);
            bsClusters[0]->setNextElement(bsClusters[1]);
        }

        unsigned int pos;

        for (int i = 0; i < numberOfBlocks; i++) {
            pos = (rand() % (m_fatSize / m_blockSize));
            while (m_statusArray[pos] != FREE) {
                pos = (rand() % (m_fatSize / m_blockSize));
            }
            bsClusters[i]->setIndex(pos);
            m_statusArray[pos] = OCCUPIED;
        }
        return bsClusters[0];
    }else{
        return nullptr;
    }
}

/**
 * Frees the memory of the given file.
 * @param AbstractFile*  file
 */
void BSFatSimulation::freeFileMemory(AbstractFile *file) {
    BSFatFile* bsFatFile = dynamic_cast<BSFatFile*>(file);
    BSCluster* cluster = bsFatFile->getFirstBlock();

    bsFatFile->setFirstBlock(nullptr);

    while(cluster->getNextBlock() != nullptr){
        m_statusArray[cluster->getIndex()] = FREE;
        BSCluster* pre = cluster;
        cluster = cluster->getNextBlock();
        delete pre;
    }
}

/**
 * Returns the path of the current directory.
 * @return const char* path
 */
const char *BSFatSimulation::getPath() {
    Directory* directory = m_currentDirectory;
    std::string path = "";
    while (directory != nullptr){
        std::string preName = directory->getName();
        path = preName + "/" + path;
        directory = directory->getParentDirectory();
    }
    const char* pathAsChar = path.c_str();

    return pathAsChar;
}
