#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    webdavthreadclient = new webdavthread();
    webdavthreadclient->DoSetup(cthread);
    webdavthreadclient->moveToThread(&cthread);

    connect(this, SIGNAL(startwebdav()), webdavthreadclient, SLOT(webdav()));

    if(!cthread.isRunning()){
        cthread.start();
    }else{
        emit startwebdav();
    }
}

MainWindow::~MainWindow()
{

    if(cthread.isRunning()){
        cthread.quit();
        cthread.wait();
    }

    delete ui;
}
