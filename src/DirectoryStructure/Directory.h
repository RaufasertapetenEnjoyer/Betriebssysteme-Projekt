
#ifndef BS_DIRECTORY_H
#define BS_DIRECTORY_H
#include "AbstractElement.h"
#include "AbstractFile.h"

/**
 * First bit of attributes is the editable flag
 *
 */

class Directory: public AbstractElement {
public:
    Directory(char *name, Attributes *attributes);
    ~Directory() override;

    Directory* getLastDirectoryOfTheList();
    void setLastDirectoryOfTheList(Directory* directoryToSet);

    AbstractFile* getLastFileOfTheList(); // can be deleteted
    void setLastFileOfTheList(AbstractFile* fileToSet);

    Directory* getParentDirectory();
    void setParentDirectory(Directory* newParent);

    bool testConvention(char *nameToTest) override;

    void createChildDirectory(char* name, Attributes* attributes);

    void createChildFile(AbstractFile *file);

    bool checkIfDirectoryNameExists(char* name);
    bool checkIfFileNameExists(char* name);

    AbstractFile* getFileList();
    Directory* getSubDirectoryList();

    Directory* getNextDirectory();
    void setNextDirectory(Directory* directoryToSet);

    Directory* getPrevDirectory();
    void setPreviousDirectory(Directory* directoryToSet);

    void setFileList(AbstractFile* file);
    void setDirectoryList(Directory* directory);

    unsigned int getNumberOfFiles(); // can be deleted
    void setNumberOfFiles(unsigned int numberOfFiles);

    bool checkIfParentsAreEditable();

    bool isEditable();
    void setEditable(bool editable);

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
