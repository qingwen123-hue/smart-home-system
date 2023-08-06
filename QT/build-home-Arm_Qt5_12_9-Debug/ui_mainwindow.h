/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *LEDButton;
    QPushButton *FANButton;
    QLabel *label;
    QLineEdit *lsensEdit;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *temperatureEdit;
    QLineEdit *humidityEdit;
    QLabel *label_4;
    QLabel *label_5;
    QLineEdit *weatherEdit;
    QLineEdit *IndoortemEdit;
    QLabel *label_curtime;
    QLabel *weather_img;
    QLabel *label_6;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 480);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        LEDButton = new QPushButton(centralwidget);
        LEDButton->setObjectName(QString::fromUtf8("LEDButton"));
        LEDButton->setGeometry(QRect(590, 290, 121, 51));
        FANButton = new QPushButton(centralwidget);
        FANButton->setObjectName(QString::fromUtf8("FANButton"));
        FANButton->setGeometry(QRect(590, 200, 121, 51));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 190, 91, 51));
        lsensEdit = new QLineEdit(centralwidget);
        lsensEdit->setObjectName(QString::fromUtf8("lsensEdit"));
        lsensEdit->setGeometry(QRect(120, 200, 121, 31));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(30, 240, 81, 41));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(30, 290, 81, 41));
        temperatureEdit = new QLineEdit(centralwidget);
        temperatureEdit->setObjectName(QString::fromUtf8("temperatureEdit"));
        temperatureEdit->setGeometry(QRect(120, 250, 121, 31));
        humidityEdit = new QLineEdit(centralwidget);
        humidityEdit->setObjectName(QString::fromUtf8("humidityEdit"));
        humidityEdit->setGeometry(QRect(120, 300, 121, 31));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(300, 200, 81, 31));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(290, 250, 81, 41));
        weatherEdit = new QLineEdit(centralwidget);
        weatherEdit->setObjectName(QString::fromUtf8("weatherEdit"));
        weatherEdit->setGeometry(QRect(360, 200, 131, 31));
        IndoortemEdit = new QLineEdit(centralwidget);
        IndoortemEdit->setObjectName(QString::fromUtf8("IndoortemEdit"));
        IndoortemEdit->setGeometry(QRect(360, 250, 131, 31));
        label_curtime = new QLabel(centralwidget);
        label_curtime->setObjectName(QString::fromUtf8("label_curtime"));
        label_curtime->setGeometry(QRect(550, 130, 301, 61));
        QFont font;
        font.setPointSize(24);
        label_curtime->setFont(font);
        weather_img = new QLabel(centralwidget);
        weather_img->setObjectName(QString::fromUtf8("weather_img"));
        weather_img->setGeometry(QRect(570, 20, 96, 96));
        label_6 = new QLabel(centralwidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(150, 50, 331, 71));
        QFont font1;
        font1.setPointSize(36);
        label_6->setFont(font1);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 28));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        LEDButton->setText(QString());
        FANButton->setText(QString());
        label->setText(QApplication::translate("MainWindow", "\345\256\244\345\206\205\345\205\211\345\274\272:", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "\345\256\244\345\206\205\346\270\251\345\272\246:", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "\345\256\244\345\206\205\346\271\277\345\272\246:", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "\345\244\251\346\260\224:", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "\345\256\244\345\244\226\346\270\251\345\272\246:", nullptr));
        label_curtime->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        weather_img->setText(QString());
        label_6->setText(QApplication::translate("MainWindow", "\346\231\272\350\203\275\345\256\266\345\261\205\347\263\273\347\273\237", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
