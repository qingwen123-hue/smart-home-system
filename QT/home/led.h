#ifndef LED_H
#define LED_H

#define LEDOFF 				48			/* 关灯 */
#define LEDON 				49			/* 开灯 */
//void LED_Init();
void LED_ON();
void LED_OFF();
unsigned char Led_GetState();

#endif // LED_H
