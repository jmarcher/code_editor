#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include "editor.h"
#include "fileexplorer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupUI();
    void createActions();
    void createMenus();

    QSplitter *splitter;
    Editor *editor;
    FileExplorer *fileExplorer;
};

#endif // MAINWINDOW_H 