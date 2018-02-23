#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QImage>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QWidget>
#include <qpainter.h>
#include <opencv2/opencv.hpp>

#include "image_scissor.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void iniParam();
    bool FindImagePosition(const int &g_x, const int &g_y, int &i_x, int &i_y);
    void ShowImage(const cv::Mat &image, int c);

    ImageScissor _image_scissor;

    bool _b_action_set_seed;
    bool _b_action_mouse_press;
    bool _b_open_img;
    bool _b_save_mask;

private slots:

    void on_zoomIn_clicked();

    void on_zoomOut_clicked();

    void on_actionOpen_File_triggered();

    void on_horizontalSlider_sliderMoved(int position);

    void on_actionExit_triggered();

    void on_actionSet_Seed_triggered();

    void on_actionDelete_Latest_Seed_triggered();

    void on_actionCost_Graph_triggered();

    void on_actionPixel_Node_triggered();

    void on_actionMinimum_Path_triggered();

    void on_actionSave_Mask_triggered();

private:
    Ui::MainWindow *ui;

protected:
    // mouse event: http://blog.sina.com.cn/s/blog_8b97b05e0100v6kk.html
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void paintEvent(QPaintEvent *e);
};

#endif // MAINWINDOW_H
