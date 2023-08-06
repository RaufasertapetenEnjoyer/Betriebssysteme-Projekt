#ifndef FILEPROPERTIES_H
#define FILEPROPERTIES_H

#include <QDialog>

namespace Ui {
class FileProperties;
}

class FileProperties : public QDialog
{
    Q_OBJECT

public:
    explicit FileProperties(QWidget *parent = nullptr);
    ~FileProperties();

private:
    Ui::FileProperties *ui;
};

#endif // FILEPROPERTIES_H
