#ifndef INRAREDTHREAD_H
#define INRAREDTHREAD_H

#include <QThread>

enum IR_KEY 
{
	IR_0 = 0x98,
	IR_1 = 0xA2,
	IR_2 = 0x62,
	IR_3 = 0xE2,
	IR_4 = 0x22,
	IR_5 = 0x02,
	IR_6 = 0xC2,
	IR_7 = 0xE0,
	IR_8 = 0xA8,
	IR_9 = 0x90,
	IR_up =  0x18,
	IR_down	 = 0x4A,
	IR_left = 0x10,
	IR_right = 0x5A,
	IR_ok = 0x38,
	IR_xi = 0x68,
	IR_ji = 0xB0
};

#define IS_RI_KEY(PERIPH) (((PERIPH) == IR_0) || \
							((PERIPH) == IR_1) || \
							((PERIPH) == IR_2) || \
							((PERIPH) == IR_3) || \
							((PERIPH) == IR_4) || \
							((PERIPH) == IR_5) || \
							((PERIPH) == IR_6) || \
							((PERIPH) == IR_7) || \
							((PERIPH) == IR_8) || \
							((PERIPH) == IR_9) || \
							((PERIPH) == IR_up) || \
							((PERIPH) == IR_down) || \
							((PERIPH) == IR_left) || \
							((PERIPH) == IR_right) || \
							((PERIPH) == IR_ok) || \
							((PERIPH) == IR_xi) || \
							((PERIPH) == IR_ji) )

class InraredThread : public QThread
{
    Q_OBJECT

public:
    explicit InraredThread();
    ~InraredThread();
    
protected:
    void run() override;   //重写QThread类的虚函数，也是线程子类的入口函数
signals:
    void  SendReceive(unsigned char data);
};

#endif // INRAREDTHREAD_H
