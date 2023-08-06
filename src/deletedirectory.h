#ifndef DELETEDIRECTORY_H
#define DELETEDIRECTORY_H

#include <QDialog>

namespace Ui {
class deleteDirectory;
}

class deleteDirectory : public QDialog
{
    Q_OBJECT

public:
    explicit deleteDirectory(QWidget *parent = nullptr);
    ~deleteDirectory();

private:
    Ui::deleteDirectory *ui;
};

#endif // DELETEDIRECTORY_H
