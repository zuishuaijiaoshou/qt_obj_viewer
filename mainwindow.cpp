#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <Qtoolbar>
#include <QFileDialog>
#include <QMessageBox>
#include "openglwidget.h"

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

void MainWindow::CreateOpenGlWidget(const QString& file_nm) {
    OpenGLWidget *openGLWidget = new OpenGLWidget(this, file_nm);
    this->setCentralWidget(openGLWidget);

    openGLWidget->RenderObjFile();
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
    QString file_nm = QFileDialog::getOpenFileName(this);

    CreateOpenGlWidget(file_nm);
}

