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
    QString getName();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::CreateDirectory *ui;
    bool nameIsNotEmpty();
};

#endif // CREATEDIRECTORY_H
