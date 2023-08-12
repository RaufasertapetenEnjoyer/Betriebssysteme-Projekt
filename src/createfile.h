#ifndef CREATEFILE_H
#define CREATEFILE_H

#include <QDialog>

namespace Ui {
class CreateFile;
}

class CreateFile : public QDialog
{
    Q_OBJECT

public:
    explicit CreateFile(QWidget *parent = nullptr);
    ~CreateFile();
    QString getName();
    int getSize();
    bool getEditable();
    bool getSystem();
    bool getAscii();
    bool getRandomAccess();


private slots:

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_lineEdit_textChanged(const QString &arg1);

    void on_editable_stateChanged(int arg1);

    void on_system_stateChanged(int arg1);

    void on_ascii_stateChanged(int arg1);

    void on_checkBox_4_stateChanged(int arg1);

private:
    Ui::CreateFile *ui;
    bool checkEnable();
    bool nameIsNotEmpty();
};

#endif // CREATEFILE_H
