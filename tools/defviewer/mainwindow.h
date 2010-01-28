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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindowClass;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onJumpToImage();

private:
    Ui::MainWindowClass *ui;

    QDirModel model;
    QGraphicsScene scene;
    QPainter painter;
    QImageReader ir;
    QImage im;
    QPixmap pix;

    int delay;
    QTime time;
    QTimer timer;

    void LoadImage(QModelIndex index);
    void JumpToImage(QModelIndex index);

private slots:
    void on_horizontalSlider_sliderMoved(int position);
    void on_checkBox_toggled(bool checked);
    void on_treeWidgetFrames_clicked(QModelIndex index);
    void on_treeWidgetFrames_activated(QModelIndex index);
    void on_treeView_clicked(QModelIndex index);
    void on_treeView_activated(QModelIndex index);
};

#endif // MAINWINDOW_H
