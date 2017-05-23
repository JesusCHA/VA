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
    RCDraw *visorS, *visorD, *visorBD, *visorBS;
    QImage *imgS, *imgD, *imgBD, *imgBS;
    Mat colorImage, grayImage, destColorImage, destGrayImage, gray2ColorImage, destGray2ColorImage;
    Mat imgCanny, segIMage, visitados;
    Mat bottomGrayImage, destBottomGrayImage;
    bool capture, showColorImage, winSelected, bordes;
    Rect imageWindow;
    Mat dispImg, bordeImg, pfijos;
    float ancho;

    struct region{
        int idR;
        int cantidad;
        Point seed;
        uchar color;
        std::vector<Point> frontera;
    };

    struct corner{
        Point coor;
        float vcoor;
        bool  homologo;
    };

    struct greater_than{

            inline bool operator() (const corner& corner1, const corner& corner2)
            {
                return (corner1.vcoor > corner2.vcoor);
            }
     };

    std::vector<region> listRegion;
    vector <float> DispRegiones;
    Mat corner1, corner2;
    std::vector<corner> LCorner;
    std::vector<Point> homologos;

public slots:
    //void load_from_file();
    void compute();
    void start_stop_capture(bool start);
    void change_color_gray(bool color);
    void selectWindow(QPointF p, int w, int h);
    void deselectWindow();
    void loadFile();
    void loadGround();

    void crearRegiones();
    void analisisRegion(Point pInicial, Mat imagen, Mat &visitados, int idReg);
    void dibujarImg();
    void frontera();
    void flagBorde();

    void calcularEsquinas();
    void calcMediaDisp();

    void propagar();
    void dispersion(const Point &inicial);
    void disparity();
    void generarImagenDisp();
    void showDispImg();

    void drawCorners();


};


#endif // MAINWINDOW_H
