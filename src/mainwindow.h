#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QTreeWidgetItem>
#include "DirectoryStructure/Directory.h"
#include "CDRomStructure/CDRomDirectory.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void reload();
    void buildTree(Directory * dir, QTreeWidgetItem *parent);
    void buildCDTree(CDRomDirectory * dir, QTreeWidgetItem *parent);
    void openPath(QString path);
    void openCDPath(QString path);
    std::string getPath();
private slots:
    void deleteFile();
    void fileProperties();
    void createFile();
    void createDir();
    void deleteDir();
    void dirProperties();
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_3_clicked();

    void on_treeWidget_2_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    Ui::MainWindow *ui;
    void loadCD();
};
#endif // MAINWINDOW_H
