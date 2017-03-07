#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cap = new VideoCapture(0);
    if(!cap->isOpened())
        cap = new VideoCapture(1);
    capture = true;
    showColorImage = false;
    winSelected = false;
    cap->set(CV_CAP_PROP_FRAME_WIDTH, 320);
    cap->set(CV_CAP_PROP_FRAME_HEIGHT, 240);
    imgS = new QImage(320,240, QImage::Format_RGB888);
    visorS = new RCDraw(320,240, imgS, ui->imageFrameS);
    imgD = new QImage(320,240, QImage::Format_RGB888);
    visorD = new RCDraw(320,240, imgD, ui->imageFrameD);

    colorImage.create(240,320,CV_8UC3);
    grayImage.create(240,320,CV_8UC1);
    destColorImage.create(240,320,CV_8UC3);
    destGrayImage.create(240,320,CV_8UC1);
    gray2ColorImage.create(240,320,CV_8UC3);
    destGray2ColorImage.create(240,320,CV_8UC3);

    connect(&timer,SIGNAL(timeout()),this,SLOT(compute()));
    connect(ui->captureButton,SIGNAL(clicked(bool)),this,SLOT(start_stop_capture(bool)));
    connect(ui->colorButton,SIGNAL(clicked(bool)),this,SLOT(change_color_gray(bool)));
    connect(visorS,SIGNAL(windowSelected(QPointF, int, int)),this,SLOT(selectWindow(QPointF, int, int)));
    connect(visorS,SIGNAL(pressEvent()),this,SLOT(deselectWindow()));
    connect(ui->loadButton,SIGNAL(clicked(bool)),this,SLOT(loadFile()));
    connect(ui->saveButton,SIGNAL(clicked(bool)),this,SLOT(saveFile()));
    connect(ui->pixelTButton,SIGNAL(clicked()),&pt,SLOT(open()));
    connect(pt.okButton,SIGNAL(clicked()),&pt,SLOT(close()));
    connect(ui->kernelButton,SIGNAL(clicked(bool)),&lf,SLOT(open()));
    connect(ui->operOrderButton,SIGNAL(clicked(bool)),&op,SLOT(open()));
    timer.start(60);


}

MainWindow::~MainWindow()
{
    delete ui;
    delete cap;
    delete visorS;
    delete visorD;
    delete imgS;
    delete imgD;

}

void MainWindow::compute()
{

    if(capture && cap->isOpened())
    {
        *cap >> colorImage;

        cvtColor(colorImage, grayImage, CV_BGR2GRAY);
        cvtColor(colorImage, colorImage, CV_BGR2RGB);

    }


    if(showColorImage)
    {
        memcpy(imgS->bits(), colorImage.data , 320*240*3*sizeof(uchar));
        memcpy(imgD->bits(), destColorImage.data , 320*240*3*sizeof(uchar));
    }
    else
    {
        cvtColor(grayImage,gray2ColorImage, CV_GRAY2RGB);
        cvtColor(destGrayImage,destGray2ColorImage, CV_GRAY2RGB);
        memcpy(imgS->bits(), gray2ColorImage.data , 320*240*3*sizeof(uchar));
        memcpy(imgD->bits(), destGray2ColorImage.data , 320*240*3*sizeof(uchar));

    }

    if(winSelected)
    {
        visorS->drawSquare(QPointF(imageWindow.x+imageWindow.width/2, imageWindow.y+imageWindow.height/2), imageWindow.width,imageWindow.height, Qt::green );
    }
    visorS->update();
    visorD->update();

}

void MainWindow::start_stop_capture(bool start)
{
    if(start)
    {
        ui->captureButton->setText("Stop capture");
        capture = true;
    }
    else
    {
        ui->captureButton->setText("Start capture");
        capture = false;
    }
}

void MainWindow::change_color_gray(bool color)
{
    if(color)
    {
        ui->colorButton->setText("Gray image");
        showColorImage = true;
    }
    else
    {
        ui->colorButton->setText("Color image");
        showColorImage = false;
    }
}


void MainWindow::loadFile(){

    QString imgload;
    imgload = QFileDialog::getOpenFileName(this,tr("Open Image"), "/home", tr("Image Files (*.png *.jpg *.bmp)"));

    capture = false;
    Mat img = imread(imgload.toStdString());

    cv::resize(img,img, cv::Size(320,240));
    cvtColor(img, grayImage, CV_BGR2GRAY);
    cvtColor(img, colorImage, CV_BGR2RGB);

}


void MainWindow::saveFile(){

try{
    QString imgsave;

    imgsave = QFileDialog::getSaveFileName(this,tr("Save Image"), "/home/default.png", tr("Image Files (*.png *.jpg *.bmp)"));

    Mat img;

    if(showColorImage)
    {
        cvtColor(destColorImage, img, CV_RGB2BGR);
        imwrite(imgsave.toStdString() , img);

    }
    else {
        cvtColor(destGrayImage, img, CV_GRAY2BGR );
        imwrite(imgsave.toStdString() , img);
    }
}catch (Exception e)
    {
    }

}


void MainWindow::transformPx(){
    std::vector<uchar> wt[256];
    int gdO = pt.newPixelBox1->value();
    int gdi = pt.newPixelBox2->value();
    int gdj = pt.newPixelBox3->value();
    int gdF = pt.newPixelBox4->value();

    int goO = pt.origPixelBox1->value();
    int goi = pt.origPixelBox2->value();
    int goj = pt.origPixelBox3->value();
    int goF = pt.origPixelBox4->value();

    int a,b;
    b = goi - goO;

/*    for (int i = goO; i < goi; ++i) {
        a = (gdi-gdO)*(i-goi);
        a = a/b;
        wt[i] = (char)a;
    }

    for (int i = goi; i < goj; ++i) {
        a = (gdj-gdi)*(i-goj);
        a = a/b;
        wt[i] = (char)a;
    }

    for (int i = goj; i < goF; ++i) {
        a = (gdF-gdj)*(i-goF);
        a = a/b;
        wt[i] = (char)a;
    }

    //preguntar cómo pasar de int a uchar

    //usar funcion LUT
    cv::LUT(grayImage,wt, destGrayImage);

   // wt.resize(256);
*/
}

void MainWindow::selectWindow(QPointF p, int w, int h)
{
    QPointF pEnd;
    if(w>0 && h>0)
    {
        imageWindow.x = p.x()-w/2;
        if(imageWindow.x<0)
            imageWindow.x = 0;
        imageWindow.y = p.y()-h/2;
        if(imageWindow.y<0)
            imageWindow.y = 0;
        pEnd.setX(p.x()+w/2);
        if(pEnd.x()>=320)
            pEnd.setX(319);
        pEnd.setY(p.y()+h/2);
        if(pEnd.y()>=240)
            pEnd.setY(239);
        imageWindow.width = pEnd.x()-imageWindow.x;
        imageWindow.height = pEnd.y()-imageWindow.y;

        winSelected = true;
    }
}

void MainWindow::deselectWindow()
{
    winSelected = false;
}


