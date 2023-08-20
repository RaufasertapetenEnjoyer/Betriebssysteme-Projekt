//
// Created by jntre on 01.08.2023.
//

#include "INodeSimulation.h"
#include "INode.h"
#include "INodeFile.h"
#include "ctime"
#include <cstring>
#include "iostream"



INodeSimulation::INodeSimulation(unsigned int blockSize, unsigned int totalSize) {
    m_blockSize = blockSize;
    m_totalSize = totalSize;
    m_iNodeSize = 16;
}

void INodeSimulation::simulate() {
//todo
}

Directory *INodeSimulation::getRootDirectory() {
    Directory* directory = m_currentDirectory;
    while(directory->getParentDirectory() != nullptr) {
        directory = directory->getParentDirectory();
    }
    return directory;
}

int INodeSimulation::getFreeDiskSpace() {
    int freeCounter = 0;
    for (int i = 0; i < getNumberOfFilesThatCanBeSaved(); i++) {
        if(m_statusArray[i] == FREE){
            freeCounter++;
        }
    }
    return freeCounter;
}

void INodeSimulation::defragmentDisk(Directory *directory, int currentPosition) {
    if(directory == nullptr){
        return;
    }

    auto* file = dynamic_cast<INodeFile*>(directory->getFileList());

    while (file != nullptr){
        int* currentTable = file->getINode()->getAddressPointers();

        int numberOfBlocks = 0;
        int index = 0;
        int tableNumber = -2;

        while(currentTable != nullptr && currentTable[index] != -1){
            if(m_statusArray[currentPosition] == FREE){
                m_statusArray[currentPosition] = RESERVED;
                m_statusArray[currentTable[index]] = FREE;
                file->getINode()->addAddressAtIndex(currentPosition, 12, index);
                m_statusArray[currentPosition] = OCCUPIED;
            }else{
                while(m_statusArray[currentPosition] == CORRUPTED || m_statusArray[currentPosition] == RESERVED){
                    currentPosition++;
                }

                if(m_statusArray[currentPosition] == FREE){
                    m_statusArray[currentPosition] = RESERVED;
                    m_statusArray[currentTable[index]] = FREE;
                    file->getINode()->addAddressAtIndex(currentPosition, 12, index);
                    m_statusArray[currentPosition] = OCCUPIED;
                }
//                else{
//                    if(currentPosition != currentTable[index]){
//                        BSCluster* secondCurrent = searchClusterByIndex(getRootDirectory(), currentPosition);
//                        int saver = current->getIndex();
//                        current->setIndex(secondCurrent->getIndex());
//                        secondCurrent->setIndex(saver);
//                    }
//                }
//todo: ggf. Erklärung  nötig, um auf INode umzustellen
            }
            currentPosition++;
            index++;
            if(index >= 12) {
                index = 0;
                if(tableNumber == -2) {
                    tableNumber++;
                    currentTable = *file->getINode()->getFirstIndirectPointers();
                } else {
                    tableNumber++;
                    currentTable = *file->getINode()->getDoubleIndirectPointers()[tableNumber];
                }
            }
        }
        file = dynamic_cast<INodeFile*>(file->getNextFile());
    }

    Directory* subDirectory = directory->getSubDirectoryList();
    while(subDirectory != nullptr){
        defragmentDisk(subDirectory, currentPosition);
        subDirectory = subDirectory->getNextDirectory();
    }
}

float INodeSimulation::getFragmentation(Directory *directory, float fragmentation) {
    if(directory == nullptr){
        return 0.0;
    }

    auto* file = dynamic_cast<INodeFile*>(directory->getFileList());
    float sumOfFrag = 0.0;

    while(file != nullptr) {
        int* currentTable = file->getINode()->getAddressPointers();

        int minIndex = currentTable[0];
        int maxIndex = currentTable[0];
        int numberOfBlocks = 0;
        int index = 0;
        int tableNumber = -2;

        while(currentTable != nullptr && currentTable[index] != -1) {
            if (currentTable[index] > maxIndex) {
                maxIndex = currentTable[index];
            } else if (currentTable[index] < minIndex) {
                minIndex = currentTable[index];
            }
            index++;
            if(index >= 12) {
                index = 0;
                if(tableNumber == -2) {
                    tableNumber++;
                    currentTable = *file->getINode()->getFirstIndirectPointers();
                } else {
                    tableNumber++;
                    currentTable = *file->getINode()->getDoubleIndirectPointers()[tableNumber];
                }
            }
            numberOfBlocks++;
        }
        int length = maxIndex - minIndex + 1;
        sumOfFrag += (float)(length - numberOfBlocks) / length;
        file = dynamic_cast<INodeFile*>(file->getNextFile());
    }

    Directory* subDirectory = directory->getSubDirectoryList();
    while(subDirectory != nullptr){

        sumOfFrag += getFragmentation(subDirectory, sumOfFrag);
        subDirectory = subDirectory->getNextDirectory();
    }

    return (sumOfFrag / (float) m_numberOfFiles);}


void INodeSimulation::createFile(char *name, bool editable, bool system, bool ascii, bool randAccFile, int size) {
    Attributes* attributes = new Attributes;
    attributes->size = size;
    attributes->dateOfCreation = time(nullptr);
    attributes->dateOfLastEdit = time(nullptr);
    char attributesByte[1];

    attributes->attributes = attributesByte;
    INode* node = new INode;

    INodeFile* test = new INodeFile(name, attributes, size, node);
    if(editable) {
        test->setBit(attributesByte, 0);
    }
    if(system) {
        test->setBit(attributesByte, 1);
    }
    if(ascii) {
        test->setBit(attributesByte, 2);
    }
    if(randAccFile) {
        test->setBit(attributesByte, 3);
    }
    Directory* directory = m_currentDirectory;
    directory->setLastFileOfTheList(test);
    while (directory->getParentDirectory() != nullptr){
        directory->setNumberOfFiles(directory->getNumberOfFiles() + 1);
        directory = directory->getParentDirectory();
    }
}

void INodeSimulation::createFilesForSim(char **names, unsigned int length) {


//todo
    Directory* directory = m_currentDirectory;
    while (directory->getParentDirectory() != nullptr){
        directory->setNumberOfFiles(directory->getNumberOfFiles() + 1);
        directory = directory->getParentDirectory();
    }
}

void INodeSimulation::createDirectoriesForSim() {
//todo
}

void INodeSimulation::createDirectory(char *name, Attributes *attributes) {
    m_currentDirectory->createChildDirectory(name, attributes);
}

void INodeSimulation::updateFile(char *name, bool isEditable, bool isSystem, bool isAscii, bool isRamFile, AbstractFile *file, int size) {
//    if(file->tstBit(file->getAttributes()->attributes, 0)){
//        if(!file->isSystem()){
//            if(strcmp(file->getName(), name) != 0){
//                file->setName(name);
//            }
//            if(isSystem != file->isSystem()){
//                file->tstBit(file->getAttributes()->attributes, 1) ? file->clrBit(file->getAttributes()->attributes, 1) : file->setBit(file->getAttributes()->attributes, 1);
//            }
//            int numberOfNewBlocks = ceil((double) size / (double) m_blockSize);
//            int numberOfCurrBlocks = ceil((double) file->getSize() / (double) m_blockSize);
//
//            auto fatFile = dynamic_cast<BSFatFile*>(file);
//
//            if (numberOfCurrBlocks < numberOfNewBlocks) {
//                if ((numberOfNewBlocks - numberOfCurrBlocks) < getFreeDiskSpace()) {
//                    BSCluster* clusterToAdd = initBSCluster(numberOfNewBlocks - numberOfCurrBlocks);
//                    BSCluster *lastCluster = fatFile->getLastBlock();
//
//                    lastCluster->setNextElement(clusterToAdd);
//                    clusterToAdd->setPrevElement(lastCluster);
//                }
//            } else if (numberOfCurrBlocks > numberOfNewBlocks) {
//                int counter = numberOfCurrBlocks - numberOfNewBlocks;
//                BSCluster* cluster = fatFile->getLastBlock();
//
//                while (counter != 0) {
//                    BSCluster* currCluster = cluster;
//                    cluster = cluster->getPrevBlock();
//
//                    currCluster->setPrevElement(nullptr);
//                    currCluster->setNextElement(nullptr);
//
//                    m_statusArray[currCluster->getIndex()] = FREE;
//
//                    delete currCluster;
//                    counter--;
//                }
//            }
//            file->setSize(size);
//        }
//        //nur attribute können gesetzt werden
//        if(isEditable != file->isEditable()){
//            file->tstBit(file->getAttributes()->attributes, 0) ? file->clrBit(file->getAttributes()->attributes, 0) : file->setBit(file->getAttributes()->attributes, 0);
//        }
//        if(isAscii != file->isAscii()){
//            file->tstBit(file->getAttributes()->attributes, 2) ? file->clrBit(file->getAttributes()->attributes, 2) : file->setBit(file->getAttributes()->attributes, 2);
//        }
//        if (isRamFile != file->isRandAccFile()){
//            file->tstBit(file->getAttributes()->attributes, 3) ? file->clrBit(file->getAttributes()->attributes, 3) : file->setBit(file->getAttributes()->attributes, 3);
//        }
//        file->getAttributes()->dateOfLastEdit = time(nullptr);
//    }
//todo
}

void INodeSimulation::updateDirectory(char* name, bool isEditable, Directory* directory) {
    if(directory->isEditable()){
        if(strcmp(name, directory->getName()) != 0){
            directory->setName(name);
        }
        if(isEditable != directory->isEditable()){
            directory->setEditable(isEditable);
        }
    }
}

const char *INodeSimulation::getPath() {
    Directory* directory = m_currentDirectory;
    std::string path = "";
    while (directory != nullptr){
        std::string preName = directory->getName();
        path = preName + "/" + path;
        directory = directory->getParentDirectory();
    }
    const char* pathAsChar = path.c_str();

    return pathAsChar;}

void INodeSimulation::freeFileMemory(AbstractFile *file) {
    INodeFile* iNodeFile = dynamic_cast<INodeFile*>(file);

    int* currentTable = iNodeFile->getINode()->getAddressPointers();

    int index = 0;
    int statusIndex = -1;
    int tableNumber = -2;

    while(currentTable != nullptr && currentTable[index] != -1) {
        m_statusArray[currentTable[index]] = FREE;
        index++;
        if(index >= 12) {
            index = 0;
            if(tableNumber == -2) {
                tableNumber++;
                int* prevTable = currentTable;
                currentTable = *iNodeFile->getINode()->getFirstIndirectPointers();
                delete prevTable;
            } else {
                tableNumber++;
                currentTable = *iNodeFile->getINode()->getDoubleIndirectPointers()[tableNumber];
            }
        }
    }
    iNodeFile->getINode()->setAddressPointers(nullptr);
    iNodeFile->getINode()->setFirstIndirectPointers(nullptr);
    iNodeFile->getINode()->setDoubleIndirectPointers(nullptr);
    delete iNodeFile->getINode();
    iNodeFile->setINode(nullptr); //todo: check, ob alles gelöscht wird
}

void INodeSimulation::deleteFile(AbstractFile *file) {
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

void INodeSimulation::deleteDirectory(Directory *directory) {
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

Directory *INodeSimulation::getCurrentDirectory() {
    return m_currentDirectory;
}

void INodeSimulation::setCurrentDirectory(Directory *directory) {
    m_currentDirectory = directory;
}

unsigned char *INodeSimulation::getStatusArray() {
    return m_statusArray;
}

unsigned int INodeSimulation::getBlockSize() {
    return m_blockSize;
}

unsigned int INodeSimulation::getTotalSize() {
    return m_totalSize;
}

unsigned int INodeSimulation::getNumberOfFilesThatCanBeSaved() {
    return m_iNodeSize / m_blockSize;
}

unsigned int INodeSimulation::getNumberOfCurrentlySavedFiles() {
    return m_numberOfFiles;
}

unsigned int INodeSimulation::getINodeSize() {
    return m_iNodeSize;
}

unsigned int INodeSimulation::getNumberOfINodes() {
    return m_numberOfBlocksPerINode;
}

INodeFile* INodeSimulation::convertFatToINode(BSFatFile* bsFatFile, int bsBlockSize) {
    INode* iNode = new INode(bsFatFile->getSize() / bsBlockSize, 12);
    int* currentTable = iNode->getAddressPointers();

    int numberOfBlocks = 0;
    int iNodeIndex = 0;
    int tableNumber = -2;
    BSCluster* currentCluster = bsFatFile->getFirstBlock();

    while(currentTable != nullptr && currentCluster != nullptr) {
        currentTable[iNodeIndex] = currentCluster->getIndex();
        iNodeIndex++;
        if(iNodeIndex >= 12) {
            iNodeIndex = 0;
            if(tableNumber == -2) {
                tableNumber++;
                currentTable = *iNode->getFirstIndirectPointers();
            } else {
                tableNumber++;
                currentTable = *iNode->getDoubleIndirectPointers()[tableNumber];
            }
        }
        numberOfBlocks++;
    }
    return new INodeFile(bsFatFile->getName(), bsFatFile->getAttributes(), bsFatFile->getSize(), iNode);
}

void INodeSimulation::copyFileFromROM(BSFatFile* bsFatFile, Directory* directory) {
    INodeFile* newFile = convertFatToINode(bsFatFile, 512);
    directory->setLastFileOfTheList(newFile);
    //todo: size etc. in Directories setzen
}
