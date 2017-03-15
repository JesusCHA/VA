#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>


#include <rcdraw.h>
#include <ui_pixelTForm.h>
#include <ui_operOrderForm.h>
#include <ui_lFilterForm.h>

using namespace cv;



class PixelTDialog : public QDialog, public Ui::PixelTForm{
    Q_OBJECT
public:
    PixelTDialog(QDialog *parent=0) : QDialog(parent){
        setupUi(this);
    }
};


class lFilterForm : public QDialog, public Ui::LFilterForm{
    Q_OBJECT
public:
    lFilterForm(QDialog *parent=0) : QDialog(parent){
        setupUi(this);
    }
};


class OperOrderForm : public QDialog, public Ui::OperOrderForm{
    Q_OBJECT
public:
    OperOrderForm(QDialog *parent=0) : QDialog(parent){
        setupUi(this);
    }
};

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer timer;

    VideoCapture *cap;
    RCDraw *visorS, *visorD;
    QImage *imgS, *imgD;
    Mat colorImage, grayImage, destColorImage, destGrayImage;
    Mat gray2ColorImage, destGray2ColorImage;
    bool capture, showColorImage, winSelected;
    Rect imageWindow;

    PixelTDialog pt;
    lFilterForm lf;
    OperOrderForm op;

    Matx33f kernel;
    bool tfPx = false;

public slots:
    void compute();
    void start_stop_capture(bool start);
    void change_color_gray(bool color);
    void selectWindow(QPointF p, int w, int h);
    void deselectWindow();

    void loadFile();
    void saveFile();


    void operationSwitch();

    void closeorder();
    void closekernel();
    void tfPxb();
    void transformPx();

    void umbralizar();
    void ecualizar();
    void suavizadoGauss();
    void filtroMed();
    void filtroLin();
    void erosion();
    void dilate();



};


#endif // MAINWINDOW_H
