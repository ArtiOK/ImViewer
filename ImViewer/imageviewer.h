#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include <QScrollArea>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QScrollBar>
#include <QDir>
#include <QFileSystemModel>
#include <QListView>

namespace Ui {
class ImageViewer;
}

class ImageViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = nullptr);
    ~ImageViewer();



private slots:
    void on_actionOpen_triggered();
    void on_actionZoom_in_triggered();
    void on_actionZoom_out_triggered();
    void on_actionNormal_size_triggered();
    void on_actionFit_to_Window_triggered();
    void on_listView_2_doubleClicked(const QModelIndex &index);

private:
    Ui::ImageViewer *ui;
    QFileSystemModel *model;
    QListView *listView_2;
    QScrollArea *scrollArea;
    QLabel *imageLabel;
    QAction *actionOpen;
    QAction *actionZoom_in;
    QAction *actionZoom_out;
    QAction *actionNormal_size;
    QAction *actionFit_to_Window;
    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenuBar *menuBar;
    double scaleFactor;

    void createActions();
    void createMenus();
    void updateActions();
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);
};

#endif // IMAGEVIEWER_H
