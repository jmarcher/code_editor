#ifndef FILEEXPLORER_H
#define FILEEXPLORER_H

#include <QTreeView>
#include <QFileSystemModel>
#include <QDir>
#include <QSettings>

class FileExplorer : public QTreeView
{
    Q_OBJECT

public:
    FileExplorer(QWidget *parent = nullptr);
    ~FileExplorer();
    void setRootPath(const QString &path);

signals:
    void fileSelected(const QString &filePath);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onItemClicked(const QModelIndex &index);

private:
    QFileSystemModel *model;
    QPoint dragStartPosition;
    bool isDragging;
    void setupModel();
    void saveLastPath();
    void loadLastPath();
    QSettings settings;
};

#endif // FILEEXPLORER_H 