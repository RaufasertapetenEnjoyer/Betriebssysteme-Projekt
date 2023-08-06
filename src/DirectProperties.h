#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <QDialog>

namespace Ui {
class directProperties;
}

class directProperties : public QDialog
{
    Q_OBJECT

public:
    explicit directProperties(QWidget *parent = nullptr);
    ~directProperties();

private:
    Ui::directProperties *ui;
};

#endif // PROPERTIES_H
