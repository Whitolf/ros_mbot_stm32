#ifndef __MOTOR_H
#define __MOTOR_H

#include <sys.h>	 
#include "pid.h"
#include "control.h"

extern int    motorLeft,motorRight;                      //���PWM����
//�ٶ�
extern int    leftSpeedNow; 
extern int    rightSpeedNow; 
//����1000֮����ٶ��趨ֵ
extern int    leftSpeedSet; 
extern int    rightSpeedSet; 
extern u8 Flag_Qian,Flag_Hou,Flag_Left,Flag_Right,Flag_sudu; //����ң����صı���


#define PWMA   TIM1->CCR4    //��
#define AIN2   PBout(12)
#define AIN1   PBout(13)


#define BIN1   PBout(15)     //��
#define BIN2   PBout(14)
#define PWMB   TIM1->CCR1    //PA11

void Set_Pwm(int moto1,int moto2);
u8 Turn_Off(int voltage);
void App_main(void);
int myabs(int a);
void USART3_IRQHandler(void);

#endif

