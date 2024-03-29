
#include "BSFatSimulation.h"
#include "BSCluster.h"
#include "BSFatFile.h"
#include "iostream"
#include <cmath>
#include <cstring>
#include <QString>
#include <ctime>


/**
 * Bei File gibt es 4 verschiedene Flags die gesetzt werden können, das sind die Flags editable (Index 0),
 * Systemfile (Index 1), Asccii-File (index 2) und  Random-Access-File (Index 3)
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
    char* fileNamesForRoot[] = {"programm1.c", "2programm.c", "prog3.c.cpp", "p4rogramm1.c", "program5m.c", "pr6ogramm.c", "progra7mm.c", "programm8.c", "progr9amm.c", "program.cpp"};
    createFilesForSim(fileNamesForRoot, 10);
    createDirectoriesForSim();
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
                        if(secondCurrent == nullptr){
                            std::cout<<"wrong"<<std::endl;
                        }

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
        attributes->attributes = new char[1];

        auto *file = new BSFatFile(name, attributes, size);
        file->setSize(size);

        file->setEditable(editable);
        file->setSystem(system);
        file->setAscii(ascii);
        file->setRandAccFile(randAccFile);

        file->setFirstBlock(initBSCluster(numberOfBlocks));

        m_numberOfFiles++;

        AbstractFile* file1 = dynamic_cast<AbstractFile*>(file);
        m_currentDirectory->createChildFile( file1);

        Directory* directory = m_currentDirectory;
        while (directory != nullptr){
            directory->setNumberOfFiles(directory->getNumberOfFiles() + 1);
            directory = directory->getParentDirectory();
        }
    }
}

/**
 * Creates a BSFat simulation, initializes all parameters and creates some directories and files.
 * @param unsigned int blockSize
 * @param unsigned fatSize
 * @param char* name
 */
BSFatSimulation::BSFatSimulation(unsigned int blockSize, unsigned int fatSize, char* name) {
    m_name = name;
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
        createFile(names[i], true, false, true, false, (int) (rand() % 2100 + 1));
    }
}

/**
 * Calculates the fragmentation of the whole disk (recursive).
 * @param Directory* directory (start from root)
 * @param float fragmentation (start with 0.0)
 * @return
 */
void BSFatSimulation::getFragmentation(Directory *directory, float &fragmentation) {
    if(directory == nullptr){
        return;
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

    fragmentation += sumOfFrag;

    Directory* subDirectory = directory->getSubDirectoryList();
    while(subDirectory != nullptr){

        getFragmentation(subDirectory, fragmentation);
        subDirectory = subDirectory->getNextDirectory();
    }


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
    Attributes** attributes = new Attributes*[13];

    for(int i=0; i<13; i++){
        attributes[i] = new Attributes();
        attributes[i]->dateOfCreation = time(nullptr);
        attributes[i]->dateOfLastEdit = time(nullptr);
        attributes[i]->attributes = new char[1];
    }

    char* fileNamesMichael[] =  {"main.c", "hallo.txt", "Presentation.docx"};
    char* fileNameBetriebssysteme[] = {"BSSimulation.h"};
    char* fileNamesSWT[] = {"AbstractAnimal.java", "Dog.java"};

    m_currentDirectory->createChildDirectory(directoryNames[0], attributes[0]);
    std::cout<<m_currentDirectory->getSubDirectoryList()->getName()<<std::endl;
    m_currentDirectory = m_currentDirectory->getSubDirectoryList();


    //Michael/
    m_currentDirectory->createChildDirectory(directoryNames[3],attributes[1]);
    m_currentDirectory->createChildDirectory(directoryNames[4], attributes[2]);
    m_currentDirectory->createChildDirectory(directoryNames[5], attributes[3]);

    createFilesForSim(fileNamesMichael, 3);

    //Michael/Betriebssysteme
    m_currentDirectory = m_currentDirectory->getSubDirectoryList();
    createFilesForSim(fileNameBetriebssysteme, 1);
    m_currentDirectory->createChildDirectory(directoryNames[9],attributes[4]);

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
    m_currentDirectory->createChildDirectory(directoryNames[1], attributes[5]);

    //Jan/
    m_currentDirectory = m_currentDirectory->getSubDirectoryList()
            ->getNextDirectory();
    m_currentDirectory->createChildDirectory(directoryNames[6], attributes[6]);
    m_currentDirectory->createChildDirectory(directoryNames[7], attributes[7]);

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
    m_currentDirectory->createChildDirectory(directoryNames[2], attributes[8]);

    //Simon/
    m_currentDirectory = m_currentDirectory->getSubDirectoryList()
            ->getNextDirectory()
            ->getNextDirectory();
    m_currentDirectory->createChildDirectory(directoryNamesSimon[0], attributes[9]);

    //Simon/com
    m_currentDirectory = m_currentDirectory->getSubDirectoryList();
    m_currentDirectory->createChildDirectory(directoryNamesSimon[1], attributes[10]);

    //Simon/com/de
    m_currentDirectory = m_currentDirectory->getSubDirectoryList();
    m_currentDirectory->createChildDirectory(directoryNamesSimon[2], attributes[11]);

    //Simon/com/de/bs
    m_currentDirectory = m_currentDirectory->getSubDirectoryList();
    m_currentDirectory->createChildDirectory(directoryNamesSimon[3], attributes[12]);

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
    BSCluster* bsCluster = nullptr;
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
    BSCluster* found = nullptr;
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
 * @return bool success
 */
bool BSFatSimulation::deleteFile(AbstractFile *file) {
    if(file->isSystem()){
         return false;
    }
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
    }else{
        return false;
    }
    Directory* directory = m_currentDirectory;
    while(directory != nullptr){
        directory->setNumberOfFiles(directory->getNumberOfFiles() - 1);
        directory = directory->getParentDirectory();
    }
    m_numberOfFiles--;
    return true;
}

/**
 * Deletes the given directory under the current directory, if and only if the directory is empty.
 * @param directory
 * @return bool success
 */
bool BSFatSimulation::deleteDirectory(Directory *directory) {
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
 * Updates the attributes of a given file.
 * @param char* name
 * @param bool isEditable
 * @param bool isSystem
 * @param bool isAscii
 * @param bool isRamFile
 * @param AbstractFile* file
 * @param int size
 * @return bool success
 */
bool BSFatSimulation::updateFile(char *name, bool isEditable, bool isSystem, bool isAscii, bool isRamFile,
                                 AbstractFile *file, int size) {//evtl. ausgaben oder ähnliches nötig
    if(file->isEditable()){
        if(!file->isSystem()){
            if(strcmp(file->getName(), name) != 0){
                file->setName(name);
            }
            if(isSystem != file->isSystem()){
                file->tstBit(file->getAttributes()->attributes, 1) ? file->clrBit(file->getAttributes()->attributes, 1) : file->setBit(file->getAttributes()->attributes, 1);
            }
            int numberOfNewBlocks = ceil((double) size / (double) m_blockSize);
            int numberOfCurrBlocks = ceil((double) file->getSize() / (double) m_blockSize);

            auto fatFile = dynamic_cast<BSFatFile*>(file);

            if (numberOfCurrBlocks < numberOfNewBlocks) {
                if ((numberOfNewBlocks - numberOfCurrBlocks) < getFreeDiskSpace()) {
                    BSCluster* clusterToAdd = initBSCluster(numberOfNewBlocks - numberOfCurrBlocks);
                    BSCluster *lastCluster = fatFile->getLastBlock();

                    lastCluster->setNextElement(clusterToAdd);
                    clusterToAdd->setPrevElement(lastCluster);
                }
            } else if (numberOfCurrBlocks > numberOfNewBlocks) {
                int counter = numberOfCurrBlocks - numberOfNewBlocks;
                BSCluster* cluster = fatFile->getLastBlock();

                while (counter != 0) {
                    BSCluster* currCluster = cluster;
                    cluster = cluster->getPrevBlock();

                    currCluster->setPrevElement(nullptr);
                    currCluster->setNextElement(nullptr);

                    m_statusArray[currCluster->getIndex()] = FREE;

                    delete currCluster;
                    counter--;
                }
            }
            file->setSize(size);
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
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

/**
 * @brief updates a directory
 * @param char* name
 * @param bool isEditable
 * @param Directory * directory
 * @return bool success
 */
bool BSFatSimulation::updateDirectory(char* name, bool isEditable, Directory* directory) {
    if(directory->isEditable()){
        if(strcmp(name, directory->getName()) != 0){
            directory->setName(name);
        }
        if(isEditable != directory->isEditable() ){
            updateEditableOnContent(directory);
        }
        return true;
    }else{
        return false;
    }
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

    while(cluster != nullptr){
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

/**
 * Changes all files and directories beneath the given directory from editable to not-editable.
 * @param Directory* directory
 */
void BSFatSimulation::updateEditableOnContent(Directory *directory) {
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
bool BSFatSimulation::checkIfEditIsValid(char *name, bool isEditable, bool isSystem, bool isAscii, bool isRamFile,
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
 * Returns name of current BsFatSimulation.
 * @return char* name
 */
char* BSFatSimulation::getName(){
    return m_name;
}



/**
 * Copy a given file to the current directory of the BSFat.
 * @param CDRomDirectory* directoryToCopy
 * @param const int cdRomBlockSize
 * @return bool success
 */
bool BSFatSimulation::copyCDRomFile(CDRomFile* cdRomFile, const int cdRomBlockSize) {
    int numberOfBlocksForFat = ceil((double) cdRomFile->getSize() / (double) m_blockSize);
    int numberOfBlocksForCDRom = ceil((double) cdRomFile->getSize() / (double) cdRomBlockSize);

    if(numberOfBlocksForFat <= getFreeDiskSpace()){
        //Copy name
        QString name(cdRomFile->getName());
        char *nameOfFile = new char[name.length()];
        strcpy(nameOfFile, cdRomFile->getName());
        //Copy attributes
        Attributes* attributesOfFile = new Attributes();
        memccpy(attributesOfFile, cdRomFile->getAttributes(),1, sizeof(Attributes));

        BSCluster* list = nullptr;
        BSCluster* next = nullptr;
        BSCluster* prev = nullptr;

        int stagedBlocksOfCDRom = 0;
        int counter = 0;
        int length = numberOfBlocksForFat + cdRomFile->getFirstBlock();

        for (int i = cdRomFile->getFirstBlock(); i <= length; i++) {
            stagedBlocksOfCDRom++;
            if(stagedBlocksOfCDRom * cdRomBlockSize == m_blockSize || (i == length)){
                next = new BSCluster();

                unsigned int pos;
                pos = (rand() % (m_fatSize / m_blockSize));
                while (m_statusArray[pos] != FREE) {
                    pos = (rand() % (m_fatSize / m_blockSize));
                }
                next->setIndex((int) pos);
                m_statusArray[pos] = OCCUPIED;
                next->setNextElement(nullptr);
                next->setPrevElement(prev);

                if(counter == 0){
                    list = next;
                }else{
                    prev->setNextElement(next);
                }
                prev = next;
                counter++;
                stagedBlocksOfCDRom = 0;
            }
        }
        auto* bsFatFile = new BSFatFile(nameOfFile, attributesOfFile, cdRomFile->getSize());
        bsFatFile->setFirstBlock(list);
        auto* file = dynamic_cast<AbstractFile*>(bsFatFile);
        m_currentDirectory->createChildFile(file);
        m_numberOfFiles++;
        Directory* directory = m_currentDirectory;
        while (directory != nullptr){
            directory->setNumberOfFiles(directory->getNumberOfFiles() + 1);
            directory = directory->getParentDirectory();
        }
        return true;
    }else{
        return false;
    }
}

/**
* Copy an given directory with all its content to the current directory of the BSFat.
* @param CDRomDirectory* directoryToCopy
* @param const int cdRomBlockSize
*/
void BSFatSimulation::copyCDRomDirectory(CDRomDirectory *directoryToCopy, const int cdRomBlockSize) {
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

