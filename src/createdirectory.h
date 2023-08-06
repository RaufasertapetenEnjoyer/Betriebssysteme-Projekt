#ifndef CREATEDIRECTORY_H
#define CREATEDIRECTORY_H

#include <QDialog>

namespace Ui {
class CreateDirectory;
}

class CreateDirectory : public QDialog
{
    Q_OBJECT

public:
    explicit CreateDirectory(QWidget *parent = nullptr);
    ~CreateDirectory();

private:
    Ui::CreateDirectory *ui;
};

#endif // CREATEDIRECTORY_H
