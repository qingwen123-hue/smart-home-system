#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDebug>
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>
#include <QRect>
#include <QPushButton>
#include <QFile>
#include "inraredthread.h"
#include <QTime>
#include "uart.h"

extern int gTemp,gHumi,gIndoorTem,gWeatherCode,gHour,gMin;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //重写定时器的事件   虚函数 子类重写父类的虚函数
    virtual void timerEvent(QTimerEvent *);

    int id_1,id_2,id_3;
    InraredThread *Inrared;
    uart *mUart;
private slots:
    void on_LEDButton_clicked();

    void on_FANButton_clicked();
    void ReceiveInrared(unsigned char data);
    void ReceiveUart(unsigned char data);
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
