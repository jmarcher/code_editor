#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    createActions();
    createMenus();
    setWindowTitle("Code Editor");
    resize(1200, 800);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    splitter = new QSplitter(Qt::Horizontal, this);
    setCentralWidget(splitter);

    fileExplorer = new FileExplorer(this);
    editor = new Editor(this);

    splitter->addWidget(fileExplorer);
    splitter->addWidget(editor);

    // Set initial sizes
    splitter->setSizes({200, 1000});

    // Connect file explorer signals
    connect(fileExplorer, &FileExplorer::fileSelected, this, [this](const QString &filePath) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            editor->setPlainText(file.readAll());
            editor->setLanguage(filePath);
            file.close();
        }
    });
}

void MainWindow::createActions()
{
    // File menu actions
    QAction *openAction = new QAction("Open", this);
    connect(openAction, &QAction::triggered, this, [this]() {
        QString filePath = QFileDialog::getOpenFileName(this, "Open File");
        if (!filePath.isEmpty()) {
            QFile file(filePath);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                editor->setPlainText(file.readAll());
                editor->setLanguage(filePath);
                file.close();
            }
        }
    });

    QAction *saveAction = new QAction("Save", this);
    connect(saveAction, &QAction::triggered, this, [this]() {
        QString filePath = QFileDialog::getSaveFileName(this, "Save File");
        if (!filePath.isEmpty()) {
            QFile file(filePath);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                file.write(editor->toPlainText().toUtf8());
                file.close();
            }
        }
    });

    // Add actions to the window
    addAction(openAction);
    addAction(saveAction);
}

void MainWindow::createMenus()
{
    QMenu *fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction("Open");
    fileMenu->addAction("Save");
} 