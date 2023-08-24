#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <QDialog>
#include "DirectoryStructure/Directory.h"
#include "DirectoryStructure/AbstractFile.h"
#include "CDRomStructure/CDRomDirectory.h"

namespace Ui {
class directProperties;
}

class directProperties : public QDialog
{
    Q_OBJECT

public:
    explicit directProperties(QWidget *parent = nullptr, Directory *dir = nullptr, std::string path = nullptr);
    explicit directProperties(QWidget *parent = nullptr, CDRomDirectory *dir = nullptr, std::string path = nullptr);
    ~directProperties();
    QString getName();
    bool getEditable();

private slots:
    void on_nameEd_textChanged(const QString &arg1);

    void on_checkBox_stateChanged(int arg1);

private:
    Ui::directProperties *ui;
};

#endif // PROPERTIES_H
