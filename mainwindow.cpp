#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "FindKeysDialog/findkeysdialog.h"

#include <QMessageBox>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    if (QMessageBox::Yes == QMessageBox::question(this, "Завершение работы",
                          "Вы уверены что хотите зевершить работу программы?",
                          QMessageBox::Yes|QMessageBox::No)){
        event->accept();
    }
}

void MainWindow::on_actionFindKeys_triggered()
{
    FindKeysDialog *findDlg = new FindKeysDialog();
    this->setCentralWidget(findDlg);
    findDlg->exec();
}
