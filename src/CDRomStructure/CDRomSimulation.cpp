//
// Created by m.busch on 16.08.2023.
//

#include "CDRomSimulation.h"

/**
 * Returns the size of one block in the current CD-ROM.
 * @return int blockSize
 */
int CDRomSimulation::getBlockSize() {
    return m_blockSize;
}

/**
 * Set the block size to the given new size.
 * @param int blockSize
 */
void CDRomSimulation::setBlockSize(int blockSize) {
    m_blockSize = blockSize;
}

/**
 * Return the volume-descriptor of the CD-ROM.
 * @return
 */
VolumeDescriptor *CDRomSimulation::getVolumeDescriptor() {
    return m_volumeDescriptor;
}

/**
 * Returns the next index of the CD-Rom which is free.
 * @return int index, -1 = memory is full.
 */
int CDRomSimulation::getIndexOfNextFreeBlock() {
    for (int i = 0; i < m_numberOfBlocks; ++i) {
        if(m_statusArray[i] == FREE){
            return i;
        }
    }
    return -1;
}

/**
 * Creates the CD-ROM simulation, initializes all values and create som files and directories.
 * @param int blockSize
 * @param int romSize
 * @param char* name
 * @param char* description
 */
CDRomSimulation::CDRomSimulation(int blockSize, int romSize, char* name, char* description) {
    m_blockSize = blockSize;
    m_romSize = romSize;

    auto* volumeDescriptor = new VolumeDescriptor;
    volumeDescriptor->name = name;
    volumeDescriptor->description = description;
    volumeDescriptor->creationTime = time(nullptr);
    volumeDescriptor->lastUpdate = time(nullptr);

    m_volumeDescriptor = volumeDescriptor;

    m_numberOfBlocks = ceil((double) m_romSize / (double) m_blockSize);
    m_statusArray = new char[m_numberOfBlocks];

    for (int i = 0; i < m_numberOfBlocks; i++) {
        m_statusArray[i] = FREE;
    }

    char* root =  {"cd"};
    auto* attributes = new Attributes;
    attributes->attributes = new char[1];
    attributes->dateOfCreation = time(nullptr);
    attributes->dateOfLastEdit = time(nullptr);

    auto* cdRomDirectory = new CDRomDirectory(root, attributes, 0);
    cdRomDirectory->setBit(cdRomDirectory->getAttributes()->attributes,0);
    cdRomDirectory->setParentDirectory(nullptr);

    m_currentDirectory = cdRomDirectory;

    char* fileNamesForRoot[] = {"test.txt", "hallo.cpp", "gruppe.java"};
    createFilesForSim(fileNamesForRoot, 3);
    simulate();
}

/**
 * Creates an child file and adds it to the current directory.
 * @param char* name
 * @param bool editable
 * @param bool system
 * @param bool ascii
 * @param bool randAccFile
 * @param int size
 */
void CDRomSimulation::createChildFile(char *name, bool editable, bool system, bool ascii, bool randAccFile, int size) {
    auto* attributes = new Attributes;
    attributes->attributes = new char[1];
    int index = getIndexOfNextFreeBlock();
    auto* file = new CDRomFile(name, attributes,size, index);

    attributes->dateOfCreation = time(nullptr);

    editable ? file->setBit(file->getAttributes()->attributes,0) : file->clrBit(file->getAttributes()->attributes,0);
    system ? file->setBit(file->getAttributes()->attributes,1) : file->clrBit(file->getAttributes()->attributes,1);
    ascii ? file->setBit(file->getAttributes()->attributes,2) : file->clrBit(file->getAttributes()->attributes,2);
    randAccFile ? file->setBit(file->getAttributes()->attributes,3) : file->clrBit(file->getAttributes()->attributes,3);

    attributes->dateOfLastEdit = time(nullptr);

    int numberOfBlocks = ceil((double) size / (double) m_blockSize);
    for (int i = 0; i < numberOfBlocks; i++) {
        m_statusArray[i+index] = OCCUPIED;
    }
    m_currentDirectory->createChildElement(dynamic_cast<AbstractElementCDRom*>(file));
}

/**
 * Creates an child directory and adds it to the current directory.
 * @param char* name
 */
void CDRomSimulation::createChildDirectory(char *name) {
    auto* attributes = new Attributes;
    attributes->attributes = new char[1];

    auto* directory = new CDRomDirectory(name, attributes,0);

    directory->setBit(directory->getAttributes()->attributes,1);
    attributes->dateOfCreation = time(nullptr);
    attributes->dateOfLastEdit = time(nullptr);
}

/**
 * Simulates an directory structure.
 */
void CDRomSimulation::simulate() {
    char* directoryNames[] = {"Steam", "Battlenet", "Ubisoft"};
    createDirectoriesForSim(directoryNames, 3);

    //root/Steam
    m_currentDirectory = dynamic_cast<CDRomDirectory*>(m_currentDirectory->getList()->getNextElement()->getNextElement()->getNextElement());
    char* fileNamesSteam[] = {"csgo.exe", "gta5.exe"};
    createFilesForSim(fileNamesSteam, 2);

    char* directoryNamesSteam[] = {"common", "images", "cheats"};
    createDirectoriesForSim(directoryNamesSteam,3);

    //root/Steam/common
    m_currentDirectory = dynamic_cast<CDRomDirectory*>(m_currentDirectory->getList()->getNextElement()->getNextElement());
    char* fileNamesCommon[] = {"ct.c", "t.c", "hostage.c"};
    createFilesForSim(fileNamesCommon,3);

    //root/Steam/images
    m_currentDirectory = dynamic_cast<CDRomDirectory*>(m_currentDirectory->getParentDirectory()->getList()->getNextElement()->getNextElement());
    char* fileNamesImages[] = {"t,png", "ct,png"};
    createFilesForSim(fileNamesImages,2);

    //root/Steam
    m_currentDirectory = m_currentDirectory->getParentDirectory();

    //root/Battlenet
    m_currentDirectory = dynamic_cast<CDRomDirectory*>(m_currentDirectory->getNextElement()->getNextElement());
    char* fileNamesBattlenet[] = {"Overwatch.exe", "Hearthstone.exe"};
    createFilesForSim(fileNamesBattlenet, 2);

    //root/Ubisoft
    m_currentDirectory = dynamic_cast<CDRomDirectory*>(m_currentDirectory->getNextElement());
    char* fileNamesUbisoft[] = {"RainbowS.exe"};
    createFilesForSim(fileNamesUbisoft, 1);
    m_currentDirectory = m_currentDirectory->getParentDirectory();
}

/**
 * Creates files for the simulation.
 * @param char** fileNames
 * @param int length
 */
void CDRomSimulation::createFilesForSim(char **fileNames, int length) {
    for (int i = 0; i < length; i++) {
        auto* attributes = new Attributes;
        attributes->attributes = new char[1];
        attributes->dateOfCreation = time(nullptr);
        attributes->dateOfLastEdit= time(nullptr);

        auto* file = new CDRomFile(fileNames[i], attributes, (rand() % 1000) + 1, getIndexOfNextFreeBlock());
        file->setBit(file->getAttributes()->attributes, 0);
        file->setBit(file->getAttributes()->attributes, 2);

        int index = getIndexOfNextFreeBlock();
        int numberOfBlocks = ceil((double) file->getSize() / (double) m_blockSize);

        for (int j = index; j < index + numberOfBlocks; j++) {
            m_statusArray[j] = OCCUPIED;
        }

        auto* elementCdRom = dynamic_cast<AbstractElementCDRom*>(file);

        m_currentDirectory->createChildElement(elementCdRom);
    }
}

/**
 * Creates directories for the simulation.
 * @param char** directoryNames
 * @param int length
 */
void CDRomSimulation::createDirectoriesForSim(char **directoryNames, int length) {
    for (int i = 0; i < length; i++) {
        auto* attributes = new Attributes;
        attributes->attributes = new char[1];
        attributes->dateOfCreation = time(nullptr);
        attributes->dateOfLastEdit= time(nullptr);

        auto* directory = new CDRomDirectory(directoryNames[i], attributes, 0);
        directory->setBit(directory->getAttributes()->attributes, 0);

        directory->setParentDirectory(m_currentDirectory);

        auto* elementCdRom = dynamic_cast<AbstractElementCDRom*>(directory);

        m_currentDirectory->createChildElement(elementCdRom);
    }
}

/**
 * Returns the root directory of the CD-ROM.
 * @return CDRomDirectory* root
 */
CDRomDirectory *CDRomSimulation::getRootDirectory() {
    CDRomDirectory* directory = m_currentDirectory;
    while (directory->getParentDirectory() != nullptr){
        directory = directory->getParentDirectory();
    }
    return directory;
}

/**
 * Returns the current directory.
 * @return CDRomDirectory* current
 */
CDRomDirectory* CDRomSimulation::getCurrentDirectory(){
    return m_currentDirectory;
}

/**
 * Set the current directory to the given.
 * @param CDRomDirectory* directoryToSet
 */
void CDRomSimulation::setCurrentDirectory(CDRomDirectory* directoryToSet){
   m_currentDirectory = directoryToSet;
}
