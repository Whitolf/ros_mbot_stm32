#ifndef __KEY_H
#define __KEY_H	 

//#include "stm32f10x.h"
#include "sys.h"
//#define KEY GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)
#define KEY PAin(5)

void KEY_Init(void);          //������ʼ��

u8 click_N_Double (u8 time);  //��������ɨ���˫������ɨ��
u8 click(void);               //��������ɨ��
u8 Long_Press(void);          //����ɨ��  

void key(void);

#endif  
