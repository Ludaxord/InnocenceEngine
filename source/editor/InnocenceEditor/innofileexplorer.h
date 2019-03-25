#ifndef INNOFILEEXPLORER_H
#define INNOFILEEXPLORER_H

#include <QListView>
#include <QFileSystemModel>

class InnoFileExplorer : public QListView
{
    Q_OBJECT
public:
    explicit InnoFileExplorer(QWidget *parent = 0);

    void initialize();

    void SetRootPath(QString path);
    QString GetFilePath(QModelIndex index);

private:
    void SetRootDirectory(const std::string& directory);
    QString GetRootPath();
    QString GetSelectionPath();

    QFileSystemModel* m_fileModel;

signals:

public slots:
    void DoubleClick(QModelIndex index);
};

#endif // INNOFILEEXPLORER_H