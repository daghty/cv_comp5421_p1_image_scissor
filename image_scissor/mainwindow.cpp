#include "mainwindow.h"
#include "ui_mainwindow.h"

#define DEBUG 1

using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    iniParam();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::iniParam()
{
    _b_open_img = false;
    _b_action_set_seed = false;
    _b_action_mouse_press = false;
    _b_save_mask = false;
    std::cout << "[INFO]: Set Bool False" << std::endl;

    this->centralWidget()->setMouseTracking(true);
    this->setMouseTracking(true);
    ui->imgviewer_label->setMouseTracking(true);

}

QImage Mat2QImage(const cv::Mat &mat)
{
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        std::cerr << "ERROR: Mat could not be converted to QImage." << std::endl;
        return QImage();
    }
}

void MainWindow::on_zoomIn_clicked()
{
    if (_image_scissor._img_src.data)
    {
        cv::Mat temp = _image_scissor._img_src.clone();
        int rows = _image_scissor._row;
        int cols = _image_scissor._col;
        cv::resize(temp, temp, Size(int(cols*1.25), int(rows*1.25)), 0,0,INTER_LINEAR);
        _image_scissor.setInput(temp);
        QImage q_img = Mat2QImage(temp);
        ui->imgviewer_label->setPixmap(QPixmap::fromImage(q_img));
        ui->imgviewer_label->resize(ui->imgviewer_label->pixmap()->size());
    }
}

void MainWindow::on_zoomOut_clicked()
{
    if (_image_scissor._img_src.data)
    {
        cv::Mat temp = _image_scissor._img_src.clone();
        int rows = _image_scissor._row;
        int cols = _image_scissor._col;
        cv::resize(temp, temp, Size(int(cols*0.75), int(rows*0.75)), 0,0,INTER_LINEAR);
        _image_scissor.setInput(temp);
        QImage q_img = Mat2QImage(temp);
        ui->imgviewer_label->setPixmap(QPixmap::fromImage(q_img));
        ui->imgviewer_label->resize(ui->imgviewer_label->pixmap()->size());
    }
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    std::cout << "[INFO]: Position of Slide: " << position << std::endl;
//    if (position < 20) return;
//    if (_image_scissor._img_src.data)
//    {
//        cv::Mat temp = _image_scissor._img_src.clone();
//        int rows = _image_scissor._row;
//        int cols = _image_scissor._col;
//        cv::resize(temp, temp, Size(int(cols*position/50.0), int(rows*position/50.0)), 0,0,INTER_LINEAR);
//        _image_scissor.setInput(temp);
//        QImage q_img = QImage((const unsigned char*)(temp.data), temp.cols, temp.rows, QImage::Format_RGB888);
//        ui->label->setPixmap(QPixmap::fromImage(q_img));
//        ui->label->resize(ui->label->pixmap()->size());
//    }
}

/*
 * @brief: convert the mouse position in the mainwindow to image position
 * @param[in]: g_x, g_y: mouse position in the mainwindow
 * @param[out]: i_x, i_y: image position
 */
bool MainWindow::FindImagePosition(const int &g_x, const int &g_y,
                                    int &i_x, int &i_y)
{
    bool f_x = true;
    bool f_y = true;
    if (g_x < ui->imgviewer_label->x())
    {
        i_x = 0;
        f_x = false;
    }
    else if (g_x >= ui->imgviewer_label->x() + ui->imgviewer_label->width())
    {
        i_x = ui->imgviewer_label->width() - 1;
        f_x = false;
    }
    else
    {
        i_x = g_x - ui->imgviewer_label->x();
    }

    if (g_y < ui->imgviewer_label->y())
    {
        i_y = 0;
        f_y = false;
    }
    else if (g_y >= ui->imgviewer_label->y() + ui->imgviewer_label->height())
    {
        i_y = ui->imgviewer_label->height() - 1;
        f_y = false;
    }
    else
    {
        i_y = g_y - ui->imgviewer_label->y();
    }
    return (f_x & f_y);
}

void MainWindow::ShowImage(const cv::Mat &image, int c = 0)
{
    if (c == 0)
    {
        QImage q_img = Mat2QImage(image);
        ui->imgviewer_label->setPixmap(QPixmap::fromImage(q_img));
        ui->imgviewer_label->resize(ui->imgviewer_label->pixmap()->size());
    }
    else if (c == 1)
    {
        QImage q_img = Mat2QImage(image);
        ui->maskviewer_label->setPixmap(QPixmap::fromImage(q_img));
        ui->maskviewer_label->resize(ui->maskviewer_label->pixmap()->size());
    }
}

/*
 * @brief: open an image
 */
void MainWindow::on_actionOpen_File_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, tr("open"), tr("../"));
    if (!path.isEmpty())
    {
        cv::Mat img = cv::imread(path.toStdString());
//        cv::Mat img = cv::imread("../curless.png");
        if (!img.data)
        {
            QMessageBox::warning(this, tr("warning"), tr("Something wrong in this image ..."));
            return;
        } else
        {
            std::cout << "[INFO]: Open an Image" << std::endl;
            _b_open_img = true;
            _image_scissor.setInput(img);
            ShowImage(img);
        }
    } else
    {
        QMessageBox::warning(this, tr("warning"), tr("Path is wrong, please select a right file ..."));
        return;
    }
}

/*
 * @brief: set a seed
 */
void MainWindow::on_actionSet_Seed_triggered()
{
    if (!_b_open_img) return;
    _b_action_set_seed = true;
    _b_save_mask = true;
#ifndef DEBUG
    on_actionCost_Graph_triggered();
#endif
    std::cout << "[BOOL] Set Seed Menu: " << _b_action_set_seed << std::endl;
}

/*
 * @brief: delete the latest seed
 */
void MainWindow::on_actionDelete_Latest_Seed_triggered()
{
    if (!_b_action_set_seed) return;
    _image_scissor.deleteSeed();
    if (_image_scissor._seed.empty())
    {
        _b_action_set_seed = false;
        _b_action_mouse_press = false;
        _image_scissor.clearParam();
        _image_scissor._draw_img = _image_scissor._img_src.clone();
    }
    std::cout << "[INFO] Delete Seed" << std::endl;
}

/*
 * @brief: mouse press triggered function
 */
void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if (!_b_action_set_seed) return;

    int px, py;
    if (!FindImagePosition(e->x(), e->y(), px, py)) return;
    _b_action_mouse_press = true;
    _image_scissor.addSeed(py, px);
    ui->seedpos_label->setText("Seed Position x:" + QString::number(px) + ", y:" + QString::number(py));
}

/*
 * @brief: mouse move triggered function
 */
void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    ui->global_mousepos_label->setText("Global Mouse Position x:" + QString::number(e->x ()) + ", y:" + QString::number(e->y ()));
    int px, py;
    bool b = FindImagePosition(e->x(), e->y(), px, py);
    ui->local_mousepos_label->setText("Local Mouse Position x:" + QString::number(px) + ", y:" + QString::number(py));
#ifndef DEBUG_MOVE
    if (_b_action_mouse_press)
    {
        _image_scissor.setDest(py, px);
        _image_scissor.DrawShortestPath();
        ShowImage(_image_scissor._draw_img);
    }
#endif
}

void MainWindow::paintEvent(QPaintEvent *e)
{
////    Q_UNUSED(e);
//    QPainter painter(this);
//    painter.setPen(QPen(Qt::red, 3));
//    painter.drawPoint(rand()%400, rand()%400);
////    std::cout << "[INFO] Path Size: " << _image_scissor._path.size() << std::endl;
//    for (size_t i = 0; i < _image_scissor._path.size(); i++)
//    {
//        int px = _image_scissor._path[i][1];
//        int py = _image_scissor._path[i][0];
////        painter.drawPoint(px + ui->imgviewer_label->x(), py + ui->imgviewer_label->y());
//        painter.drawPoint(500, 500);
//    }
////    std::cout << std::endl;
}

/*
 * @brief: keyboard press triggered function
 */
void MainWindow::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
        case Qt::Key_Enter :
        {
            _image_scissor.getMask();
            ShowImage(_image_scissor._draw_img);
            ShowImage(_image_scissor._mask, 1);
            _b_action_set_seed = false;
            _b_action_mouse_press = false;
            _image_scissor.clearParam();
            break;
        }
        case Qt::Key_S : break;
        case Qt::Key_A : break;
        case Qt::Key_D : break;
        default:
            break;
    }
}

void MainWindow::on_actionPixel_Node_triggered()
{
    if (!_b_open_img) return;
    if (_image_scissor._pixel_node.empty()) return;
    cv::imshow("Pixel Node", _image_scissor._pixel_node);
    cv::waitKey(0);
}

void MainWindow::on_actionCost_Graph_triggered()
{
    if (!_b_open_img) return;
    if (_image_scissor._cost_graph.empty()) return;
    cv::imshow("Cost Graph", _image_scissor._cost_graph);
    cv::imshow("Link Cost", _image_scissor._link_cost);
    cv::waitKey(0);
}

void MainWindow::on_actionMinimum_Path_triggered()
{
    if (!_b_action_set_seed) return;
    if (_image_scissor._minipath_tree.empty()) return;
    cv::imshow("Minimum Path", _image_scissor._minipath_tree);
    cv::waitKey(0);
}

void MainWindow::on_actionSave_Mask_triggered()
{
    if (!_b_save_mask) return;
    cv::imwrite("/home/jjiao/Documents/cv_ws/project_1/image_scissor/mask.png", _image_scissor._mask);
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

