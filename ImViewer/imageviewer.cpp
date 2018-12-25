#include "imageviewer.h"
#include "ui_imageviewer.h"
#include <QFileInfo>
#include <QStringList>

ImageViewer::ImageViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageViewer)
{
    ui->setupUi(this);

    //FileSystemModel
    model = new QFileSystemModel(this);
    model->setFilter(QDir::Files | QDir::AllDirs);
    model->setNameFilters(QStringList() << "*.jpg" <<"*.jpeg"<< "*.png");

    model->setRootPath("");
    ui->listView_2->setModel(model);

    //Pointers Assign
    actionOpen = ui->actionOpen;
    actionZoom_in = ui->actionZoom_in;
    actionZoom_out = ui->actionZoom_out;
    actionNormal_size = ui->actionNormal_size;
    actionFit_to_Window = ui->actionFit_to_Window;
    //-----------------------------------------------------||

    scaleFactor = 1.0;
    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Dark);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    setCentralWidget(scrollArea);

    setWindowTitle(tr("Image Viewer 1.0"));

    createActions();
    createMenus();
}

ImageViewer::~ImageViewer()
{
    delete ui;
}

void ImageViewer::createActions()
{
    //Menu Actions
    QAction *actionOpen = new QAction(tr("&Open..."), this);
    actionOpen->setShortcut(tr("Ctrl+O"));

    QAction *actionZoom_in = new QAction(tr("Zoom &In"),this);
    actionZoom_in->setShortcut(tr("Ctrl++"));
    actionZoom_in->setEnabled(false);

    QAction *actionZoom_out = new QAction(tr("Zoom &Out"),this);
    actionZoom_out->setShortcut(tr("Ctrl+-"));
    actionZoom_out->setEnabled(false);

    QAction *actionNormal_size = new QAction(tr("&Normal Size"),this);
    actionNormal_size->setShortcut(tr("Ctrl+0"));
    actionNormal_size->setEnabled(false);

    QAction *actionFit_to_Window = new QAction(tr("&Fit to Window"),this);
    actionFit_to_Window->setShortcut(tr("Ctrl+F"));
    actionFit_to_Window->setEnabled(false);
    actionFit_to_Window->setChecked(false);
}

void ImageViewer::updateActions()
{
    actionZoom_in->setEnabled(!actionFit_to_Window->isChecked());
    actionZoom_out->setEnabled(!actionFit_to_Window->isChecked());
    actionNormal_size->setEnabled(!actionFit_to_Window->isChecked());
}

void ImageViewer::createMenus()
{
    //Menu
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(actionOpen);

    QMenu *viewMenu = new QMenu(tr("&View"), this);
    viewMenu->addAction(actionZoom_in);
    viewMenu->addAction(actionZoom_out);
    viewMenu->addAction(actionNormal_size);
    viewMenu->addAction(actionFit_to_Window);

    //Menu Bar
    QMenuBar *menuBar = new QMenuBar(this);
    menuBar->addMenu(fileMenu);
    menuBar->addMenu(viewMenu);
}

void ImageViewer::on_actionOpen_triggered()
{
       QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("JPEG (*.jpg *.jpeg);;PNG (*.png)"));

       if (!fileName.isEmpty())
       {
            QImage image(fileName);
            if (image.isNull()) {
                QMessageBox::information(this, tr("Image Viewer"), tr("Cannot load %1.").arg(fileName));
                return;
            }

            imageLabel->setPixmap(QPixmap::fromImage(image));

            actionFit_to_Window->setEnabled(true);
            updateActions();

            if (!actionFit_to_Window->isChecked())
                imageLabel->adjustSize();
       }
}

void ImageViewer::on_actionZoom_in_triggered()
{
    scaleImage(1.25);
}

void ImageViewer::on_actionZoom_out_triggered()
{
    scaleImage(0.8);
}

void ImageViewer::on_actionNormal_size_triggered()
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}

void ImageViewer::on_actionFit_to_Window_triggered()
{
    bool fitToWindow = actionFit_to_Window->isChecked();
        scrollArea->setWidgetResizable(fitToWindow);
        if (!fitToWindow) {
            on_actionNormal_size_triggered();
        }
        updateActions();
}

void ImageViewer::scaleImage(double factor)
{
     Q_ASSERT(imageLabel->pixmap());
     scaleFactor *= factor;
     imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

     adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
     adjustScrollBar(scrollArea->verticalScrollBar(), factor);

     actionZoom_in->setEnabled(scaleFactor < 3.0);
}

void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value() + ((factor - 1) * scrollBar->pageStep()/2)));
}

void ImageViewer::on_listView_2_doubleClicked(const QModelIndex &index)
{
    QListView *listView = (QListView*)sender();
    QFileInfo fileInfo = model->fileInfo(index);
    if (fileInfo.fileName() == "..")
    {
        QDir dir = fileInfo.dir();
        dir.cdUp();
        listView->setRootIndex(model->index(dir.absolutePath()));
    }
    else if (fileInfo.fileName()==".")
    {
        listView->setRootIndex(model->index(""));
    }
    else if (fileInfo.isDir())
    {
        listView->setRootIndex(index);
    }
    else if(fileInfo.isFile())
    {
        QString fileName = fileInfo.canonicalFilePath();
        QImage image(fileName);

        if (image.isNull())
        {
          QMessageBox::information(this, tr("Image Viewer"), tr("Cannot load %1.").arg(fileName));
          return;
        }

        imageLabel->setPixmap(QPixmap::fromImage(image));

        actionFit_to_Window->setEnabled(true);
        updateActions();

        if (!actionFit_to_Window->isChecked())
            imageLabel->adjustSize();
    }

}

