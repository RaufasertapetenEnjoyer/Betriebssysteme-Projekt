//
// Created by jntre on 01.08.2023.
//

#include "INodeSimulation.h"
#include "INode.h"
#include "INodeFile.h"
#include "ctime"
#include <cstring>
#include "iostream"
#include "../CDRomStructure/CDRomDirectory.h"


INodeSimulation::INodeSimulation(unsigned int blockSize, unsigned int totalSize, char* name) {
    m_name = name;
    m_statusArray = new unsigned char[totalSize / blockSize];
    std::cout << "create INodeSim" << '\n' << std::endl;
    m_blockSize = blockSize;
    m_totalSize = totalSize;
    m_numberOfFiles = 0;

    std::cout << "totalSize" << totalSize << std::endl;
    std::cout << "blockSize" << blockSize << std::endl;
    for (int i = 0; i < (totalSize/blockSize); i++) {
        std::cout << "assign to index " << i << std::endl;
        m_statusArray[i] = FREE;
    }

    simulate();
    m_currentDirectory = getRootDirectory();
    std::cout << m_currentDirectory->getName() << std::endl;
}

void INodeSimulation::simulate() {

    std::cout << getNumberOfFilesThatCanBeSaved() << '\n' << std::endl;
    for (int i = 0; i < 40; i++) {
        if((rand() % 2) + 1 == 1){
            m_statusArray[rand() % getNumberOfFilesThatCanBeSaved()] = CORRUPTED;
        }else{
            m_statusArray[rand() % getNumberOfFilesThatCanBeSaved()] = RESERVED;
        }
    }


    char* root =  {"root"};
    Attributes * attributes = new Attributes();
    attributes->dateOfCreation = time(nullptr);
    attributes->dateOfLastEdit = time(nullptr);
    attributes->attributes = new char[1];
    auto* directory = new Directory(root, attributes);
    directory->setBit(directory->getAttributes()->attributes,0);
    std::cout << directory->getName() << '\n' << std::endl;
    directory->setParentDirectory(nullptr);
    m_currentDirectory = directory;

    //char** fileNamesForRoot = new char*[] {"programm1.c", "2programm.c", "prog3.c.cpp", "p4rogramm1.c", "program5m.c", "pr6ogramm.c", "progra7mm.c", "programm8.c", "progr9amm.c", "program.cpp"};

    char* fileNamesForRoot[] = { "program5m.c", "pr6ogramm.c", "progra7mm.c", "programm8.c", "progr9amm.c", "program.cpp"};
    std::cout << directory->getName() << '\n' << std::endl;
    createFilesForSim(fileNamesForRoot, 6);
    createDirectoriesForSim();
}

Directory *INodeSimulation::getRootDirectory() {
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
        int navigationIndex = 0;
        int tableNumber = -2;

        while(currentTable != nullptr && currentTable[index] != -1) {
            if(m_statusArray[currentPosition] == FREE){
                m_statusArray[currentPosition] = RESERVED;
                m_statusArray[currentTable[index]] = FREE;
                file->getINode()->addAddressAtIndex(currentPosition, index);
                m_statusArray[currentPosition] = OCCUPIED;
            }else{
                while(m_statusArray[currentPosition] == CORRUPTED || m_statusArray[currentPosition] == RESERVED){
                    currentPosition++;
                }

                if(m_statusArray[currentPosition] == FREE){
                    m_statusArray[currentPosition] = RESERVED;
                    m_statusArray[currentTable[index]] = FREE;
                    file->getINode()->addAddressAtIndex(currentPosition, navigationIndex);
                    m_statusArray[currentPosition] = OCCUPIED;
                }else{
                    if(currentPosition != currentTable[index]){
                        INode* secondCurrent = searchINodesByIndex(getRootDirectory(), currentPosition);
                        if(secondCurrent == nullptr) {
                            break;
                        }
                        secondCurrent->findAndReplaceAddress(currentPosition, currentTable[index]);
                        file->getINode()->addAddressAtIndex(currentPosition, navigationIndex);
                    }
                }
            }
            currentPosition++;
            index++;
            navigationIndex++;
            if(index >= 12) {
                index = 0;
                if(tableNumber == -2 && file->getINode()->getFirstIndirectPointers() != nullptr) {
                    tableNumber++;
                    currentTable = *file->getINode()->getFirstIndirectPointers();
                } else if (file->getINode()->getDoubleIndirectPointers() != nullptr){
                    tableNumber++;
                    currentTable = *file->getINode()->getDoubleIndirectPointers()[tableNumber];
                } else {
                    currentTable = nullptr;
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

void INodeSimulation::getFragmentation(Directory *directory, float& fragmentation) {
    if(directory == nullptr){
        return;
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
                if(tableNumber == -2 && file->getINode()->getFirstIndirectPointers() != nullptr) {
                    tableNumber++;
                    currentTable = *file->getINode()->getFirstIndirectPointers();
                } else if (file->getINode()->getDoubleIndirectPointers() != nullptr){
                    tableNumber++;
                    currentTable = *file->getINode()->getDoubleIndirectPointers()[tableNumber];
                } else {
                    currentTable = nullptr;
                }
            }
            numberOfBlocks++;
        }
        int length = maxIndex - minIndex + 1;
        sumOfFrag += (float)(length - numberOfBlocks) / length;
        file = dynamic_cast<INodeFile*>(file->getNextFile());
    }

    fragmentation += sumOfFrag;
    Directory* subDirectory = directory->getSubDirectoryList();
    while(subDirectory != nullptr){

        getFragmentation(subDirectory, fragmentation);
        subDirectory = subDirectory->getNextDirectory();
    }

}


void INodeSimulation::createFile(char *name, bool editable, bool system, bool ascii, bool randAccFile, int size) {
    Attributes* attributes = new Attributes;
    attributes->dateOfCreation = time(nullptr);
    attributes->dateOfLastEdit = time(nullptr);
    attributes->attributes = new char[1];

    int numberOfBlocks = ceil((double)size / (double)m_blockSize);

    INode* node = new INode(numberOfBlocks);

    INodeFile* newFile = new INodeFile(name, attributes, size, node);
    newFile->setEditable(editable);
    newFile->setSystem(system);
    newFile->setAscii(ascii);
    newFile->setRandAccFile(randAccFile);

    Directory* directory = m_currentDirectory;
    m_currentDirectory->createChildFile(newFile);

    while (directory != nullptr){
        directory->setNumberOfFiles(directory->getNumberOfFiles() + 1);
        directory = directory->getParentDirectory();
    }

    unsigned int pos;

    for (int i = 0; i < numberOfBlocks; i++) {
        pos = (rand() % (m_totalSize / m_blockSize));
        while (m_statusArray[pos] != FREE) {
            pos = (rand() % (m_totalSize / m_blockSize));
        }
        node->addAddress(pos);
        m_statusArray[pos] = OCCUPIED;
    }

    m_numberOfFiles++;

}

void INodeSimulation::createFilesForSim(char **names, unsigned int length) {
    for (int i = 0; i < length; i++) {
        createFile(names[i], true, false, true, false, (int) (rand() % 2100 + 1));
    }
}

void INodeSimulation::createDirectoriesForSim() {
    m_currentDirectory = getRootDirectory();

    char* directoryNames[] = {"Michael", "Jan", "Simon", "DigitaleSysteme", "Physik", "Algorithmen", "Datenstrukturen", "Elektrotechnik", "Praktikumsabgaben","Projekt BS", "Links"};

    //user-directory Michael
    //      with four files in it (main.c, test.txt and Dokumentation.docx, log.txt)
    //      with two subdirectories (Digitale Systeme and Physik)
    //Subdirectory Digitale Systeme
    //      with two files
    //Subdirectory Projekt is empty
    //Subdirectory Physik
    //      with one file in it
    char* fileNamesMichael[] =  {"main.c", "test.txt", "Dokumentation.docx", "log.txt"};
    char* fileNameDigitaleSysteme[] = {"Arduino.c", "LineFollower.c"};
    char* fileNamesPhysik[] = {"Diagramme.csv"};
    Attributes** attributes = new Attributes*[12];

    for(int i=0; i<12; i++){
        attributes[i] = new Attributes();
        attributes[i]->dateOfCreation = time(nullptr);
        attributes[i]->dateOfLastEdit = time(nullptr);
        attributes[i]->attributes = new char[1];
    }

    m_currentDirectory->createChildDirectory(directoryNames[0], attributes[0]);
    std::cout<<m_currentDirectory->getSubDirectoryList()->getName()<<std::endl;
    m_currentDirectory = m_currentDirectory->getSubDirectoryList();


    //Michael/
    m_currentDirectory->createChildDirectory(directoryNames[3], attributes[1]);
    std::cout<<m_currentDirectory->getSubDirectoryList()->getName()<<std::endl;
    m_currentDirectory->createChildDirectory(directoryNames[4], attributes[2]);

    createFilesForSim(fileNamesMichael, 4);

    //Michael/Digitale Systeme
    m_currentDirectory = m_currentDirectory->getSubDirectoryList();
    createFilesForSim(fileNameDigitaleSysteme, 2);
    m_currentDirectory->createChildDirectory(directoryNames[10], attributes[3]);

    //Michael/Physik
    m_currentDirectory = m_currentDirectory->getNextDirectory();
    createFilesForSim(fileNamesPhysik, 1);
    m_currentDirectory = getRootDirectory();

    //user-directory Jan
    //      with two subdirectories (Algorithmen und Datenstrukturen)
    //Subdirectory Algorithmen
    //      with one file in it
    //Subdirectory Datenstrukturen
    //      with two files
    char* fileNamesAlgorithmen[] = {"Skript.docx"};
    char* fileNamesDatenstrukturen[] = {"VerketteteListe.pdf", "Schaubild.png"};
    m_currentDirectory->createChildDirectory(directoryNames[1], attributes[4]);

    //Jan/
    m_currentDirectory = m_currentDirectory->getSubDirectoryList()
            ->getNextDirectory();

    m_currentDirectory->createChildDirectory(directoryNames[5], attributes[5]);
    m_currentDirectory->createChildDirectory(directoryNames[6], attributes[6]);

    //Jan/Algorithmen
    m_currentDirectory = m_currentDirectory->getSubDirectoryList();

    createFilesForSim(fileNamesAlgorithmen, 1);

    //Jan/Datenstrukturen
    m_currentDirectory = m_currentDirectory->getNextDirectory();

    createFilesForSim(fileNamesDatenstrukturen, 2);

    m_currentDirectory = getRootDirectory();


    //user-directory Simon
    //      with two subdirectory (com, bsProjekt)
    //Subdirectory com
    //      with one subdirectory (de)
    //Subdirectory de
    //      with one subdirectory (bs)
    //Subdirectory bs
    //      with one subdirectory (BsProjekt)
    //Subdirectory BsProjekt
    //      with three files
    char* fileNamesPraktikum[] = {"Abgabe1.docx", "Abgabe2.docx", "Abgabe3.docx", "Abgabe4.docx", "Abgabe5.docx", };
    char* fileNamesBsProjekt[] = {"main.cpp", "icon1.png", "mainWindow.qt"};
    char* directoryNamesSimon[] = {"com", "de", "bs", "bsProjekt"};
    m_currentDirectory->createChildDirectory(directoryNames[2], attributes[7]);

    //Simon/
    m_currentDirectory = m_currentDirectory->getSubDirectoryList()
            ->getNextDirectory()
            ->getNextDirectory();
    std::cout<<m_currentDirectory->getName()<<std::endl;
    m_currentDirectory->createChildDirectory(directoryNamesSimon[0], attributes[8]);


    //Simon/com
    m_currentDirectory = m_currentDirectory->getSubDirectoryList();
    m_currentDirectory->createChildDirectory(directoryNamesSimon[1], attributes[9]);
    std::cout<<m_currentDirectory->getName()<<std::endl;

    //Simon/com/de
    m_currentDirectory = m_currentDirectory->getSubDirectoryList();
    m_currentDirectory->createChildDirectory(directoryNamesSimon[2], attributes[10]);

    //Simon/com/de/bs
    m_currentDirectory = m_currentDirectory->getSubDirectoryList();
    m_currentDirectory->createChildDirectory(directoryNamesSimon[3], attributes[11]);

    //Simon/com/de/bs/bsProjekt
    m_currentDirectory = m_currentDirectory->getSubDirectoryList();
    createFilesForSim(fileNamesBsProjekt,3);


    m_currentDirectory = getRootDirectory();
    std::cout<<m_currentDirectory->getName()<<std::endl;
}

void INodeSimulation::createDirectory(char *name, Attributes *attributes) {
    m_currentDirectory->createChildDirectory(name, attributes);
}

void INodeSimulation::updateFile(char *name, bool isEditable, bool isSystem, bool isAscii, bool isRamFile, AbstractFile *file, int size) {
    if(file->tstBit(file->getAttributes()->attributes, 0)){
        if(!file->isSystem()){
            if(strcmp(file->getName(), name) != 0){
                file->setName(name);
            }
            if(isSystem != file->isSystem()){
                file->tstBit(file->getAttributes()->attributes, 1) ? file->clrBit(file->getAttributes()->attributes, 1) : file->setBit(file->getAttributes()->attributes, 1);
            }
            int numberOfNewBlocks = ceil((double) size / (double) m_blockSize);
            int numberOfCurrBlocks = ceil((double) file->getSize() / (double) m_blockSize);

            auto iNodeFile = dynamic_cast<INodeFile*>(file);

            if (numberOfCurrBlocks < numberOfNewBlocks) {

                int pos;

                for (int i = 0; i < numberOfNewBlocks - numberOfCurrBlocks; i++) {
                    pos = (rand() % (m_totalSize / m_blockSize));
                    while (m_statusArray[pos] != FREE) {
                        pos = (rand() % (m_totalSize / m_blockSize));
                    }
                    iNodeFile->getINode()->addAddress(pos);
                    m_statusArray[pos] = OCCUPIED;
                }
            } else if (numberOfCurrBlocks > numberOfNewBlocks) {
                iNodeFile->getINode()->deleteAddressFromIndex(numberOfNewBlocks - 1);
            }
            file->setSize(size);
        }
        //nur attribute können gesetzt werden
        if(isEditable != file->isEditable()){
            file->tstBit(file->getAttributes()->attributes, 0) ? file->clrBit(file->getAttributes()->attributes, 0) : file->setBit(file->getAttributes()->attributes, 0);
        }
        if(isAscii != file->isAscii()){
            file->tstBit(file->getAttributes()->attributes, 2) ? file->clrBit(file->getAttributes()->attributes, 2) : file->setBit(file->getAttributes()->attributes, 2);
        }
        if (isRamFile != file->isRandAccFile()){
            file->tstBit(file->getAttributes()->attributes, 3) ? file->clrBit(file->getAttributes()->attributes, 3) : file->setBit(file->getAttributes()->attributes, 3);
        }
        file->getAttributes()->dateOfLastEdit = time(nullptr);
    }
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

    return pathAsChar;
}


void INodeSimulation::freeFileMemory(AbstractFile *file) {
    INodeFile* iNodeFile = dynamic_cast<INodeFile*>(file);

    int* currentTable = iNodeFile->getINode()->getAddressPointers();
    int* prevTable;

    int index = 0;
    int tableNumber = -2;

    while(currentTable != nullptr && currentTable[index] != -1) {
        m_statusArray[currentTable[index]] = FREE;
        index++;
        if(index == 12) {
            index = 0;
            if(tableNumber == -2 && iNodeFile->getINode()->getFirstIndirectPointers() != nullptr) {
                tableNumber++;
                currentTable = *iNodeFile->getINode()->getFirstIndirectPointers();
            } else if (iNodeFile->getINode()->getDoubleIndirectPointers() != nullptr){
                tableNumber++;
                currentTable = *iNodeFile->getINode()->getDoubleIndirectPointers()[tableNumber];
            } else {
                currentTable = nullptr;
            }
        }
    }

    delete iNodeFile->getINode()->getAddressPointers();
    delete iNodeFile->getINode()->getFirstIndirectPointers();

    /*for (int i = 0; i < 12; i++) {
        delete *iNodeFile->getINode()->getDoubleIndirectPointers()[i];
    }*/
    delete iNodeFile->getINode()->getDoubleIndirectPointers();

    delete iNodeFile->getINode();
    iNodeFile->setINode(nullptr);
}

void INodeSimulation::deleteFile(AbstractFile *file) {
    std::cout<<file->getName()<<std::endl;
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
            return;
        }else if(directory->getNextDirectory() != nullptr && directory->getPrevDirectory() == nullptr){
            Directory* nextDirectory = directory->getNextDirectory();
            nextDirectory->setPreviousDirectory(nullptr);

            m_currentDirectory->setDirectoryList(nextDirectory);

            directory->setNextDirectory(nullptr);

            delete directory;
            return;
        }else if(directory->getNextDirectory() != nullptr && directory->getPrevDirectory() != nullptr){
            Directory* nextDirectory = directory->getNextDirectory();
            Directory* prevDirectory = directory->getPrevDirectory();
            nextDirectory->setPreviousDirectory(prevDirectory);
            prevDirectory->setNextDirectory(nextDirectory);

            directory->setPreviousDirectory(nullptr);
            directory->setNextDirectory(nullptr);

            delete directory;
        }else if(directory->getNextDirectory() == nullptr && directory->getPrevDirectory() == nullptr){
            m_currentDirectory->setDirectoryList(nullptr);

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
    return m_totalSize / m_blockSize;
}

unsigned int INodeSimulation::getNumberOfCurrentlySavedFiles() {
    return m_numberOfFiles;
}

void INodeSimulation::updateEditableOnContent(Directory *directory) {

}

bool INodeSimulation::checkIfEditIsValid(char *name, bool isEditable, bool isSystem, bool isAscii, bool isRamFile,
                                         AbstractFile *file, int size) {
    return false;
}

INodeFile* INodeSimulation::convertROMToINode(CDRomDirectory* directoryToCopy, int bsBlockSize) {
//    INode* iNode = new INode(bsFatFile->getSize() / bsBlockSize);
//    int* currentTable = iNode->getAddressPointers();
//
//    int numberOfBlocks = 0;
//    int iNodeIndex = 0;
//    int tableNumber = -2;
//    BSCluster* currentCluster = bsFatFile->getFirstBlock();
//
//    while(currentTable != nullptr && currentCluster != nullptr) {
//        currentTable[iNodeIndex] = currentCluster->getIndex();
//        iNodeIndex++;
//        if(iNodeIndex >= 12) {
//            iNodeIndex = 0;
//            if(tableNumber == -2 && iNodeFile->getINode()->getFirstIndirectPointers() != nullptr) {
//                tableNumber++;
//                currentTable = *iNode->getFirstIndirectPointers();
//            } else (iNodeFile->getINode()->getDoubleIndirectPointers() != nullptr){
//                tableNumber++;
//                currentTable = *iNode->getDoubleIndirectPointers()[tableNumber];
//            } else {
//                currentTable = nullptr;
//            {
//        }
//        numberOfBlocks++;
//    }
//    return new INodeFile(bsFatFile->getName(), bsFatFile->getAttributes(), bsFatFile->getSize(), iNode);
}

void INodeSimulation::copyCDRomDirectory(CDRomDirectory* directoryToCopy, const int cdRomBlockSize){
//    INodeFile* newFile = convertFatToINode(bsFatFile, 512);
//    directory->setLastFileOfTheList(newFile);
    //todo: size etc. in Directories setzen
}

char *INodeSimulation::getName() {
    return m_name;
}

INode *INodeSimulation::searchINodesByIndex(Directory *directory, unsigned int index) {
    if(directory == nullptr){
        return nullptr;
    }

    auto* current = dynamic_cast<INodeFile*>(directory->getFileList());

    while(current != nullptr) {
        if(current->getINode()->checkIfAddressExists(index)) {
            return current->getINode();
        } else {
            current = dynamic_cast<INodeFile*>(current->getNextFile());
        }
    }

    INode* target = nullptr;
    Directory* subDirectory = directory->getSubDirectoryList();
    while(subDirectory != nullptr && target == nullptr){

        target = searchINodesByIndex(subDirectory, index);
        subDirectory = subDirectory->getNextDirectory();
    }
    return target;
}
