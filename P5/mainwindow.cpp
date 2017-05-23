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
    capture = false;
    showColorImage = false;
    winSelected = false;
    bordes = false;

    cap->set(CV_CAP_PROP_FRAME_WIDTH, 320);
    cap->set(CV_CAP_PROP_FRAME_HEIGHT, 240);
    imgS = new QImage(320,240, QImage::Format_RGB888);
    visorS = new RCDraw(320,240, imgS, ui->imageFrameS);
    imgD = new QImage(320,240, QImage::Format_RGB888);
    visorD = new RCDraw(320,240, imgD, ui->imageFrameD);

    imgBS = new QImage(320,240, QImage::Format_RGB888);
    visorBS = new RCDraw(320,240, imgBS, ui->imageFrameBS);
    imgBD = new QImage(320,240, QImage::Format_RGB888);
    visorBD = new RCDraw(320,240, imgBD, ui->imageFrameBD);

    colorImage.create(240,320,CV_8UC3);
    grayImage.create(240,320,CV_8UC1);
    destColorImage.create(240,320,CV_8UC3);
    destGrayImage.create(240,320,CV_8UC1);
    bottomGrayImage.create(240,320,CV_8UC1);
    destBottomGrayImage.create(240,320,CV_8UC1);
    gray2ColorImage.create(240,320,CV_8UC3);
    destGray2ColorImage.create(240,320,CV_8UC3);


    segIMage.create(240,320,CV_32SC1);
    visitados.create(240,320,CV_8UC1);
    dispImg.create(240,320,CV_32FC1);
    pfijos.create(240,320,CV_8UC1);

    visitados.setTo(0);
    segIMage.setTo(-1);
    pfijos.setTo(0);


    connect(&timer,SIGNAL(timeout()),this,SLOT(compute()));
    connect(ui->captureButton,SIGNAL(clicked(bool)),this,SLOT(start_stop_capture(bool)));
    connect(ui->colorButton,SIGNAL(clicked(bool)),this,SLOT(change_color_gray(bool)));
    connect(visorS,SIGNAL(windowSelected(QPointF, int, int)),this,SLOT(selectWindow(QPointF, int, int)));
    connect(visorS,SIGNAL(pressEvent()),this,SLOT(deselectWindow()));
    connect(ui->loadimageButton,SIGNAL(clicked(bool)),this,SLOT(loadFile()));
    connect(ui->LoadGrtruthButton,SIGNAL(clicked(bool)),this,SLOT(loadGround()));
    connect(ui->showCorners, SIGNAL(stateChanged(int)),this, SLOT(flagBorde()));
    connect(ui->InitializeDisButton,SIGNAL(clicked(bool)),this,SLOT(calcMediaDisp()));
    connect(ui->propagateDisButton,SIGNAL(clicked(bool)),this,SLOT(generarImagenDisp()));

    timer.start(60);

}

MainWindow::~MainWindow(){
    delete ui;
    delete cap;
    delete visorS;
    delete visorD;
    delete imgS;
    delete imgD;
    delete visorBS;
    delete visorBD;
    delete imgBS;
    delete imgBD;
}

void MainWindow::compute()
{
    if(capture && cap->isOpened())
    {
        *cap >> colorImage;
        cvtColor(colorImage, grayImage, CV_BGR2GRAY);
        cvtColor(colorImage, destGrayImage, CV_BGR2GRAY);
    }


    //dibujarImg();

    if(bordes){
        drawCorners();
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


        cvtColor(bottomGrayImage,gray2ColorImage, CV_GRAY2RGB);
        cvtColor(destBottomGrayImage,destGray2ColorImage, CV_GRAY2RGB);

        memcpy(imgBS->bits(), gray2ColorImage.data , 320*240*3*sizeof(uchar));
        memcpy(imgBD->bits(), destGray2ColorImage.data , 320*240*3*sizeof(uchar));
    }

    if(winSelected)
    {
        visorS->drawSquare(QPointF(imageWindow.x+imageWindow.width/2, imageWindow.y+imageWindow.height/2), imageWindow.width,imageWindow.height, Qt::green );
    }
    visorS->update();
    visorD->update();
    visorBD->update();
    visorBS->update();


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
        QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open Image", "Open Image"), "/home/~", tr("Image Files (*.png *.jpg *.bmp)"));

        if(fileNames.size() == 2){

            capture = false;
            colorImage = imread(fileNames[0].toStdString(), CV_LOAD_IMAGE_COLOR);
            destColorImage = imread(fileNames[1].toStdString(), CV_LOAD_IMAGE_COLOR);
             ancho = colorImage.cols;
            cvtColor(colorImage, grayImage, CV_BGR2GRAY);
            cvtColor(colorImage, colorImage, CV_BGR2RGB);

            cvtColor(destColorImage, destGrayImage, CV_BGR2GRAY);
            cvtColor(destColorImage, destColorImage, CV_BGR2RGB);

            cv::resize(colorImage, colorImage, Size(320, 240));//comprobar si es del mismo tamaño
            cv::resize(grayImage, grayImage, Size(320, 240));

            cv::resize(destColorImage, destColorImage, Size(320, 240));//comprobar si es del mismo tamaño
            cv::resize(destGrayImage, destGrayImage, Size(320, 240));

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

}catch (Exception e)
    {
    }
    crearRegiones();
}

void MainWindow::loadGround(){
    try{
        QString imgload;
        imgload = QFileDialog::getOpenFileName(this,tr("Open Image"), "/home", tr("Image Files (*.png *.jpg *.bmp)"));

        capture = false;
        Mat img = imread(imgload.toStdString());

        cv::resize(img,img, cv::Size(320,240));
        cvtColor(img, destBottomGrayImage, CV_BGR2GRAY);
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
    Canny(grayImage, imgCanny, 30, 100);
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
    calcularEsquinas();
    disparity();
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
            if(abs (grisSemilla - grayImage.at<uchar>(pAct.y,pAct.x)) < 40){
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

void MainWindow::propagar(){

    for(int i=0; i<grayImage.rows;i++){
        for(int j=0; j<grayImage.cols;j++){
            if(bordeImg.at<uchar>(i, j) == 0)
                qDebug()<<"ddd";
                dispersion(Point(j, i));
        }
    }
}

void MainWindow::dispersion(const Point &inicial)
{
    std::vector<Point> lista, entorno;
    Point pNuevo;
    uint i=0;
    lista.push_back(inicial);

    while(i<lista.size()){

        Point pAct=lista[i];
        float value = dispImg.at<float>(pAct.y, pAct.x);
        uchar id = segIMage.at<uchar>(pAct.y, pAct.x);

        if(pAct.x>=0 && pAct.x<320 && pAct.y >=0 && pAct.y<240 && (int)visitados.at<uchar>(pAct.y, pAct.x)==0
                &&pfijos.at<uchar>(pAct.y, pAct.x) == 0) {

            visitados.at<uchar>(pAct.y, pAct.x)=1;
            if(bordeImg.at<uchar>(pAct.y, pAct.x) == 0){

                pNuevo.x= pAct.x-1;
                pNuevo.y= pAct.y;
                lista.push_back(pNuevo);
                if(segIMage.at<uchar>(pNuevo.y, pNuevo.x) == id)
                    entorno.push_back(pNuevo);

                pNuevo.x= pAct.x+1;
                pNuevo.y= pAct.y;
                lista.push_back(pNuevo);
                if(segIMage.at<uchar>(pNuevo.y, pNuevo.x) == id)
                    entorno.push_back(pNuevo);

                pNuevo.x= pAct.x;
                pNuevo.y= pAct.y-1;
                lista.push_back(pNuevo);
                if(segIMage.at<uchar>(pNuevo.y, pNuevo.x) == id)
                    entorno.push_back(pNuevo);

                pNuevo.x= pAct.x;
                pNuevo.y= pAct.y+1;
                lista.push_back(pNuevo);
                if(segIMage.at<uchar>(pNuevo.y, pNuevo.x) == id)
                    entorno.push_back(pNuevo);

                for(uint i=0;i<entorno.size();i++)
                    value += dispImg.at<float>(entorno[i].y, entorno[i].x);

                if(entorno.size()>0){
                    value /= (entorno.size()+1);
                    //cout<<"value: "<<value<<endl;
                    dispImg.at<float>(pAct.y, pAct.x) = value;
                }
                entorno.clear();
            }

            //visorD->drawEllipse(pNuevo.x, pNuevo.y, 2, 2, Qt::green);
        }

        i++;
        //cout<<lista.size()<<endl;
    }
    lista.clear();

}

void MainWindow::disparity(){

    Mat result;
    corner p;
    int w, h, W, H;
    w = 9;
    h = 9;
    W = 320;
    H = 240;
    homologos.clear();
    result.create((W-w+1), (H-h+1), grayImage.type());
    double minVal; double maxVal; Point minLoc; Point maxLoc;



    for(uint i=0;i<LCorner.size();i++){
        p = LCorner[i];
        if((p.coor.y + h) <= H && (p.coor.y-h) >= 0 && (p.coor.x-w) >= 0 && (p.coor.x+w) <= W){

            Mat templ(grayImage, cv::Rect(p.coor.x - w/2, p.coor.y - h/2, w, h));
            Mat franja(destGrayImage, cv::Rect(0, p.coor.y-h/2, 320, h));
            cv::matchTemplate(franja, templ, result, CV_TM_CCOEFF_NORMED);
            cv::minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );





            if( maxVal >= 0.9){

                Point hom = Point(maxLoc.x + w/2, p.coor.y);
                LCorner[i].homologo = true;
                homologos.push_back(hom);
                LCorner[i].vcoor = LCorner[i].coor.x - hom.x;

                pfijos.at<uchar>(LCorner[i].coor.y, LCorner[i].coor.x) = 1;
                if(LCorner[i].vcoor >= 0)
                    dispImg.at<float>(LCorner[i].coor.y, LCorner[i].coor.x) = LCorner[i].vcoor;
                else
                    dispImg.at<float>(LCorner[i].coor.y, LCorner[i].coor.x) = 0;

            }else
                LCorner[i].homologo = false;
        }
    }
}


void MainWindow::calcMediaDisp(){

    uchar id;
    std::vector<float> counter;
    counter.resize(listRegion.size());
    DispRegiones.resize(listRegion.size());
    for(uint i = 0;i<counter.size();i++){
        counter[i] = 1;
        DispRegiones[i]=0;
    }

   for(uint i=0; i<LCorner.size();i++){
        if(LCorner[i].homologo){
            id = segIMage.at<uchar>(LCorner[i].coor.y, LCorner[i].coor.x);
            DispRegiones[id]+=LCorner[i].vcoor;
            counter[id]++;
        }
    }
    for(uint i = 0;i<DispRegiones.size();i++){
        DispRegiones[i] = DispRegiones[i]/counter[i];

    }
    qDebug()<<"ddd";

    propagar();
    qDebug()<<"ddd";

    showDispImg();

    /*for(int i = 0;i<DispRegiones.size();i++)
        cout<<"region: "<<i<<" disparidad media: "<<DispRegiones[i]<<endl;*/

}

void MainWindow::calcularEsquinas(){
        Mat dst;

        cv::cornerHarris(grayImage, dst, 3, 3, 0.04);

        corner c;
        LCorner.clear();
        for( int j = 0; j < dst.rows ; j++ )
               { for( int i = 0; i < dst.cols; i++ )
                    {
                      if( dst.at<float>(j,i) > 0.000001)
                        {
                          c.coor.x = i;
                          c.coor.y = j;
                          c.vcoor = dst.at<float>(j,i);
                           LCorner.push_back(c);
                           }

                    }
               }

        std::sort(LCorner.begin(), LCorner.end(), greater_than());
        Point c1, c2;
        uint i = 0, j = 1;
        while(i<LCorner.size()){
           c1 = LCorner[i].coor;
           while(j<LCorner.size()){
                c2 = LCorner[j].coor;
                float dst = (c1.x - c2.x) * (c1.x - c2.x) + (c1.y - c2.y) * (c1.y - c2.y);
                dst = sqrt(dst);
                if(dst <= 5)
                    LCorner.erase(LCorner.begin() + j);
                else
                    j++;
           }
           i++;
           j = i + 1;
        }
    }

void MainWindow::generarImagenDisp(){
    uchar id, final;
    float value;

       for(int i=0; i<dispImg.rows;i++){
            for(int j=0; j<dispImg.cols;j++){
                if(pfijos.at<uchar>(i, j) == 0){
                    id = segIMage.at<uchar>(i, j);
                    dispImg.at<float>(i, j) = DispRegiones[id];
                }
            }
       }

       for(int i=0; i<bottomGrayImage.rows;i++){
           for(int j=0; j<bottomGrayImage.cols;j++){
                value = dispImg.at<float>(i, j) * 3 * ancho/320;
                if(value > 255)
                    final = 255;
                else if(value < 0)
                    final = 0;
                else
                    final = value;
                bottomGrayImage.at<uchar>(i, j) = final;
           }
       }
}

void MainWindow::showDispImg(){
    uchar id, final;
    float value;
    for(int i=0; i<bottomGrayImage.rows;i++){
        for(int j=0; j<bottomGrayImage.cols;j++){
             value = dispImg.at<float>(i, j) * 3 * ancho/320;
             if(value > 255)
                 final = 255;
             else if(value < 0)
                 final = 0;
             else
                 final = value;
             bottomGrayImage.at<uchar>(i, j) = final;
        }
    }
}


void MainWindow::drawCorners(){
    for(uint i = 0; i < LCorner.size();i++){
        Point c1 = LCorner[i].coor;
        visorS->drawEllipse(QPoint(c1.x, c1.y), 1, 1, Qt::red, -1, 0);
    }
    for(uint i = 0; i < homologos.size();i++){
        Point c1 = homologos[i];
        visorD->drawEllipse(QPoint(c1.x, c1.y), 1, 1, Qt::green, -1, 0);
    }
    for(uint i = 0; i < LCorner.size();i++){
        Point c1 = LCorner[i].coor;
        if(LCorner[i].homologo)
        visorS->drawEllipse(QPoint(c1.x, c1.y), 1, 1, Qt::green, -1, 0);
    }
}


void MainWindow::deselectWindow(){
    winSelected = false;
}


