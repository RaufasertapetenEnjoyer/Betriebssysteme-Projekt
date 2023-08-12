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
    explicit FileProperties(QWidget *parent = nullptr, AbstractFile *file = nullptr, std::string path = NULL);
    ~FileProperties();

private slots:
    void on_fileSize_textChanged(const QString &arg1);

    void on_fileName_textChanged(const QString &arg1);

    void on_checkBox_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);

    void on_checkBox_3_stateChanged(int arg1);

    void on_checkBox_4_stateChanged(int arg1);

private:
    Ui::FileProperties *ui;
};

#endif // FILEPROPERTIES_H
