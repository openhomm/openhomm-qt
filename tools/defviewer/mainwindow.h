#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

namespace Ui
{
    class MainWindowClass;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    Ui::MainWindowClass *ui;

    QDirModel model;
    QGraphicsScene scene;
    QImageReader ir;
    QImage im;
    QPixmap pix;
    void LoadImage(QModelIndex index);
    void JumpToImage(QModelIndex index);

private slots:
    void on_treeWidgetFrames_clicked(QModelIndex index);
    void on_treeWidgetFrames_activated(QModelIndex index);
    void on_treeView_clicked(QModelIndex index);
    void on_treeView_activated(QModelIndex index);
};

#endif // MAINWINDOW_H
