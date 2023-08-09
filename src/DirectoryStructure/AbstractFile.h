

#ifndef BS_PROJEKT_ABSTRACTFILE_H
#define BS_PROJEKT_ABSTRACTFILE_H
#include "AbstractElement.h"

/**
 * First bit of attributes is the editable flag
 * Second is the system flag
 * Third is the ascii flag
 * Fourth is the ram-file flag
 */

class AbstractFile: public AbstractElement {
public:
    AbstractFile(char* name, Attributes* attributes, int size);
    ~AbstractFile() override;
    bool testConvention(char* name) override;

    AbstractFile* getNextFile();
    void setNextFile(AbstractFile* fileToSet);

    AbstractFile* getPrevFile();
    void setPrevFile(AbstractFile* fileToSet);

    bool isEditable();
    void setEditable(bool isEditable);

    bool isSystem();
    void setSystem(bool isSystem);

    bool isAscii();
    void setAscii(bool isAscii);

    bool isRandAccFile();
    void setRandAccFile(bool isRandAccFile);

private:
    AbstractFile* m_nextFile;
    AbstractFile* m_prevFile;
};


#endif //BS_PROJEKT_ABSTRACTFILE_H
