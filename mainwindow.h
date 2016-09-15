#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QListWidgetItem>
#include "dataload.h"
#include "draw.h"
#include "spath.h"
#include "kdtree.h"
#include "segtree.h"
#include "top-k.h"
#include "taxidata.h"
#include "findbyname.h"
#include "mysignal.h"
#include <ctime>
#include <cstdio>
#include "cstdlib"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void myslot(int type, int id);
    void myslot1(double dis);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_LIST_itemClicked(QListWidgetItem *item);

    void on_LIST_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_9_clicked();


    void on_LIST1_itemClicked();

    void on_LIST1_itemDoubleClicked();

    void on_pushButton_12_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H