

#ifndef BS_PROJEKT_ABSTRACTFILE_H
#define BS_PROJEKT_ABSTRACTFILE_H
#include "AbstractElement.h"


class AbstractFile: public AbstractElement {
public:
    AbstractFile(char* name, char* attributes, int size);
    bool testConvention(char* name) override;

    AbstractFile* getNextFile();
    void setNextFile(AbstractFile* fileToSet);

    AbstractFile* getPrevFile();
    void setPrevFile(AbstractFile* fileToSet);


private:
    AbstractFile* m_nextFile;
    AbstractFile* m_prevFile;
};


#endif //BS_PROJEKT_ABSTRACTFILE_H
