#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //cap = new VideoCapture(0);
    cap = new VideoCapture(0);
    if(!cap->isOpened())
        cap = new VideoCapture(1);
    capture = true;
    showColorImage = false;
    winSelected = false;
    bordes = false;

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

    segIMage.create(240,320,CV_32SC1);
    visitados.create(240,320,CV_8UC1);
    visitados.setTo(0);
    segIMage.setTo(-1);


    connect(&timer,SIGNAL(timeout()),this,SLOT(compute()));
    connect(ui->captureButton,SIGNAL(clicked(bool)),this,SLOT(start_stop_capture(bool)));
    connect(ui->colorButton,SIGNAL(clicked(bool)),this,SLOT(change_color_gray(bool)));
    connect(visorS,SIGNAL(windowSelected(QPointF, int, int)),this,SLOT(selectWindow(QPointF, int, int)));
    connect(visorS,SIGNAL(pressEvent()),this,SLOT(deselectWindow()));
    connect(ui->loadimageButton,SIGNAL(clicked(bool)),this,SLOT(loadFile()));
    connect(ui->checkBox, SIGNAL(stateChanged(int)),this, SLOT(flagBorde()));
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

    crearRegiones();
    dibujarImg();

    if(bordes){
        drawBorders();
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


void MainWindow::crearRegiones(){
    visitados.setTo(0);
    segIMage.setTo(-1);
    listRegion.clear();
    Canny(grayImage, imgCanny, 50, 200);
    for (int y = 0; y < 240; ++y) {
        for (int x = 0; x < 320; ++x) {
            //punto identificador -1  y que no sea imagen de borde
            if(segIMage.at<int>(y,x) == -1 && imgCanny.at<int>(y,x) == 0){
                region reg;
                reg.idR = listRegion.size();
                reg.seed = Point(x,y);
                reg.cantidad = 0;
                reg.color = grayImage.at<uchar>(y,x);
                //reg.frontera.push_back();
                listRegion.push_back(reg);
                analisisRegion(reg.seed, imgCanny, visitados, reg.idR);
            }
        }
    }
    frontera();
}

void MainWindow::frontera(){
    int idpAct;
    Point pAct;
    region * r;
    for (int y = 0; y < 240; y++) {
        for (int x = 0; x < 320; x++) {
           idpAct = segIMage.at<int>(y,x);
           if(idpAct >= 0){
               pAct = Point(x,y);
               r = &listRegion[idpAct];
               if(x<319 && segIMage.at<int>(y,x+1) != idpAct){
                    r->frontera.push_back(pAct);
               } else if(y<239 && segIMage.at<int>(y+1,x) != idpAct){
                    r->frontera.push_back(pAct);
               } else if(x > 0 && segIMage.at<int>(y,x-1) != idpAct){
                    r->frontera.push_back(pAct);
               } else if(y > 0 && segIMage.at<int>(y-1,x) != idpAct){
                    r->frontera.push_back(pAct);
               }
            }
        }
    }
}

void MainWindow::flagBorde(){

    if(bordes){
        bordes=false;
    }else{
        bordes=true;
    }
}

void MainWindow::drawBorders(){
   region r;
   Point p;
   qDebug()<< listRegion.size();
   for (uint i = 0; i < listRegion.size(); i++) {
       r = listRegion[i];
       for(uint j = 0; j < r.frontera.size(); j++){
           qDebug()<<"entra" << i << j;
           p = r.frontera[j];
           visorD->drawSquare(QPoint(p.x,p.y),3,3,Qt::green);
           //visorS->drawSquare(QPointF(rect.x+rect.width/2, rect.y+rect.height/2), rect.width,rect.height, Qt::green );
       }
   }
}


void MainWindow::analisisRegion(Point pInicial, Mat imagen, Mat &visitados, int idReg){
    std::vector<Point> lista;
    Point pAct, pNuevo;
    uint i=0;
    lista.push_back(pInicial);
    uchar grisSemilla = grayImage.at<uchar>(pInicial.y, pInicial.x);
    while(i<lista.size()){
        pAct = lista[i];
        if(pAct.x>=0 && pAct.x<320 && pAct.y>=0 && pAct.y<240 && visitados.at<uchar>(pAct.y, pAct.x)==0){
            //Realizar   la   comprobación   correspondiente   sobre   el   píxel
            if(abs (grisSemilla - grayImage.at<uchar>(pAct.y,pAct.x)) < 30){
                visitados.at<uchar>(pAct.y, pAct.x)=1;
                //Si se deben analizar también los vecinos, incluirlos en la lista
                segIMage.at<int>(pAct.y,pAct.x) = idReg;

                pNuevo.x = pAct.x-1;
                pNuevo.y = pAct.y;                
                lista.push_back(pNuevo);
                pNuevo.x = pAct.x+1;
                pNuevo.y = pAct.y;
                lista.push_back(pNuevo);
                pNuevo.x = pAct.x;
                pNuevo.y = pAct.y-1;
                lista.push_back(pNuevo);
                pNuevo.x = pAct.x-1;
                pNuevo.x = pAct.x;
                pNuevo.y = pAct.y+1;
                lista.push_back(pNuevo);
            }
        }
    i++;
    }


    lista.clear();
}

void MainWindow::dibujarImg(){
    for (int y = 0; y < 240; ++y) {
        for (int x = 0; x < 320; ++x) {
            destGrayImage.at<uchar>(y,x) = listRegion[segIMage.at<int>(y,x)].color;
        }
    }
}

void MainWindow::deselectWindow()
{
    winSelected = false;
}


