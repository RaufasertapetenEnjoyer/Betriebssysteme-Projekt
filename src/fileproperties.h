#ifndef FILEPROPERTIES_H
#define FILEPROPERTIES_H

#include <QDialog>
#include "DirectoryStructure/AbstractFile.h"
#include "CDRomStructure/CDRomFile.h"
#include "CDRomStructure/CDRomSimulation.h"

namespace Ui {
class FileProperties;
}

class FileProperties : public QDialog
{
    Q_OBJECT

public:
    explicit FileProperties(QWidget *parent = nullptr, AbstractFile *file = nullptr, std::string path = NULL, int platte = 0);
    explicit FileProperties(QWidget *parent = nullptr, CDRomFile *file = nullptr, std::string path = NULL, CDRomSimulation * cd = nullptr);
    ~FileProperties();
    QString getName();
    int getSize();
    bool getEditable();
    bool getSystem();
    bool getAscii();
    bool getRandomAccess();

private slots:
    void on_fileSize_textChanged(const QString &arg1);

    void on_fileName_textChanged(const QString &arg1);

    void on_checkBox_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);

    void on_checkBox_3_stateChanged(int arg1);

    void on_checkBox_4_stateChanged(int arg1);

    void on_buttonBox_accepted();

private:
    Ui::FileProperties *ui;
    bool nameIsNotEmpty();
};

#endif // FILEPROPERTIES_H
