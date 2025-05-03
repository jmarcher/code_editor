#include "fileexplorer.h"
#include <QHeaderView>
#include <QFileSystemModel>
#include <QDir>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QMimeData>
#include <QFileInfo>
#include <QCloseEvent>
#include <QDrag>

FileExplorer::FileExplorer(QWidget *parent) : QTreeView(parent), isDragging(false)
{
    setupModel();
    loadLastPath();
}

FileExplorer::~FileExplorer()
{
    saveLastPath();
}

void FileExplorer::setupModel()
{
    model = new QFileSystemModel(this);
    model->setRootPath(QDir::rootPath());
    model->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
    model->setNameFilters(QStringList() << "*.php" << "*.js" << "*.html" << "*.htm");
    model->setNameFilterDisables(false);

    setModel(model);
    setRootIndex(model->index(QDir::homePath()));
    setHeaderHidden(true);
    setAnimated(true);
    setIndentation(20);
    setSortingEnabled(true);
    sortByColumn(0, Qt::AscendingOrder);

    for (int i = 1; i < model->columnCount(); ++i)
        hideColumn(i);

    connect(this, &QTreeView::clicked, this, &FileExplorer::onItemClicked);
}

void FileExplorer::saveLastPath()
{
    QModelIndex currentIndex = this->currentIndex();
    if (currentIndex.isValid()) {
        QString currentPath = model->filePath(currentIndex);
        settings.setValue("lastPath", currentPath);
    }
}

void FileExplorer::loadLastPath()
{
    QString lastPath = settings.value("lastPath", QDir::homePath()).toString();
    QModelIndex index = model->index(lastPath);
    if (index.isValid()) {
        setCurrentIndex(index);
        expand(index);
        scrollTo(index);
    }
}

void FileExplorer::closeEvent(QCloseEvent *event)
{
    saveLastPath();
    event->accept();
}

void FileExplorer::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void FileExplorer::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void FileExplorer::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        QList<QUrl> urls = mimeData->urls();
        for (const QUrl &url : urls) {
            QString filePath = url.toLocalFile();
            QFileInfo fileInfo(filePath);
            if (fileInfo.isFile()) {
                emit fileSelected(filePath);
            }
        }
        event->acceptProposedAction();
    }
}

void FileExplorer::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragStartPosition = event->pos();
        isDragging = false;
    }
    QTreeView::mousePressEvent(event);
}

void FileExplorer::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton)) {
        return;
    }
    if ((event->pos() - dragStartPosition).manhattanLength() < QApplication::startDragDistance()) {
        return;
    }

    QModelIndex index = indexAt(event->pos());
    if (index.isValid() && model->fileInfo(index).isFile()) {
        isDragging = true;
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;
        QList<QUrl> urls;
        urls.append(QUrl::fromLocalFile(model->filePath(index)));
        mimeData->setUrls(urls);
        drag->setMimeData(mimeData);
        drag->exec(Qt::CopyAction);
    }
}

void FileExplorer::mouseReleaseEvent(QMouseEvent *event)
{
    isDragging = false;
    QTreeView::mouseReleaseEvent(event);
}

void FileExplorer::onItemClicked(const QModelIndex &index)
{
    QString filePath = model->filePath(index);
    QFileInfo fileInfo(filePath);
    
    if (fileInfo.isFile()) {
        emit fileSelected(filePath);
    }
}

void FileExplorer::setRootPath(const QString &path)
{
    QModelIndex index = model->index(path);
    if (index.isValid()) {
        setRootIndex(index);
    }
} 