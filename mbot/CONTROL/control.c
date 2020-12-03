#include "control.h"	
#include "pid.h"
#include "motor.h"
#include "adc.h"
#include "encoder.h"
#include "led.h"

int   Voltage          =0;           //��ص�ѹ������صı���
float yaw              =0;           //ת��������
float yaw_acc_error    =0;           //yaw�ۻ����
#define FIVE_MS_ERROR   0.00002115   //yawÿ5ms������Ư�ƵĶ���������������ԣ�����������Сʱƫ1�ȣ�ÿ���˵����ֵ����������ͬ���������м���


/**************************************************************************
�������ܣ����еĿ��ƴ��붼��������
          5ms��ʱ�ж���MPU6050��INT���Ŵ���		
**************************************************************************/
void EXTI9_5_IRQHandler(void) 
{                                                         
	EXTI_ClearITPendingBit(EXTI_Line5);                            //===���LINE12��·����λ		
	
	Led_Flash(200);                                                 //===LED��˸��֤��������������	
	
	App_main();
	
	yaw_acc_error += FIVE_MS_ERROR;								    //===yawƯ������ۼ�
	
	Get_battery_volt_average(&Voltage,100);		                    //===��ص�ѹ��������λmv��100��ȡһ��ƽ��
	
	Get_Motor_Speed(&leftSpeedNow,&rightSpeedNow);                  //===��ȡ����������ʵ�ٶ�
	
	pid_Task_Letf.speedSet  = leftSpeedSet;	                        //===���ٶ��趨ֵ��ʵʱֵ��ֵ
	pid_Task_Right.speedSet = rightSpeedSet;
	pid_Task_Letf.speedNow  = leftSpeedNow;
	pid_Task_Right.speedNow = rightSpeedNow;
	
	Pid_Ctrl(&motorLeft,&motorRight);                               //===ִ��PID���ƺ���

	if(Turn_Off(Voltage)==0)                                        //===�����ѹ�쳣
	{
		Set_Pwm(motorLeft,motorRight);                              //===��ֵ��PWM�Ĵ��� 		
	} 	
} 








