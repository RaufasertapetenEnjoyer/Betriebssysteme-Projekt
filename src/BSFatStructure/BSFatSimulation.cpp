
#include "BSFatSimulation.h"
#include "BSCluster.h"
#include "BSFatFile.h"
#include "iostream"
#include <cmath>

void BSFatSimulation::simulate() {
    char root[] = "root";
    Directory* directory = new Directory(root, new Attributes());
    currentDirectory = directory;


}

/**
 * Returns a pointer on the root-directory.
 * @return Directory* root
 */
Directory *BSFatSimulation::getRootDirectory() {
    if(currentDirectory->getParentDirectory() == nullptr){
        return currentDirectory;
    }else{
        Directory* directory = currentDirectory;

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
    for (int i = 0; i < (m_fatSize / m_blockSize); i++) {
        if(m_statusArray[i] == FREE){
            freeCounter++;
        }
    }
    return freeCounter;
}

void BSFatSimulation::defragDisk(Directory* directory) {
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
            file = dynamic_cast<BSFatFile*>(file->getNextFile());
        }

        std::cout << "Beende Defragmentierung von Datei: " << file->getName() << std::endl;
        //std::cout << "Grad der Fragmentierung betraegt nun: " << getFragmentation(pFat) * 100 << "%\n"  << std::endl;
    }

    Directory* subDirectory = directory->getSubDirectoryList();
    while(subDirectory != nullptr){
        defragDisk(subDirectory);
        subDirectory = subDirectory->getNextDirectory();
    }
}

void BSFatSimulation::createFile(char* name, char* attributes, int size) {
    BSFatFile* file = new BSFatFile(name, attributes, size);
    int numberOfBlocks = ceil((double)size / (double) m_blockSize);

    if(numberOfBlocks <= getFreeDiskSpace()){
        BSCluster** bsClusters = new BSCluster*[numberOfBlocks];

        for (int i = 0; i < numberOfBlocks; i++) {
            bsClusters[i] = new BSCluster;
        }

        bsClusters[0]->setPrevElement(nullptr);
        bsClusters[numberOfBlocks - 1]->setNextElement(nullptr);
        file->setFirstBlock(bsClusters[0]);

        if(numberOfBlocks > 2){
            for (int i = 1; i < numberOfBlocks - 1; i++) {
                bsClusters[i-1]->setNextElement(bsClusters[i]);
                bsClusters[i]->setPrevElement(bsClusters[i-1]);
                bsClusters[i]->setNextElement(bsClusters[i+1]);
                bsClusters[i+1]->setPrevElement(bsClusters[i]);
            }

            bsClusters[numberOfBlocks - 1]->setPrevElement(bsClusters[numberOfBlocks - 2]);

        }else if (numberOfBlocks == 2){
            bsClusters[1]->setPrevElement(bsClusters[0]);
            bsClusters[0]->setNextElement(bsClusters[1]);
        }

        unsigned int pos;

        for (int i = 0; i < numberOfBlocks; i++) {
            pos = (rand() % (m_fatSize / m_blockSize));
            while(m_statusArray[pos] != FREE){
                pos = (rand() % (m_fatSize / m_blockSize));
            }
            bsClusters[i]->setIndex(pos);
            m_statusArray[pos] = OCCUPIED;
        }
    } else{
        std::cerr << "Die File konnte nicht erstellt werden, weil der Name nicht der Konvention entspricht (" << std::endl;
    }
    currentDirectory->createChildFile((AbstractFile*) file);
}

BSFatSimulation::BSFatSimulation(unsigned int blockSize, unsigned int fatSize) {
    m_statusArray = new unsigned char[fatSize / blockSize];

    for (int i = 0; i < (fatSize/blockSize); i++) {
        m_statusArray[i] = FREE;
    }

    simulate();
    currentDirectory = getRootDirectory();
}

void BSFatSimulation::createFilesForSim() {
    char** fileNames = new char*[10] {"programm1.c", "2programm.c", "prog3.c.cpp", "p4rogramm1.c", "program5m.c", "pr6ogramm.c", "progra7mm.c", "programm8.c", "progr9amm.c", "program.cpp"};

    for (int i = 0; i < 10; i++) {
        createFile(fileNames[i], new char[]{'a'}, (int) (rand() % 2100 + 1));
    }
}

float BSFatSimulation::getFragmentation(Directory* directory, float fragmentation) {
    if(directory == nullptr){
        return 0.0;
    }

    float sumOfFrag = 0.0;
    BSFatFile* file = dynamic_cast<BSFatFile*>(directory->getFileList());
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

    return fragmentation;
}


