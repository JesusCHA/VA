/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDial>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QFrame *imageFrameS;
    QFrame *imageFrameD;
    QPushButton *captureButton;
    QPushButton *colorButton;
    QPushButton *loadFileButton;
    QPushButton *saveFileButton;
    QPushButton *copyButton;
    QPushButton *resizeButton;
    QPushButton *enlargeButton;
    QDial *angleDial;
    QScrollBar *horizontalTranslation;
    QScrollBar *verticalTranslation;
    QScrollBar *zoomTranslation;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *warpZoomButton;

    void setupUi(QWidget *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(860, 355);
        imageFrameS = new QFrame(MainWindow);
        imageFrameS->setObjectName(QStringLiteral("imageFrameS"));
        imageFrameS->setGeometry(QRect(20, 20, 320, 240));
        imageFrameS->setFrameShape(QFrame::StyledPanel);
        imageFrameS->setFrameShadow(QFrame::Raised);
        imageFrameD = new QFrame(MainWindow);
        imageFrameD->setObjectName(QStringLiteral("imageFrameD"));
        imageFrameD->setGeometry(QRect(390, 20, 320, 240));
        imageFrameD->setFrameShape(QFrame::StyledPanel);
        imageFrameD->setFrameShadow(QFrame::Raised);
        captureButton = new QPushButton(MainWindow);
        captureButton->setObjectName(QStringLiteral("captureButton"));
        captureButton->setGeometry(QRect(740, 20, 101, 31));
        captureButton->setCheckable(true);
        captureButton->setChecked(true);
        colorButton = new QPushButton(MainWindow);
        colorButton->setObjectName(QStringLiteral("colorButton"));
        colorButton->setGeometry(QRect(740, 60, 101, 31));
        colorButton->setCheckable(true);
        colorButton->setChecked(false);
        loadFileButton = new QPushButton(MainWindow);
        loadFileButton->setObjectName(QStringLiteral("loadFileButton"));
        loadFileButton->setGeometry(QRect(740, 100, 101, 31));
        saveFileButton = new QPushButton(MainWindow);
        saveFileButton->setObjectName(QStringLiteral("saveFileButton"));
        saveFileButton->setGeometry(QRect(740, 140, 101, 31));
        copyButton = new QPushButton(MainWindow);
        copyButton->setObjectName(QStringLiteral("copyButton"));
        copyButton->setGeometry(QRect(740, 180, 101, 31));
        resizeButton = new QPushButton(MainWindow);
        resizeButton->setObjectName(QStringLiteral("resizeButton"));
        resizeButton->setGeometry(QRect(740, 220, 101, 31));
        enlargeButton = new QPushButton(MainWindow);
        enlargeButton->setObjectName(QStringLiteral("enlargeButton"));
        enlargeButton->setGeometry(QRect(740, 260, 101, 31));
        angleDial = new QDial(MainWindow);
        angleDial->setObjectName(QStringLiteral("angleDial"));
        angleDial->setGeometry(QRect(30, 290, 51, 51));
        angleDial->setMaximum(359);
        horizontalTranslation = new QScrollBar(MainWindow);
        horizontalTranslation->setObjectName(QStringLiteral("horizontalTranslation"));
        horizontalTranslation->setGeometry(QRect(130, 310, 160, 16));
        horizontalTranslation->setMinimum(-320);
        horizontalTranslation->setMaximum(320);
        horizontalTranslation->setOrientation(Qt::Horizontal);
        verticalTranslation = new QScrollBar(MainWindow);
        verticalTranslation->setObjectName(QStringLiteral("verticalTranslation"));
        verticalTranslation->setGeometry(QRect(360, 310, 160, 16));
        verticalTranslation->setMinimum(-240);
        verticalTranslation->setMaximum(240);
        verticalTranslation->setOrientation(Qt::Horizontal);
        zoomTranslation = new QScrollBar(MainWindow);
        zoomTranslation->setObjectName(QStringLiteral("zoomTranslation"));
        zoomTranslation->setGeometry(QRect(569, 310, 111, 16));
        zoomTranslation->setMinimum(1);
        zoomTranslation->setMaximum(10);
        zoomTranslation->setSingleStep(1);
        zoomTranslation->setValue(1);
        zoomTranslation->setSliderPosition(1);
        zoomTranslation->setOrientation(Qt::Horizontal);
        label = new QLabel(MainWindow);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(140, 280, 141, 20));
        label_2 = new QLabel(MainWindow);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(40, 270, 58, 14));
        label_3 = new QLabel(MainWindow);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(370, 280, 141, 20));
        label_4 = new QLabel(MainWindow);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(600, 280, 51, 20));
        warpZoomButton = new QPushButton(MainWindow);
        warpZoomButton->setObjectName(QStringLiteral("warpZoomButton"));
        warpZoomButton->setGeometry(QRect(740, 300, 101, 31));
        warpZoomButton->setMouseTracking(false);
        warpZoomButton->setCheckable(true);
        warpZoomButton->setChecked(false);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QWidget *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Proyecto de Visi\303\263n Artificial", 0));
        captureButton->setText(QApplication::translate("MainWindow", "Stop Capture", 0));
        colorButton->setText(QApplication::translate("MainWindow", "Color Image", 0));
        loadFileButton->setText(QApplication::translate("MainWindow", "Load from file", 0));
        saveFileButton->setText(QApplication::translate("MainWindow", "Save to file", 0));
        copyButton->setText(QApplication::translate("MainWindow", "Copy", 0));
        resizeButton->setText(QApplication::translate("MainWindow", "Resize", 0));
        enlargeButton->setText(QApplication::translate("MainWindow", "Enlarge", 0));
        label->setText(QApplication::translate("MainWindow", "Horizontal translations", 0));
        label_2->setText(QApplication::translate("MainWindow", "Angle", 0));
        label_3->setText(QApplication::translate("MainWindow", "Vertical translations", 0));
        label_4->setText(QApplication::translate("MainWindow", "Zoom", 0));
        warpZoomButton->setText(QApplication::translate("MainWindow", "Warp + Zoom", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
