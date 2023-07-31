#include <iostream>
#include "DirectoryStructure/Directory.h"
#include "DirectoryStructure/AbstractFile.h"
#include "BSFatStructure/BSFatSimulation.h"

int main() {

    BSFatSimulation* bsFatSimulation = new BSFatSimulation();







    char name1[] = "root";
    char name2[] = "sub1";
    char name3[] = "sub2";

    Directory* currentDirectory;

    auto* root = new Directory(name1, new char[2] {'a', 'e'});
    root->setParentDirectory(nullptr);
    currentDirectory = root;

    root->createChildDirectory(name2, new char[2] {'i', 'o'});
    root->createChildDirectory(name3, new char[2] {'u', 'e'});

    AbstractElement* abstractElement = new AbstractFile(name3, new char[2] {'u', 'e'}, 512);


    char fileName[] = "1main.c.cpp";
    AbstractElement* abstractElement2 = new AbstractFile(fileName, new char[2] {'u', 'e'}, 512);
    if(instanceof<AbstractFile>(abstractElement2)){
        AbstractFile file = reinterpret_cast<AbstractFile &&>(abstractElement2);
        if(file.testConvention(fileName)){
            std::cout << "true"<< std::endl;
        }else{
            std::cout << "false"<< std::endl;
        }
    }
}
