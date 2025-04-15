#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <Qtoolbar>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CreateOpenFileTool();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::CreateOpenFileTool() {
    // Create a toolbar
    QToolBar *toolbar = addToolBar("Main Toolbar");

    // Create an action
    QAction *action = new QAction(QIcon(":/icons/openfile.png"), "Tool Name", this);

    // Connect the action to a slot
    connect(action, &QAction::triggered, this, &MainWindow::onToolClicked);

    // Add the action to the toolbar
    toolbar->addAction(action);
}

void MainWindow::onToolClicked() {

}

