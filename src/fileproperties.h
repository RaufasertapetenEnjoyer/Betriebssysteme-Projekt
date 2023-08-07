#ifndef FILEPROPERTIES_H
#define FILEPROPERTIES_H

#include <QDialog>
#include "DirectoryStructure/AbstractFile.h"

namespace Ui {
class FileProperties;
}

class FileProperties : public QDialog
{
    Q_OBJECT

public:
    explicit FileProperties(QWidget *parent = nullptr, AbstractFile *file = nullptr);
    ~FileProperties();

private:
    Ui::FileProperties *ui;
};

#endif // FILEPROPERTIES_H
