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

    connect(ui->addButton,SIGNAL(clicked(bool)),this,SLOT(addObject()));
    connect(ui->delButton,SIGNAL(clicked(bool)),this,SLOT(removeObject()));
    matcher = BFMatcher(cv::NORM_HAMMING);
    objetos.resize(3);
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
    match();
    showImageObject();
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

void MainWindow::showImageObject()
{
    Mat img;
    tipo_objeto obj;
    uint i;
    obj = objetos[ui->objectComboBox->currentIndex()];
    qDebug()<<obj.imagenes.size();
    if(!obj.imagenes.empty()){
        i = ui->objScrollBar->value();
        qDebug()<<"valor: "<<i;
        if(i>=obj.imagenes.size())
            i = obj.imagenes.size() - 1;
        img = obj.imagenes[i];
        destGrayImage.setTo(cv::Scalar(0,0,0));
        Mat frag2(destGrayImage, cv::Rect((320-img.cols)/2, (240-img.rows)/2,img.cols, img.rows));
        img.copyTo(frag2);
    }else
         destGrayImage.setTo(cv::Scalar(0,0,0));

}
void MainWindow::addObject()
{

            tipo_objeto obj;
            vector <KeyPoint> kp;
            vector <Point2f> v;
            Mat descriptor;
            Mat img;
            obj = objetos[ui->objectComboBox->currentIndex()];
            qDebug()<<obj.imagenes.size();

            //imagen
            Mat frag1(grayImage, imageWindow);
            if(winSelected){
                detector->detect(frag1, kp, cv::Mat());
                if(!kp.empty()){
                    detector->compute(frag1, kp, descriptor);
                    for(uint i = 0; i<v.size(); i++){
                       v.push_back(kp[i].pt);
                    }
                    frag1.copyTo(img);
                    obj.imagenes.push_back(img);
                    obj.descriptores.push_back(descriptor);
                    obj.coords.push_back(v);

                    objetos[ui->objectComboBox->currentIndex()] = obj;
                }
        }

}

void MainWindow::removeObject(){
    objetos[ui->objectComboBox->currentIndex()] = tipo_objeto();
}

void MainWindow::match(){
    tipo_objeto obj2;
    DMatch m;
    vector<DMatch> matches;
    tipo_objeto obj;
    vector <KeyPoint> kp;
    vector <vector <Point2f> > region;
    region.resize(3);
    Mat descriptor;
    vector<int> colectionToObject;
    Rect rect;
    int numobj;

    detector->detect(grayImage, kp, cv::Mat());
    if(!kp.empty()){
        detector->compute(grayImage, kp, descriptor);
        matcher.clear();
        colectionToObject.clear();
        for(uint i=0;i<objetos.size();i++){
            obj2 = objetos[i];
            for(uint j = 0; j<obj2.imagenes.size();j++)
                colectionToObject.push_back(i);
            matcher.add(obj2.descriptores);
        }

        matcher.match(descriptor, matches, vector<Mat>());

        for(uint i=0;i<matches.size();i++){
            m = matches[i];
            numobj = colectionToObject[m.imgIdx];
            obj2 = objetos[numobj];
            if(m.distance<=60){
                region[numobj].push_back(kp[m.queryIdx].pt);
            }

        }
        if(region[0].size()>60){
            rect = cv::boundingRect(region[0]);
            visorS->drawSquare(QPointF(rect.x+rect.width/2, rect.y+rect.height/2), rect.width,rect.height, Qt::green );
        }
        if(region[1].size()>60){
            rect = cv::boundingRect(region[1]);
            visorS->drawSquare(QPointF(rect.x+rect.width/2, rect.y+rect.height/2), rect.width,rect.height, Qt::blue );
        }
        if(region[2].size()>60){
            rect = cv::boundingRect(region[2]);
            visorS->drawSquare(QPointF(rect.x+rect.width/2, rect.y+rect.height/2), rect.width,rect.height, Qt::red );
        }

    }
    /*
con queryidx accedemos al la lista.
imgidx indice dentro de la coleccion donde esta ese punto-> indice de objeto.

boundingrect calcular el rectangulo.


*/




}


void MainWindow::deselectWindow()
{
    winSelected = false;
}


