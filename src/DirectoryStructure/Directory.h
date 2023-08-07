
#ifndef BS_DIRECTORY_H
#define BS_DIRECTORY_H
#include "AbstractElement.h"
#include "AbstractFile.h"

class Directory: public AbstractElement {
public:
    Directory(char *name, Attributes *attributes);
    ~Directory() override;

    Directory* getLastDirectoryOfTheList();
    void setLastDirectoryOfTheList(Directory* directoryToSet);

    AbstractFile* getLastFileOfTheList(AbstractFile* fileToSet);
    void setLastFileOfTheList(AbstractFile* fileToSet);

    Directory* getParentDirectory();
    void setParentDirectory(Directory* newParent);

    bool testConvention(char *nameToTest) override;

    void createChildDirectory(char* name, Attributes* attributes);
    int deleteChildDirectroy(char* name);

    void createChildFile(AbstractFile *file);
    int deleteChildElement(char* name);

    bool checkIfDirectoryNameExists(char* name);
    bool checkIfFileNameExists(char* name);

    AbstractFile* getFileList();
    Directory* getSubDirectoryList();

    Directory* getNextDirectory();
    void setNextDirectory(Directory* directoryToSet);

    Directory* getPrevDirectory();
    void setPreviousDirectory(Directory* directoryToSet);


private:
    /**
     * List of all directories, which are under the current.
     */
    Directory* m_directoryList;
    /**
     * List of all files, which are under the current.
     */
    AbstractFile* m_fileList;
    /**
     * Pointer to the next directory, of the top directory-list.
     */
    Directory* m_nextDirectory;
    /**
     * Pointer to the previous directory, of the top directory-list.
     */
    Directory* m_prevDirectory;
    /**
     * Pointer to the parent directory of the current directory.
     */
    Directory* m_parentDirectory;
    /**
     * Shows how many files lay under this directory.
     */
    unsigned int m_numberOfFiles;
};


#endif //BS_DIRECTORY_H
