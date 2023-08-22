//
// Created by m.busch on 16.08.2023.
//

#include "CDRomSimulation.h"

int CDRomSimulation::getBlockSize() {
    return m_blockSize;
}

void CDRomSimulation::setBlockSize(int blockSize) {
    m_blockSize = blockSize;
}

VolumeDescriptor *CDRomSimulation::getVolumeDescriptor() {
    return m_volumeDescriptor;
}

int CDRomSimulation::getIndexOfNextFreeBlock() {

}

CDRomSimulation::CDRomSimulation(int blockSize, int romSize, char* name, char* description) {
    m_blockSize = blockSize;
    m_romSize = romSize;

    auto* volumeDescriptor = new VolumeDescriptor;
    volumeDescriptor->name = name;
    volumeDescriptor->description = description;
    volumeDescriptor->creationTime = time(nullptr);
    volumeDescriptor->lastUpdate = time(nullptr);

    m_numberOfBlocks = ceil((double) romSize / (double) blockSize);
    m_statusArray = new char[m_numberOfBlocks];

    for (int i = 0; i < m_numberOfBlocks; i++) {
        m_statusArray[i] = FREE;
    }
}

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

void CDRomSimulation::createChildDirectory(char *name) {
    auto* attributes = new Attributes;
    attributes->attributes = new char[1];

    auto* directory = new CDRomDirectory(name, attributes,0);

    directory->setBit(directory->getAttributes()->attributes,1);
    attributes->dateOfCreation = time(nullptr);
    attributes->dateOfLastEdit = time(nullptr);
}

void CDRomSimulation::simulate() {

}
