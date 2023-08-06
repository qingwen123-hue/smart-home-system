#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "led.h"
#include "fan.h"
#include "photosensitive.h"
#include "dht11.h"
#include "weather.h"
#include "inraredthread.h"
#include <QImage>
int gTemp,gHumi,gIndoorTem,gWeatherCode,gHour,gMin;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QList <QScreen *> list_screen = QGuiApplication::screens();
#if __arm__
    /* 重设大小 */
    this->resize(list_screen.at(0)->geometry().width(),list_screen.at(0)->geometry().height());
#else
    /* 否则则设置主窗体大小为 800x480 */
    this->resize(800, 480);
#endif

    LED_OFF();
    ui->LEDButton->setText("开灯");
    FAN_OFF();
    ui->FANButton->setText("开风扇");

    int lsens = ReadLight();
    ui->lsensEdit->setText(QString::number(lsens));

    //int temp,humi;
    DHT11_getValue(gTemp,gHumi);
    ui->temperatureEdit->setText(QString::number(gTemp));
    ui->humidityEdit->setText(QString::number(gHumi));

    //int ret = WeatherSocketInit();
    //if(ret < 0)qDebug() << "Weather Socket Fail";

    id_1 = startTimer(2000);

    //int IndoorTem,WeatherCode;
    GetWeather(gIndoorTem,gWeatherCode);
    ui->IndoortemEdit->setText(QString::number(gIndoorTem));
    ui->weatherEdit->setText(CodeToString(gWeatherCode));
    QImage* img = new QImage;
    img ->load("white/" + QString::number(gWeatherCode) + "@2x.png");
    ui->weather_img->setPixmap(QPixmap::fromImage(*img));
    delete img;
    id_2 = startTimer(1000 * 120);

    //ui->label_curtime->
    QTime current_time = QTime::currentTime();
    gHour = current_time.hour();
    gMin = current_time.minute();
    //QFont Font(QString::number(gHour) + ":" + QString::number(gMin), 16);
    //ui->label_curtime->setFont(Font);
    ui->label_curtime->setText(QString::number(gHour) + " 点 " + QString::number(gMin) + " 分");
    id_3 = startTimer(1000 * 55);
    
    Inrared = new InraredThread;
    //connect(Inrared, SIGNAL(SendReceive()), this, SLOT(ReceiveInrared()));
    connect(Inrared, &InraredThread::SendReceive, this, &MainWindow::ReceiveInrared);
    Inrared->start();

    mUart = new uart;
    connect(mUart, &uart::SendReceive, this, &MainWindow::ReceiveUart);
    mUart->start();
}

MainWindow::~MainWindow()
{
    Inrared->exit();
    mUart->exit();
    delete Inrared;
    delete mUart;
    delete ui;
}


void MainWindow::on_LEDButton_clicked()
{
    unsigned char s = Led_GetState();
    if(s == LEDON)
    {
        LED_OFF();
        ui->LEDButton->setText("开灯");
    }
    else if(s == LEDOFF)
    {
        LED_ON();
        ui->LEDButton->setText("关灯");
    }
}

void MainWindow::on_FANButton_clicked()
{
    unsigned char s = FAN_GetState();
    if(s == FANON)
    {
        FAN_OFF();
        ui->FANButton->setText("开风扇");
    }
    else if(s == FANOFF)
    {
        FAN_ON();
        ui->FANButton->setText("关风扇");
    }
}

void MainWindow::timerEvent(QTimerEvent * ev)
{
    if(ev->timerId() == id_1)
    {
        int lsens = ReadLight();
        ui->lsensEdit->setText(QString::number(lsens));

        //int temp,humi;
        DHT11_getValue(gTemp,gHumi);
        ui->temperatureEdit->setText(QString::number(gTemp));
        ui->humidityEdit->setText(QString::number(gHumi));


    }
    else if(ev->timerId() == id_2)
    {
        //int IndoorTem,WeatherCode;
        GetWeather(gIndoorTem,gWeatherCode);
        ui->IndoortemEdit->setText(QString::number(gIndoorTem));
        ui->weatherEdit->setText(CodeToString(gWeatherCode));
        QImage* img = new QImage;
        img ->load("white/" + QString::number(gWeatherCode) + "@2x.png");
        ui->weather_img->setPixmap(QPixmap::fromImage(*img));
        delete img;
//        qDebug() << "IndoorTem: " << IndoorTem;
//        qDebug() << "WeatherCode: " << WeatherCode;
    }
    else if(ev->timerId() == id_3)
    {
        QTime current_time = QTime::currentTime();
        gHour = current_time.hour();
        gMin = current_time.minute();
        //QFont Font(QString::number(gHour) + ":" + QString::number(gMin), 16);
        //ui->label_curtime->setFont(Font);
        ui->label_curtime->setText(QString::number(gHour) + " 点 " + QString::number(gMin) + " 分");
    }

}

void MainWindow::ReceiveInrared(unsigned char data)
{
    unsigned char s;
    switch (data)
    {
        case IR_1:
            s = Led_GetState();
            if(s == LEDON)
            {
                LED_OFF();
                ui->LEDButton->setText("开灯");
            }
            else if(s == LEDOFF)
            {
                LED_ON();
                ui->LEDButton->setText("关灯");
            }
            break;
        case IR_2:
            s = FAN_GetState();
            if(s == FANON)
            {
                FAN_OFF();
                ui->FANButton->setText("开风扇");
            }
            else if(s == FANOFF)
            {
                FAN_ON();
                ui->FANButton->setText("关风扇");
            }
            break;
        default:
            break;
    }
}

void MainWindow::ReceiveUart(unsigned char data)
{
    qDebug() << data;
    if(data == 0x00)
    {
        LED_ON();
        ui->LEDButton->setText("关灯");
    }
    else if(data == 0x01)
    {
        LED_OFF();
        ui->LEDButton->setText("开灯");
    }
    else if(data == 0x02)
    {
        FAN_ON();
        ui->FANButton->setText("关风扇");
    }
    else if(data == 0x03)
    {
        FAN_OFF();
        ui->FANButton->setText("开风扇");
    }
}
