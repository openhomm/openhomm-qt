#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
    ui->setupUi(this);
    QTreeView &tree = *ui->treeView;
    model.setLazyChildCount(true);
    model.setSorting(QDir::DirsFirst);
    tree.setModel(&model);
    tree.setColumnWidth(0, 300);

    QTreeWidget &treeFrames = *ui->treeWidgetFrames;
    treeFrames.setColumnCount(1);
    treeFrames.setColumnWidth(0, 50);
    treeFrames.setHeaderLabels(QStringList("Frames"));

    //view.setOptimizationFlag(QGraphicsView::DontClipPainter, true);

    delay = 10;
    connect(&timer, SIGNAL(timeout()), this, SLOT(onJumpToImage()));

    QGraphicsView &view = *ui->graphicsView;
    //view.setViewport(new QGLWidget());
    view.resize(600, 400);
    view.setScene(&scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onJumpToImage()
{
    ir.jumpToNextImage();
    ir.read(&im);
    pix = QPixmap::fromImage(im);
    scene.clear();
    scene.setSceneRect(pix.rect());
    scene.addPixmap(pix);
}

void MainWindow::LoadImage(QModelIndex index)
{
    if (!model.isDir(index))
    {
        ir.setFileName(model.filePath(index));

        QLabel &label = *ui->label;
        QTreeWidget &treeFrames = *ui->treeWidgetFrames;

        QList<QTreeWidgetItem *> items;
        treeFrames.clear();
        for (int i = -1; ir.jumpToImage(i); i--)
        {
            QTreeWidgetItem *parent = new QTreeWidgetItem(
                                    (QTreeWidget*)0
                                    , QStringList(QString("Block %1").arg(qAbs(i) - 1))
                                    );
            items.append(parent);

            for (int j = 0; j < ir.imageCount(); j++)
            {
                QTreeWidgetItem *child = new QTreeWidgetItem(
                                    parent
                                    , QStringList(QString("frame: %1").arg(j))
                                    );
                items.append(child);
            }
            treeFrames.addTopLevelItems(items);
        }
        ir.jumpToImage(-1);


        QString s = model.fileName(index);
        label.setText(s);
        if (ir.read(&im))
        {
            label.setText(s + QString(" width: %1 height %2").arg(im.rect().width()).arg(im.rect().height()));

            pix = QPixmap::fromImage(im);
            //QPixmap pix(model.filePath(index));
            scene.clear();
            scene.setSceneRect(pix.rect());
            scene.addPixmap(pix);
        }
        else
            label.setText(s + ": fail read");
    }
}

void MainWindow::JumpToImage(QModelIndex index)
{
    QModelIndex parent = index.parent();
    if (parent.isValid())
    {
        if (ir.jumpToImage(-(parent.row() + 1)))
        {
            if (index.isValid())
            {
                ir.jumpToImage(index.row());
                ir.read(&im);

                pix = QPixmap::fromImage(im);
                scene.clear();
                scene.addPixmap(pix);
                scene.setSceneRect(pix.rect());
            }
        }
    }
}



void MainWindow::on_treeView_activated(QModelIndex index)
{
    LoadImage(index);
}

void MainWindow::on_treeView_clicked(QModelIndex index)
{
    LoadImage(index);
}

void MainWindow::on_treeWidgetFrames_activated(QModelIndex index)
{
    JumpToImage(index);
}

void MainWindow::on_treeWidgetFrames_clicked(QModelIndex index)
{
    JumpToImage(index);
}

void MainWindow::on_checkBox_toggled(bool checked)
{
    if (checked)
        timer.start(delay);
    else
        timer.stop();
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    delay = position;
    if (timer.isActive())
    {
        timer.stop();
        timer.start(delay);
    }
}

