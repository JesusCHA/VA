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
#include <QtWidgets/QComboBox>
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
    QLabel *label;
    QComboBox *objetosComboBox;
    QPushButton *addObjButton;
    QPushButton *delObjButton;
    QScrollBar *horizontalScrollBar;

    void setupUi(QWidget *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(860, 307);
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
        label = new QLabel(MainWindow);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(740, 130, 91, 20));
        objetosComboBox = new QComboBox(MainWindow);
        objetosComboBox->setObjectName(QStringLiteral("objetosComboBox"));
        objetosComboBox->setGeometry(QRect(740, 160, 101, 27));
        addObjButton = new QPushButton(MainWindow);
        addObjButton->setObjectName(QStringLiteral("addObjButton"));
        addObjButton->setGeometry(QRect(720, 220, 131, 27));
        delObjButton = new QPushButton(MainWindow);
        delObjButton->setObjectName(QStringLiteral("delObjButton"));
        delObjButton->setGeometry(QRect(720, 250, 131, 27));
        horizontalScrollBar = new QScrollBar(MainWindow);
        horizontalScrollBar->setObjectName(QStringLiteral("horizontalScrollBar"));
        horizontalScrollBar->setGeometry(QRect(470, 270, 160, 16));
        horizontalScrollBar->setOrientation(Qt::Horizontal);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QWidget *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Proyecto de Visi\303\263n Artificial", 0));
        captureButton->setText(QApplication::translate("MainWindow", "Stop Capture", 0));
        colorButton->setText(QApplication::translate("MainWindow", "Color Image", 0));
        label->setText(QApplication::translate("MainWindow", "Select Object", 0));
        objetosComboBox->clear();
        objetosComboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Object 1", 0)
         << QApplication::translate("MainWindow", "Object 2", 0)
         << QApplication::translate("MainWindow", "Object 3", 0)
        );
        addObjButton->setText(QApplication::translate("MainWindow", "Add Object Image", 0));
        delObjButton->setText(QApplication::translate("MainWindow", "Del Object Image", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
