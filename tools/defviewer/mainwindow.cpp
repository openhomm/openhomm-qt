// openhomm - open source clone of Heroes of Might and Magic III
// Copyright (C) 2009-2010 openhomm developers team (see AUTHORS)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
    ui->setupUi(this);
    QGridLayout &grid = *ui->gridLayout;
    grid.addWidget(&tree, 0, 0, 0, 1);
    connect(&tree, SIGNAL(clicked(QModelIndex)), this, SLOT(onTreeActivated(QModelIndex)));
    connect(&tree, SIGNAL(activated(QModelIndex)), this, SLOT(onTreeActivated(QModelIndex)));
    connect(&tree, SIGNAL(highlighted(QModelIndex)), this, SLOT(onTreeActivated(QModelIndex)));
    model.setLazyChildCount(true);
    model.setSorting(QDir::DirsFirst);
    tree.setModel(&model);
    tree.setColumnWidth(0, 300);

    QTreeWidget &treeFrames = *ui->treeWidgetFrames;
    treeFrames.setColumnCount(1);
    treeFrames.setHeaderLabels(QStringList("Frames"));

    delay = 100;
    connect(&timer, SIGNAL(timeout()), this, SLOT(onJumpToImage()));

    QGraphicsView &view = *ui->graphicsView;
    view.resize(600, 400);
    view.setScene(&scene);

    ui->label->setText("");
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
        for (int i = -1; i > -100; i--)
        {
            if (ir.jumpToImage(i))
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
        }
        ir.jumpToImage(-1);


        QString s = model.fileName(index);
        label.setText(s);
        if (ir.read(&im))
        {
            label.setText(s + QString(" width: %1 height: %2").arg(im.rect().width()).arg(im.rect().height()));

            pix = QPixmap::fromImage(im);
            scene.clear();
            scene.setSceneRect(pix.rect());
            scene.addPixmap(pix);
        }
        else
        {
            scene.clear();
            label.setText(s + ": fail read");
        }
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



void MainWindow::onTreeActivated(QModelIndex index)
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

