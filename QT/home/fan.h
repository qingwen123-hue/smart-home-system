#ifndef FAN_H
#define FAN_H

#define FANOFF 				48			/* 关灯 */
#define FANON 				49			/* 开灯 */

void FAN_ON();
void FAN_OFF();
unsigned char FAN_GetState();


#endif // FAN_H
