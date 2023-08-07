
#include "BSFatSimulation.h"
#include "BSCluster.h"
#include "BSFatFile.h"
#include "DirectoryStructure/Directory.h"
#include "time.h"
#include "iostream"
#include <math.h>


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
   /* m_statusArray[5]=CORRUPTED;
    m_statusArray[7]=CORRUPTED;
    m_statusArray[10]=CORRUPTED;
    m_statusArray[13]=CORRUPTED;
    m_statusArray[14]=RESERVED;*/
    char root[] = "root";
    Directory* directory = new Directory(root, new Attributes());
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
/*void BSFatSimulation::defragmentDisk(Directory* directory) {
    if(directory == nullptr){
        return;
    }

    int counter = 0;
    BSFatFile* file = dynamic_cast<BSFatFile*>(directory->getFileList());

    while(file != nullptr){
        std::cout << "Beginne Degragmentierung von Datei: " << file->getName() << std::endl;

        BSCluster* current = file->getFirstBlock();

        while (current != nullptr) {
            if (m_statusArray[counter] == FREE) {
                m_statusArray[counter] = RESERVED;
                m_statusArray[current->getIndex()] = FREE;
                current->setIndex(counter);
                m_statusArray[counter] = OCCUPIED;
            } else {

                while (m_statusArray[counter] == RESERVED || m_statusArray[counter] == CORRUPTED) {
                    counter++;
                }

                if(m_statusArray[counter] == FREE){
                    m_statusArray[counter] = RESERVED;
                    m_statusArray[current->getIndex()] = FREE;
                    current->setIndex(counter);
                    m_statusArray[counter] = OCCUPIED;
                }

                BSFatFile* file2 = dynamic_cast<BSFatFile*>(directory->getFileList());

                while(file2 != nullptr) {
                    BSCluster *secondCurrent = file2->getFirstBlock();

                    while (secondCurrent != nullptr) {
                        if (secondCurrent->getIndex() == counter) {
                            int saver = current->getIndex();
                            current->setIndex(secondCurrent->getIndex());
                            secondCurrent->setIndex(saver);
                            break;
                        }
                        secondCurrent = secondCurrent->getNextBlock();
                    }
                    file2 = dynamic_cast<BSFatFile*>(file2->getNextFile());
                }
            }
            counter++;
            current = current->getNextBlock();
        }
        file = dynamic_cast<BSFatFile*>(file->getNextFile());
        //std::cout << "Beende Defragmentierung von Datei: " << file->getName() << std::endl;
        //std::cout << "Grad der Fragmentierung betraegt nun: " << getFragmentation(pFat) * 100 << "%\n"  << std::endl;
    }

    Directory* subDirectory = directory->getSubDirectoryList();
    while(subDirectory != nullptr){
        defragmentDisk(subDirectory);
        subDirectory = subDirectory->getNextDirectory();
    }
}*/

void BSFatSimulation::defragmentDisk(Directory *directory, int currentPosition) {
    if(directory == nullptr){
        return;
    }

    BSFatFile* file = dynamic_cast<BSFatFile*>(directory->getFileList());

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
                        BSCluster* test = nullptr;
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
        std::cout << "Speicher aufgebraucht, es können keine Datein mehr erstellt werden!";
    }else {
        Attributes *attributes = new Attributes();
        char *att = new char[1];
        attributes->dateOfCreation = time(nullptr);
        attributes->dateOfLastEdit = time(nullptr);
        attributes->size = size;
        attributes->attributes = att;
        setEditable(attributes, editable);
        setSystem(attributes, system);
        setAsccii(attributes, ascii);
        setRandAccFile(attributes, randAccFile);

        BSFatFile *file = new BSFatFile(name, attributes, size);

        file->setAttributes(attributes);

        if (numberOfBlocks <= getFreeDiskSpace()) {
            BSCluster **bsClusters = new BSCluster *[numberOfBlocks];

            for (int i = 0; i < numberOfBlocks; i++) {
                bsClusters[i] = new BSCluster();
            }

            bsClusters[0]->setPrevElement(nullptr);
            bsClusters[numberOfBlocks - 1]->setNextElement(nullptr);
            file->setFirstBlock(bsClusters[0]);

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
            m_numberOfFiles++;
        }
        m_currentDirectory->createChildFile((AbstractFile *) file);
    }
}

BSFatSimulation::BSFatSimulation(unsigned int blockSize, unsigned int fatSize) {
    m_statusArray = new unsigned char[fatSize / blockSize];
    m_blockSize = blockSize;
    m_fatSize = fatSize;

    for (int i = 0; i < (fatSize/blockSize); i++) {
        m_statusArray[i] = FREE;
    }

    simulate();
    m_currentDirectory = getRootDirectory();
}

void BSFatSimulation::createFilesForSim(char** names, unsigned int length) {
    for (int i = 0; i < length; i++) {
        createFile(names[i], true, false, false, true, (int) (rand() % 2100 +1));
    }
}

float BSFatSimulation::getFragmentation(Directory* directory, float fragmentation) {
    if(directory == nullptr){
        return 0.0;
    }

    if(directory->getFileList() == nullptr){
        return 0.0;
    }

    BSFatFile* file = dynamic_cast<BSFatFile*>(directory->getFileList());
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

    fragmentation += sumOfFrag;

    Directory* subDirectory = directory->getSubDirectoryList();
    while(subDirectory != nullptr){
        getFragmentation(subDirectory, fragmentation);
        subDirectory = subDirectory->getNextDirectory();
    }

    return (fragmentation / (float) m_numberOfFiles) * 100;
}

void BSFatSimulation::createDirectory(char* name, Attributes* attributes) {
    m_currentDirectory->createChildDirectory(name, attributes);
}

/**
 * Sets a bit of an char array (0-index-based).
 * @param char* array, array where to set the bit
 * @param int bitToSet, bit which has to be set
 */
void BSFatSimulation::setBit(char *array, int bitToSet) {
    array[bitToSet/8] = array[bitToSet/8] | ((int) pow(2, (bitToSet % 8)));
}

/**
 * Clears a bit of an char array (0-index-based).
 * @param char* array, array where to clear the bit
 * @param int bitToClear, bit which has to be cleared
 */
void BSFatSimulation::clrBit(char *array, int bitToClear) {
    array[bitToClear/8] = array[bitToClear/8] & ~((int) pow(2, (bitToClear % 8)));
}

/**
 * Test a bit of an char array (0-index-based).
 * @param char* array, where to test the bit
 * @param int bitToTest, bit which has to be tested
 * @return int bit, the bit it self 0 or 1
 */
int BSFatSimulation::tstBit(char *array, int bitToTest) {
    return (array[bitToTest/8] & (int) pow(2, (bitToTest % 8))) ? 1 : 0;
}

bool BSFatSimulation::isEditable(Attributes* attributes) {
    return tstBit(attributes->attributes, 0);
}

void BSFatSimulation::setEditable(Attributes *attributes, bool isEditable) {
    isEditable ? setBit(attributes->attributes, 0) : clrBit(attributes->attributes, 0);
}

bool BSFatSimulation::isSystem(Attributes *attributes) {
    return tstBit(attributes->attributes, 1);
}

void BSFatSimulation::setSystem(Attributes *attributes, bool isService) {
    isService ? setBit(attributes->attributes, 1) : clrBit(attributes->attributes, 1);
}

bool BSFatSimulation::isAsccii(Attributes *attributes) {
    return tstBit(attributes->attributes, 2);
}

void BSFatSimulation::setAsccii(Attributes *attributes, bool isAsccii) {
    isAsccii ? setBit(attributes->attributes, 2) : clrBit(attributes->attributes, 2);
}

bool BSFatSimulation::isRandAccFile(Attributes *attributes) {
    return tstBit(attributes->attributes, 3);
}

void BSFatSimulation::setRandAccFile(Attributes *attributes, bool isRandAccFile) {
    isRandAccFile ? setBit(attributes->attributes, 3) : clrBit(attributes->attributes, 3);
}

Directory *BSFatSimulation::getCurrentDirectory() {
    return m_currentDirectory;
}

void BSFatSimulation::setCurrentDirectory(Directory *directory) {
    m_currentDirectory = directory;
}

unsigned char * BSFatSimulation::getStatusArray() {
    return m_statusArray;
}

unsigned int BSFatSimulation::getBlockSize() {
    return m_blockSize;
}

unsigned int BSFatSimulation::getFatSize() {
    return m_fatSize;
}

unsigned int BSFatSimulation::getNumberOfFilesThatCanBeSaved() {
    return m_fatSize / m_blockSize;
}

unsigned int BSFatSimulation::getNumberOfCurrentlySavedFiles() {
    return m_numberOfFiles;
}

void BSFatSimulation::createDirectoriesForSim() {
    m_currentDirectory = getRootDirectory();

    //char** directoryNames = new char*[] {"Michael", "Jan", "Simon", "Betriebssysteme", "Algo", "SWT", "Mathe1", "Mathe2", "Datenbanken", "Projekt"};
    char* directoryNames[] = {"Michael", "Jan", "Simon", "Betriebssysteme", "Algo", "SWT", "Mathe1", "Mathe2", "Datenbanken", "Projekt"};
    m_currentDirectory->createChildDirectory(directoryNames[0], new Attributes());

    //char** fileNames = new char*[] {"main.c", "hallo.txt", "Presentation.docx"};
    char* fileNames[] = {"main.c", "hallo.txt", "Presentation.docx"};
    m_currentDirectory = m_currentDirectory->getSubDirectoryList();
    createFilesForSim(fileNames, 3);
    m_currentDirectory = getRootDirectory();

    m_currentDirectory->createChildDirectory(directoryNames[1], new Attributes());
    m_currentDirectory->createChildDirectory(directoryNames[2], new Attributes());

    m_currentDirectory = m_currentDirectory->getSubDirectoryList();

    m_currentDirectory->createChildDirectory(directoryNames[3], new Attributes());
    m_currentDirectory->createChildDirectory(directoryNames[4], new Attributes());
    m_currentDirectory->createChildDirectory(directoryNames[5], new Attributes());

    m_currentDirectory = m_currentDirectory->getSubDirectoryList();

    m_currentDirectory->createChildDirectory(directoryNames[9], new Attributes());

    m_currentDirectory = getRootDirectory()->getSubDirectoryList()->getNextDirectory();

    m_currentDirectory->createChildDirectory(directoryNames[6], new Attributes());
    m_currentDirectory->createChildDirectory(directoryNames[7], new Attributes());

    m_currentDirectory = getRootDirectory()->getSubDirectoryList()->getNextDirectory()->getNextDirectory();

    m_currentDirectory->createChildDirectory(directoryNames[8], new Attributes());
}

BSCluster * BSFatSimulation::searchClusterByIndex(Directory *directory, unsigned int index) {
    if(directory == nullptr){
        return nullptr;
    }

    BSFatFile* file = dynamic_cast<BSFatFile*>(directory->getFileList());
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
    while(subDirectory != nullptr){
        BSCluster* found = searchClusterByIndex(subDirectory, index);
        if(found != nullptr){
            return found;
        }
        subDirectory = subDirectory->getNextDirectory();
    }
}

char * BSFatSimulation::getPath(){
    Directory* directory = m_currentDirectory;
    while(directory->getParentDirectory()!=nullptr){

    }
}
