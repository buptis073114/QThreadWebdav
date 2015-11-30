#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "webdavthread.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



signals:
    void startwebdav();

private:
    Ui::MainWindow *ui;
    webdavthread *webdavthreadclient;
    QThread cthread;
};

#endif // MAINWINDOW_H
