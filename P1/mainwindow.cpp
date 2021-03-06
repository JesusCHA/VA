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
    connect(ui->loadFileButton,SIGNAL(clicked(bool)),this,SLOT(loadFile()));
    connect(ui->saveFileButton,SIGNAL(clicked(bool)),this,SLOT(saveFile()));
    connect(ui->copyButton,SIGNAL(clicked(bool)),this,SLOT(copyFile()));
    connect(ui->resizeButton,SIGNAL(clicked(bool)),this,SLOT(resizeFile()));
    connect(ui->enlargeButton,SIGNAL(clicked(bool)),this,SLOT(enlargeFile()));
    connect(ui->zoomTranslation,SIGNAL(clicked(bool)),this,SLOT(zoomFile()));
    connect(ui->warpZoomButton,SIGNAL(clicked(bool)),this,SLOT(warpZoomFile()));

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
        if (wrapAct==true)
            warpZoomFile();
    }
    else
    {
        cvtColor(grayImage,gray2ColorImage, CV_GRAY2RGB);
        cvtColor(destGrayImage,destGray2ColorImage, CV_GRAY2RGB);
        memcpy(imgS->bits(), gray2ColorImage.data , 320*240*3*sizeof(uchar));
        memcpy(imgD->bits(), destGray2ColorImage.data , 320*240*3*sizeof(uchar));
        if (wrapAct==true)
            warpZoomFile();

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
try{
    QString imgload;
    imgload = QFileDialog::getOpenFileName(this,tr("Open Image"), "/home", tr("Image Files (*.png *.jpg *.bmp)"));

    capture = false;
    Mat img = imread(imgload.toStdString());

    cv::resize(img,img, cv::Size(320,240));
    cvtColor(img, grayImage, CV_BGR2GRAY);
    cvtColor(img, colorImage, CV_BGR2RGB);
}catch (Exception e)
    {
    }

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


void MainWindow::resizeFile(){

    if(winSelected){
        if(showColorImage){
            Mat imgTrol1 = colorImage(imageWindow);
            cv::resize(imgTrol1,destColorImage, cv::Size(320,240));
        }else{
            Mat imgTrol1 = grayImage(imageWindow);
            cv::resize(imgTrol1,destGrayImage, cv::Size(320,240));
        }
    }
}

void MainWindow::enlargeFile(){

    if(winSelected){
        float valueY, valuex,  valueComp;
        valuex = 320.0/float(imageWindow.width);
        valueY = 240.0/float(imageWindow.height);
        valueComp = valuex;
        if(valueY < valuex)
            valueComp = valueY;
        if(showColorImage){
            destColorImage.setTo(cv::Scalar(0,0,0));
            Mat imgTrol1(colorImage, imageWindow);
            cv::resize(imgTrol1, imgTrol1, cv::Size(), valueComp, valueComp);
            Mat imgTrolDst(destColorImage, cv::Rect((320-imgTrol1.cols)/2, (240-imgTrol1.rows)/2, imgTrol1.cols, imgTrol1.rows));
            imgTrol1.copyTo(imgTrolDst);
        }else{
            destGrayImage.setTo(0);
            Mat imgTrol1(grayImage, imageWindow);
            cv::resize(imgTrol1, imgTrol1, cv::Size(), valueComp, valueComp);
            Mat imgTrolDst(destGrayImage, cv::Rect((320-imgTrol1.cols)/2, (240-imgTrol1.rows)/2, imgTrol1.cols, imgTrol1.rows));
            imgTrol1.copyTo(imgTrolDst);
        }
    }
}


void MainWindow::copyFile(){

    if(winSelected){
        if(showColorImage){
            Mat imgTrol1(colorImage, imageWindow);
            destColorImage.setTo(cv::Scalar(0,0,0));
            Mat imgtrol2(destColorImage, cv::Rect((320-imageWindow.width)/2, (240-imageWindow.height)/2,imageWindow.width, imageWindow.height));
            imgTrol1.copyTo(imgtrol2);
        }else{
            Mat imgTrol1(grayImage, imageWindow);
            destGrayImage.setTo(0);
            Mat imgtrol2(destGrayImage, cv::Rect((320-imageWindow.width)/2, (240-imageWindow.height)/2,imageWindow.width, imageWindow.height));
            imgTrol1.copyTo(imgtrol2);
        }
    }else{
        if(showColorImage)
            colorImage.copyTo(destColorImage);
        else
            grayImage.copyTo(destGrayImage);
  }


}

void MainWindow::warpZoomFile(){
    if (ui->warpZoomButton->isChecked()){
     wrapAct=true;
     if(showColorImage){
        Mat imgTrol1;
        destColorImage.setTo(cv::Scalar(0,0,0));
        Point2f pt(colorImage.cols/2, colorImage.rows/2.);
        Mat rotMax = getRotationMatrix2D(pt, ui->angleDial->value(), 1.0);
        float valuex = ui->horizontalTranslation->value();
        float valuey = ui->verticalTranslation->value();
        rotMax.at<double>(0,2) = valuex;
        rotMax.at<double>(1,2) = valuey;
        warpAffine(colorImage, imgTrol1, rotMax, Size(colorImage.cols, colorImage.rows));
        cv::resize(imgTrol1, imgTrol1, cv::Size(), ui->zoomTranslation->value(), ui->zoomTranslation->value());
        Mat imgtrol2(imgTrol1, cv::Rect((imgTrol1.cols - 320)/2, (imgTrol1.rows - 240)/2, 320, 240));
        imgtrol2.copyTo(destColorImage);
    }else{
        Mat imgTrol1;
        destGrayImage.setTo(0);
        Point2f pt(grayImage.cols/2, grayImage.rows/2.);
        Mat rotMax = getRotationMatrix2D(pt, ui->angleDial->value(), 1.0);
        float valuex = ui->horizontalTranslation->value();
        float valuey = ui->verticalTranslation->value();
        rotMax.at<double>(0,2) = valuex;
        rotMax.at<double>(1,2) = valuey;
        warpAffine(grayImage, imgTrol1, rotMax, Size(grayImage.cols, grayImage.rows));
        cv::resize(imgTrol1, imgTrol1, cv::Size(), ui->zoomTranslation->value(), ui->zoomTranslation->value());
        Mat imgtrol2(imgTrol1, cv::Rect((imgTrol1.cols - 320)/2, (imgTrol1.rows - 240)/2, 320, 240));
        imgtrol2.copyTo(destGrayImage);
    }
    }else
        wrapAct=false;


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


