#ifndef __MOTOR_H
#define __MOTOR_H

#include <sys.h>	 
#include "pid.h"
#include "control.h"

extern int    motorLeft,motorRight;                      //电机PWM变量
//速度
extern int    leftSpeedNow; 
extern int    rightSpeedNow; 
//乘以1000之后的速度设定值
extern int    leftSpeedSet; 
extern int    rightSpeedSet; 
extern u8 Flag_Qian,Flag_Hou,Flag_Left,Flag_Right,Flag_sudu; //蓝牙遥控相关的变量


#define PWMA   TIM1->CCR4    //左
#define AIN2   PBout(12)
#define AIN1   PBout(13)


#define BIN1   PBout(15)     //左
#define BIN2   PBout(14)
#define PWMB   TIM1->CCR1    //PA11

void Set_Pwm(int moto1,int moto2);
u8 Turn_Off(int voltage);
void App_main(void);
int myabs(int a);
void USART3_IRQHandler(void);

#endif

