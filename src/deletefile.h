#ifndef DELETEFILE_H
#define DELETEFILE_H

#include <QDialog>

namespace Ui {
class DeleteFile;
}

class DeleteFile : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteFile(QWidget *parent = nullptr);
    ~DeleteFile();

private:
    Ui::DeleteFile *ui;
};

#endif // DELETEFILE_H
