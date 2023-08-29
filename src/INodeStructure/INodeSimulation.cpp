//
// Created by jntre on 01.08.2023.
//

#include "INodeSimulation.h"
#include "INode.h"
#include "INodeFile.h"
#include "ctime"
#include <cstring>
#include <QString>
#include "iostream"
#include "../CDRomStructure/CDRomDirectory.h"

/**
 * A file has four different flags that can be set as attributes. The flags are editable (index 0),
 * Systemfile (index 1), Asccii-File (index 2) und  Random-Access-File (index 3)
 */

/**
 * Creates an INode simulation, initializes all parameters and creates some directories and files.
 * @param unsigned int, blockSize size of a memory-block
 * @param unsigned totalSize, total memory for the simulation
 * @param char* name, name of the drive
 */
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
        m_statusArray[i] = FREE;
    }

    simulate();
    m_currentDirectory = getRootDirectory();
    std::cout << m_currentDirectory->getName() << std::endl;
}

/**
 * Simulates the file-system with some directories and files.
 */
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

/**
 * Returns a pointer on the root-directory.
 * @return Directory* root
 */
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

/**
 * Calculates the free disk space as a number of blocks (marked with the Flag FREE).
 * @return int diskSpace
 */
int INodeSimulation::getFreeDiskSpace() {
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
 * @param currentPosition pointer on the current index in the status-array
 */
void INodeSimulation::defragmentDisk(Directory *directory, int &currentPosition) {
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
                file->getINode()->addAddressAtIndex(currentPosition, navigationIndex);
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
                        int* secondCurrent = searchINodesByIndex(getRootDirectory(), currentPosition);
                        if(secondCurrent == nullptr) {
                            std::cout<<"wrong"<<std::endl;

                        }
                        int saver = currentTable[index];
                        file->getINode()->addAddressAtIndex(currentPosition, navigationIndex);
                        *secondCurrent = saver;
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
                    currentTable = file->getINode()->getFirstIndirectPointers();
                } else if (file->getINode()->getDoubleIndirectPointers() != nullptr){
                    tableNumber++;
                    currentTable = file->getINode()->getDoubleIndirectPointers()[tableNumber];
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

/**
 * Calculates the fragmentation of the whole disk (recursive).
 * @param Directory* directory (start from root)
 * @param float fragmentation (start with 0.0)
 * @return
 */
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
                    currentTable = file->getINode()->getFirstIndirectPointers();
                } else if (file->getINode()->getDoubleIndirectPointers() != nullptr){
                    tableNumber++;
                    if (tableNumber < 12) {
                        currentTable = currentTable = file->getINode()->getDoubleIndirectPointers()[tableNumber];
                    } else {
                        currentTable = nullptr;
                    }
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

/**
 * Creates a file beneath the current directory.
 * @param char* name
 * @param Attributes* attributes
 * @param int size
 */
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

/**
 * Creates some files with random size under the current directory.
 * @param char** names, name array
 * @param length, length of the array
 */
void INodeSimulation::createFilesForSim(char **names, unsigned int length) {
    for (int i = 0; i < length; i++) {
        createFile(names[i], true, false, true, false, (int) (rand() % 13000 + 1));
    }
}

/**
 * Creates some directories for the simulation.
 */
void INodeSimulation::createDirectoriesForSim() {
    m_currentDirectory = getRootDirectory();

    char* directoryNames[] = {"Michael", "Jan", "Simon", "DigitaleSysteme", "Physik", "Algorithmen", "Datenstrukturen", "Elektrotechnik", "Praktikumsabgaben", "Links"};

    //user-directory Michael
    //      with four files in it (main.c, test.txt and Dokumentation.docx, log.txt)
    //      with two subdirectories (Digitale Systeme and Physik)
    //Subdirectory Digitale Systeme
    //      with two files
    //      with one subdirectory (Links)
    //Subdirectory Projekt is empty
    //Subdirectory Physik
    //      with one file in it
    char* fileNamesMichael[] =  {"main.c", "test.txt", "Dokumentation.docx", "log.txt"};
    char* fileNameDigitaleSysteme[] = {"Arduino.c", "LineFollower.c"};
    char* fileNamesPhysik[] = {"Diagramme.csv"};
    Attributes** attributes = new Attributes*[14];

    for(int i=0; i<14; i++){
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
    m_currentDirectory->createChildDirectory(directoryNames[9], attributes[3]);

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
    //      with three subdirectories (com, Elektrotechnik, Praktikumsabgaben)
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
    m_currentDirectory->createChildDirectory(directoryNames[7], attributes[9]);
    m_currentDirectory->createChildDirectory(directoryNames[8], attributes[10]);

    //Simon/com
    m_currentDirectory = m_currentDirectory->getSubDirectoryList();
    m_currentDirectory->createChildDirectory(directoryNamesSimon[1], attributes[11]);
    std::cout<<m_currentDirectory->getName()<<std::endl;

    //Simon/com/de
    m_currentDirectory = m_currentDirectory->getSubDirectoryList();
    m_currentDirectory->createChildDirectory(directoryNamesSimon[2], attributes[12]);

    //Simon/com/de/bs
    m_currentDirectory = m_currentDirectory->getSubDirectoryList();
    m_currentDirectory->createChildDirectory(directoryNamesSimon[3], attributes[13]);

    //Simon/com/de/bs/bsProjekt
    m_currentDirectory = m_currentDirectory->getSubDirectoryList();
    createFilesForSim(fileNamesBsProjekt,3);


    m_currentDirectory = getRootDirectory();
    std::cout<<m_currentDirectory->getName()<<std::endl;
}

/**
 * Creates an subdirectory under the current directory.
 * @param char* name, name of the directory
 * @param Attributes* attributes
 */
void INodeSimulation::createDirectory(char *name, Attributes *attributes) {
    m_currentDirectory->createChildDirectory(name, attributes);
}

/**
 * Updates the attributes of a given file.
 * @param char* name
 * @param bool isEditable
 * @param bool isSystem
 * @param bool isAscii
 * @param bool isRamFile
 * @param AbstractFile* file, file that gets updated
 * @param int size
 */
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

/**
 * @brief updates a directory
 * @param char* name
 * @param bool isEditable
 * @param Directory * directory
 * @return bool success
 */
bool INodeSimulation::updateDirectory(char* name, bool isEditable, Directory* directory) {
    if(directory->isEditable()){
        if(strcmp(name, directory->getName()) != 0){
            directory->setName(name);
        }
        if(isEditable != directory->isEditable()){
            updateEditableOnContent(directory);
        }
        return true;
    }
    else{
        return false;
    }
}

/**
 * Returns the path of the current directory
 * @return const char* path
 */
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

/**
 * Frees the memory of the given file, so that it can be savely deleted
 * @param AbstractFile*  file
 */
void INodeSimulation::freeFileMemory(AbstractFile *file) {
    INodeFile* iNodeFile = dynamic_cast<INodeFile*>(file);

    int* currentTable = iNodeFile->getINode()->getAddressPointers();

    int index = 0;
    int tableNumber = -2;

    while(currentTable != nullptr && currentTable[index] != -1) {
        m_statusArray[currentTable[index]] = FREE;
        index++;
        if(index == 12) {
            index = 0;
            if(tableNumber == -2 && iNodeFile->getINode()->getFirstIndirectPointers() != nullptr) {
                tableNumber++;
                currentTable = iNodeFile->getINode()->getFirstIndirectPointers();
            } else if (iNodeFile->getINode()->getDoubleIndirectPointers() != nullptr){
                tableNumber++;
                if (tableNumber < 12) {
                    currentTable = currentTable = iNodeFile->getINode()->getDoubleIndirectPointers()[tableNumber];
                } else {
                    currentTable = nullptr;
                }
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

/**
 * Deletes the given file from the current directory.
 * @param AbstractFile* file
 */
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

/**
 * Deletes the given directory under the current directory, if and only if the directory is empty.
 * @param directory
 * @return bool success
 */
bool INodeSimulation::deleteDirectory(Directory *directory) {
    if(directory->getSubDirectoryList() == nullptr && directory->getFileList() == nullptr){
        if(directory->getNextDirectory() == nullptr && directory->getPrevDirectory() != nullptr){
            Directory* preDirectory = directory->getPrevDirectory();
            preDirectory->setNextDirectory(nullptr);

            directory->setPreviousDirectory(nullptr);

            delete directory;
            return true;
        }else if(directory->getNextDirectory() != nullptr && directory->getPrevDirectory() == nullptr){
            Directory* nextDirectory = directory->getNextDirectory();
            nextDirectory->setPreviousDirectory(nullptr);

            m_currentDirectory->setDirectoryList(nextDirectory);

            directory->setNextDirectory(nullptr);

            delete directory;
            return true;
        }else if(directory->getNextDirectory() != nullptr && directory->getPrevDirectory() != nullptr){
            Directory* nextDirectory = directory->getNextDirectory();
            Directory* prevDirectory = directory->getPrevDirectory();
            nextDirectory->setPreviousDirectory(prevDirectory);
            prevDirectory->setNextDirectory(nextDirectory);

            directory->setPreviousDirectory(nullptr);
            directory->setNextDirectory(nullptr);

            delete directory;
            return true;
        }else if(directory->getNextDirectory() == nullptr && directory->getPrevDirectory() == nullptr){
            m_currentDirectory->setDirectoryList(nullptr);

            delete directory;
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

/**
 * Returns the attribute m_currentDirectory
 * @return Directory* directory
 */
Directory *INodeSimulation::getCurrentDirectory() {
    return m_currentDirectory;
}

/**
 * Sets the attribute m_currentDirectory.
 * @param Directory* directoryToSet
 */
void INodeSimulation::setCurrentDirectory(Directory *directory) {
    m_currentDirectory = directory;
}

/**
 * Returns the attribute m_statusArray
 * @return unsigned char* statusArray
 */
unsigned char *INodeSimulation::getStatusArray() {
    return m_statusArray;
}

/**
 * Returns the attribute m_blocksize
 * @return unsigned int blockSize
 */
unsigned int INodeSimulation::getBlockSize() {
    return m_blockSize;
}

/**
 * Returns the attribute m_totalSize
 * @return unsigned int size
 */
unsigned int INodeSimulation::getTotalSize() {
    return m_totalSize;
}

/**
 * calculates the number of blocks in the simulation
 * @return unsigned int numberOfBlocks
 */
unsigned int INodeSimulation::getNumberOfFilesThatCanBeSaved() {
    return m_totalSize / m_blockSize;
}

/**
 * Returns the attribute m_numberOfFiles
 * @return unsigned int numberOfFiles
 */
unsigned int INodeSimulation::getNumberOfCurrentlySavedFiles() {
    return m_numberOfFiles;
}

/**
 * Changes all files and directories beneath the given directory from editable to not-editable.
 * @param Directory* directory
 */
void INodeSimulation::updateEditableOnContent(Directory *directory) {
    if(directory == nullptr){
        return;
    }

    directory->clrBit(directory->getAttributes()->attributes, 0);

    AbstractFile* file = directory->getFileList();
    while (file != nullptr){
        file->clrBit(file->getAttributes()->attributes, 0);
        file = file->getNextFile();
    }

    Directory* subDirectory = directory->getSubDirectoryList();
    while(subDirectory != nullptr){
        updateEditableOnContent(subDirectory);
        subDirectory = subDirectory->getNextDirectory();
    }
}

/**
 * Checks whether the update is valid.
 * @param char* name
 * @param bool isEditable
 * @param bool isSystem
 * @param bool isAscii
 * @param bool isRamFile
 * @param AbstractFile* file
 * @param int size
 * @return true if edit is valid
 */
bool INodeSimulation::checkIfEditIsValid(char *name, bool isEditable, bool isSystem, bool isAscii, bool isRamFile,
                                         AbstractFile *file, int size) {
    auto* bsFatFile = dynamic_cast<BSFatFile*>(file);

    if(file->getSize() < size){
        int numberOfNeededBlocks = ceil((double) (size - file->getSize()) / (double ) m_blockSize);
        if(numberOfNeededBlocks > getFreeDiskSpace()){
            return false;
        }
    }
    if(!bsFatFile->testConvention(name)){
        return false;
    }
    if(!bsFatFile->isEditable()){
        return false;
    }
    if (bsFatFile->isSystem()){
        if(strcmp(bsFatFile->getName(), name) != 0 || bsFatFile->getSize() != size || isSystem != bsFatFile->isSystem()){
            return false;
        }
    }
    return true;
}

/**
 * Copy a given file to the current directory of the INode-Simulation.
 * @param CDRomDirectory* directoryToCopy
 * @param const int cdRomBlockSize
 * @return bool success
 */
bool INodeSimulation::copyCDRomFile(CDRomFile* cdRomFile, const int cdRomBlockSize){
    int numberOfBlocksForINode = ceil((double) cdRomFile->getSize() / (double) m_blockSize);
    int numberOfBlocksForCDRom = ceil((double) cdRomFile->getSize() / (double) cdRomBlockSize);

    if(numberOfBlocksForINode <= getFreeDiskSpace()){
        //Copy name
        QString name(cdRomFile->getName());
        char *nameOfFile = new char[name.length()];
        strcpy(nameOfFile, cdRomFile->getName());
        //Copy attributes
        Attributes* attributesOfFile = new Attributes();
        memccpy(attributesOfFile, cdRomFile->getAttributes(),1, sizeof(Attributes));

        INode* newNode= new INode(numberOfBlocksForINode);

        int stagedBlocksOfCDRom = 0;
        int counter = 0;
        int length = numberOfBlocksForINode + cdRomFile->getFirstBlock();

        for (int i = cdRomFile->getFirstBlock(); i <= length; i++) {
            stagedBlocksOfCDRom++;
            if(stagedBlocksOfCDRom * cdRomBlockSize == m_blockSize || (i == length)){

                unsigned int pos;
                pos = (rand() % (m_totalSize / m_blockSize));
                while (m_statusArray[pos] != FREE) {
                    pos = (rand() % (m_totalSize / m_blockSize));
                }
                newNode->addAddress(pos);
                m_statusArray[pos] = OCCUPIED;

                stagedBlocksOfCDRom = 0;
            }
        }
        auto* copiedFile = new INodeFile(nameOfFile, attributesOfFile, cdRomFile->getSize(), newNode);
        auto* file = dynamic_cast<AbstractFile*>(copiedFile);
        m_currentDirectory->createChildFile(file);
        m_numberOfFiles++;
        Directory* directory = m_currentDirectory;
        while (directory != nullptr){
            directory->setNumberOfFiles(directory->getNumberOfFiles() + 1);
            directory = directory->getParentDirectory();
        }
        return true;
    } else {
        return false;
    }
}

/**
* Copy angiven directory with all its content to the current directory of the INode-Simulation.
* @param CDRomDirectory* directoryToCopy
* @param const int cdRomBlockSize
*/
void INodeSimulation::copyCDRomDirectory(CDRomDirectory* directoryToCopy, const int cdRomBlockSize){
    if(directoryToCopy == nullptr){
        return;
    }
    QString name(directoryToCopy->getName());
    char* nameOfDirectory = new char[name.length()];
    strcpy(nameOfDirectory, directoryToCopy->getName());

    Attributes* attributesOfDirectory = new Attributes();
    memccpy(attributesOfDirectory, directoryToCopy->getAttributes(), 1, sizeof(Attributes));

    createDirectory(nameOfDirectory, attributesOfDirectory);

    m_currentDirectory = m_currentDirectory->getLastDirectoryOfTheList();
    std::cout<<"curr Dir:"<<m_currentDirectory->getName()<<std::endl;

    AbstractElementCDRom* element = directoryToCopy->getList();
    while(element != nullptr){
        if(dynamic_cast<CDRomDirectory*>(element)){
            auto* directory = dynamic_cast<CDRomDirectory*>(element);
            copyCDRomDirectory(directory, cdRomBlockSize);
        } else{
            auto* file = dynamic_cast<CDRomFile*>(element);
            copyCDRomFile(file, cdRomBlockSize);
        }
        element = element->getNextElement();
    }
    m_currentDirectory = m_currentDirectory->getParentDirectory();
}

/**
 * Returns the attribute m_name
 * @return char* name
 */
char *INodeSimulation::getName() {
    return m_name;
}

/**
 * parses through all files and checks every of the INode-Tables until the given index / address is found
 * @param directory
 * @param index searched for address
 * @return INode* INode containing the address
 */
int *INodeSimulation::searchINodesByIndex(Directory *directory, unsigned int index) {
    if(directory == nullptr){
        return nullptr;
    }

    auto* current = dynamic_cast<INodeFile*>(directory->getFileList());
    int* currentTableResult = nullptr;

    while(current != nullptr) {
        currentTableResult = current->getINode()->checkIfAddressExists(index);
        if(currentTableResult != nullptr) {
            return currentTableResult;
        } else {
            current = dynamic_cast<INodeFile*>(current->getNextFile());
        }
    }

    Directory* subDirectory = directory->getSubDirectoryList();
    while(subDirectory != nullptr && currentTableResult == nullptr){

        currentTableResult = searchINodesByIndex(subDirectory, index);
        if (currentTableResult != nullptr) {
            return currentTableResult;
        }
        subDirectory = subDirectory->getNextDirectory();
    }
    return currentTableResult;
}
