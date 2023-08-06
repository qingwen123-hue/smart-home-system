#ifndef UART_H
#define UART_H

#include <QThread>

class uart : public QThread
{
    Q_OBJECT
public:
    uart();
    ~uart();
protected:
    void run() override;   //重写QThread类的虚函数，也是线程子类的入口函数
signals:
    void  SendReceive(unsigned char data);
};

#endif // UART_H
