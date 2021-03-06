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
    connect(pt.okButton,SIGNAL(clicked()),this,SLOT(tfPxb()));
    connect(ui->kernelButton,SIGNAL(clicked(bool)),&lf,SLOT(open()));
    connect(lf.okButton,SIGNAL(clicked()),this,SLOT(closekernel()));
    connect(ui->operOrderButton,SIGNAL(clicked(bool)),&op,SLOT(open()));
    connect(op.okButton,SIGNAL(clicked()),this,SLOT(closeorder()));
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


    if(showColorImage){
        memcpy(imgS->bits(), colorImage.data , 320*240*3*sizeof(uchar));
        memcpy(imgD->bits(), destColorImage.data , 320*240*3*sizeof(uchar));
    }
    else{
        cvtColor(grayImage,gray2ColorImage, CV_GRAY2RGB);
        cvtColor(destGrayImage,destGray2ColorImage, CV_GRAY2RGB);
        memcpy(imgS->bits(), gray2ColorImage.data , 320*240*3*sizeof(uchar));
        memcpy(imgD->bits(), destGray2ColorImage.data , 320*240*3*sizeof(uchar));
    }

    if(winSelected)
    {
        visorS->drawSquare(QPointF(imageWindow.x+imageWindow.width/2, imageWindow.y+imageWindow.height/2), imageWindow.width,imageWindow.height, Qt::green );
    }

    operationSwitch(ui->operationComboBox);
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
}catch (Exception e){
    }

}


void MainWindow::operationSwitch(QComboBox *p){
    int val=p->currentIndex();
    switch (val) {
    case 0:
        transformPx();
        break;
    case 1:
        umbralizar();
        break;
    case 2:
        ecualizar();
        break;
    case 3:
        suavizadoGauss();
        break;
    case 4:
        filtroMed();
        break;
    case 5:
        filtroLin();
        break;
    case 6:
        dilate();
        break;
    case 7:
        erosion();
        break;
    case 8:
        orden();
        break;
    default:
        break;
    }
}

void MainWindow::orden(){
    if(op.firstOperCheckBox->checkState()){
        operationSwitch(op.operationComboBox1);
    }
    if(op.secondOperCheckBox->checkState()){
        opSwitchb(op.operationComboBox2);
    }
    if(op.thirdOperCheckBox->checkState()){
        opSwitchb(op.operationComboBox3);
    }
    if(op.fourthOperCheckBox->checkState()){
        opSwitchb(op.operationComboBox4);
    }
}



void MainWindow::opSwitchb(QComboBox *p){
    int val=p->currentIndex();
    switch (val) {
    case 0:
        transformPx2();
        break;
    case 1:
        umbralizar2();
        break;
    case 2:
        ecualizar2();
        break;
    case 3:
        suavizadoGauss2();
        break;
    case 4:
        filtroMed2();
        break;
    case 5:
        filtroLin2();
        break;
    case 6:
        dilate2();
        break;
    case 7:
        erosion2();
        break;
    default:
        break;
    }
}

void MainWindow::tfPxb(){
    pt.close();
}

void MainWindow::transformPx(){
    std::vector<uchar> wt;
    wt.resize(256);
    float gdO = pt.newPixelBox1->value();
    float gdi = pt.newPixelBox2->value();
    float gdj = pt.newPixelBox3->value();
    float gdF = pt.newPixelBox4->value();

    float goO = pt.origPixelBox1->value();
    float goi = pt.origPixelBox2->value();
    float goj = pt.origPixelBox3->value();
    float goF = pt.origPixelBox4->value();

    float a,b;

    b = goi - goO;
    for (int i = goO; i < goi; ++i) {
        a = (gdi-gdO)*(i-goO);
        a = a/b + gdO;
        wt[i] = (uchar)a;
    }

    b = goj - goi;
    for (int i = goi; i < goj; ++i) {
        a = (gdj-gdi)*(i-goi);
        a = a/b + gdi;
        wt[i] = (uchar)a;
    }

    b = goF - goj;
    for (int i = goj; i <= goF; ++i) {
        a = (gdF-gdj)*(i-goj);
        a = a/b + gdj;
        wt[i] = (uchar)a;
    }
    cv::LUT(grayImage,wt, destGrayImage);
}

void MainWindow::transformPx2(){
    std::vector<uchar> wt;
    wt.resize(256);
    float gdO = pt.newPixelBox1->value();
    float gdi = pt.newPixelBox2->value();
    float gdj = pt.newPixelBox3->value();
    float gdF = pt.newPixelBox4->value();

    float goO = pt.origPixelBox1->value();
    float goi = pt.origPixelBox2->value();
    float goj = pt.origPixelBox3->value();
    float goF = pt.origPixelBox4->value();

    float a,b;

    b = goi - goO;
    for (int i = goO; i < goi; ++i) {
        a = (gdi-gdO)*(i-goO);
        a = a/b + gdO;
        wt[i] = (uchar)a;
    }

    b = goj - goi;
    for (int i = goi; i < goj; ++i) {
        a = (gdj-gdi)*(i-goi);
        a = a/b + gdi;
        wt[i] = (uchar)a;
    }

    b = goF - goj;
    for (int i = goj; i <= goF; ++i) {
        a = (gdF-gdj)*(i-goj);
        a = a/b + gdj;
        wt[i] = (uchar)a;
    }
    cv::LUT(destGrayImage,wt, destGrayImage);
}


void MainWindow::umbralizar(){
    cv::threshold(grayImage,destGrayImage,ui->thresholdSpinBox->value(),255,CV_THRESH_BINARY);
}

void MainWindow::umbralizar2(){
    cv::threshold(destGrayImage,destGrayImage,ui->thresholdSpinBox->value(),255,CV_THRESH_BINARY);
}

void MainWindow::ecualizar(){
    cv::equalizeHist (grayImage, destGrayImage);
}

void MainWindow::ecualizar2(){
    cv::equalizeHist (destGrayImage, destGrayImage);
}

void MainWindow::suavizadoGauss(){
    int sz = ui->gaussWidthBox->value();
    cv::GaussianBlur(grayImage,destGrayImage,Size(sz,sz),(float)sz/5);
}

void MainWindow::suavizadoGauss2(){
    int sz = ui->gaussWidthBox->value();
    cv::GaussianBlur(destGrayImage,destGrayImage,Size(sz,sz),(float)sz/5);
}

void MainWindow::filtroMed(){
    cv::medianBlur(grayImage,destGrayImage,3);
}

void MainWindow::filtroMed2(){
    cv::medianBlur(destGrayImage,destGrayImage,3);
}

void MainWindow::filtroLin(){

    kernel(0,0) = lf.kernelBox11->value();
    kernel(0,1) = lf.kernelBox12->value();
    kernel(0,2) = lf.kernelBox13->value();

    kernel(1,0) = lf.kernelBox21->value();
    kernel(1,1) = lf.kernelBox22->value();
    kernel(1,2) = lf.kernelBox23->value();

    kernel(2,0) = lf.kernelBox31->value();
    kernel(2,1) = lf.kernelBox32->value();
    kernel(2,2) = lf.kernelBox33->value();

    cv::filter2D(grayImage, destGrayImage, CV_8U , kernel);
}

void MainWindow::filtroLin2(){

    kernel(0,0) = lf.kernelBox11->value();
    kernel(0,1) = lf.kernelBox12->value();
    kernel(0,2) = lf.kernelBox13->value();

    kernel(1,0) = lf.kernelBox21->value();
    kernel(1,1) = lf.kernelBox22->value();
    kernel(1,2) = lf.kernelBox23->value();

    kernel(2,0) = lf.kernelBox31->value();
    kernel(2,1) = lf.kernelBox32->value();
    kernel(2,2) = lf.kernelBox33->value();

    cv::filter2D(destGrayImage, destGrayImage, CV_8U , kernel);
}
void MainWindow::erosion(){
    Mat new_image;
    cv::threshold( grayImage, new_image, ui->thresholdSpinBox->value(), 255, CV_THRESH_BINARY);
    cv::erode(new_image, destGrayImage, cv::Mat(), cv::Point(-1,-1));
}

void MainWindow::erosion2(){
    Mat new_image;
    cv::threshold( destGrayImage, new_image, ui->thresholdSpinBox->value(), 255, CV_THRESH_BINARY);
    cv::erode(new_image, destGrayImage, cv::Mat(), cv::Point(-1,-1));
}

void MainWindow::dilate(){
    Mat new_image;
    cv::threshold( grayImage, new_image, ui->thresholdSpinBox->value(), 255, CV_THRESH_BINARY);
    cv::dilate(new_image, destGrayImage, cv::Mat(), cv::Point(-1,-1));
}

void MainWindow::dilate2(){
    Mat new_image;
    cv::threshold( destGrayImage, new_image, ui->thresholdSpinBox->value(), 255, CV_THRESH_BINARY);
    cv::dilate(new_image, destGrayImage, cv::Mat(), cv::Point(-1,-1));
}

void MainWindow::closeorder(){
    op.close();
}

void MainWindow::closekernel(){
    lf.close();
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


