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
