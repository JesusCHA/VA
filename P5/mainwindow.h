#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <rcdraw.h>



using namespace cv;
using namespace std;

namespace Ui {
    class MainWindow;
}



class MainWindow :

     public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer timer;

    VideoCapture *cap;
    RCDraw *visorS, *visorD, *visorBS, *visorBD;
    QImage *imgS, *imgD, *imgBS, *imgBD;
    Mat colorImage, grayImage, destColorImage, destGrayImage, bottomGrayImage, destBottomGrayImage;
    Mat gray2ColorImage, destGray2ColorImage;
    bool capture, showColorImage, winSelected,newImage;
    Rect imageWindow;
    Mat visitados, imgBorde, segmentada, pfijos;
    std::vector<Point> LFrontera;
    Mat dispImg;
    Mat bordeImg;
    float ancho;
 /*  struct region{
        int id;
        int g;
        vector<Point> PFrontera;
    };*/


    struct corner{
            Point coor;
            float vcoor;
            bool homologo;

    };

    struct greater_than
    {
        inline bool operator() (const corner& corner1, const corner& corner2)
        {
            return (corner1.vcoor > corner2.vcoor);
        }
    };

    vector <uchar> LRegiones;
    vector <float> DispRegiones;
    Mat corner1, corner2;
     std::vector<corner> LCorner;
     std::vector<Point> homologos;

public slots:
    void compute();
    //void start_stop_capture(bool start);
  //  void change_color_gray(bool color);
    void selectWindow(QPointF p, int w, int h);
    void deselectWindow();
    void load_from_file();
    void analisisRegion(const Point &inicial);
    void inicializar();
    void segmentar();
    void generarImagen();
    std::vector<cv::Point> getRegions();
    void drawRegions();
    void disparity();
    void calcularEsquinas();
    void showCorners();
    void calcMediaDisp();
    void generarImagenDisp();
    void dispersion(const Point &inicial);
    void propagar();
   void showDispImg();
   void calcAfterLoadImage();
   void loadGround();


};


#endif // MAINWINDOW_H
