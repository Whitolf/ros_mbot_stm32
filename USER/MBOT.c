#include "sys.h"

//====================�Լ������ͷ�ļ�===============================
#include "delay.h"
#include "led.h"
#include "myexti.h"
#include "adc.h"
#include "pwm.h"
#include "encoder.h"
#include "usart1.h"	
#include "usart3.h"
#include "ioi2c.h"
#include "mpu6050.h"

#include "show.h"					
#include "mbotLinuxUsart.h"
#include "pid.h"
#include "control.h"
#include "motor.h"

/*===================================================================
�����ܣ�ROSС���ײ���루ȫ����
�����д�����ںţ�С��ѧ�ƶ�������
����    ������Դ������κ����ʣ�����˽��С�࣬һ����ظ��ġ�
=====================================================================
------------------��ע���ںţ���ø�����Ȥ�ķ���---------------------
===================================================================*/
int main(void)
{ 
	//���ͼ���
	char sendCount=0;
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//����JTAG ���� SWD
	
	MY_NVIC_PriorityGroupConfig(2);	//=====�����жϷ���
	
	delay_init();	    	        //=====��ʱ������ʼ��
	
	LED_Init();                     //=====LED��ʼ��    �����	
	
	usart1_init(115200);	        //=====����1��ʼ��  ��ݮ��
	usart3_init(9600);              //=====����3��ʼ��  ����

	IIC_Init();                     //=====IIC��ʼ��    ��ȡMPU6050����
	MPU6050_initialize();           //=====MPU6050��ʼ��	
	DMP_Init();                     //=====��ʼ��DMP 

	Encoder_Init_TIM2();            //=====��ʼ��������1�ӿ�
	Encoder_Init_TIM4();            //=====��ʼ��������2�ӿ�
	
	MY_ADC_Init();                  //=====adc��ʼ��    ��ص������
	
	Motor_Init(7199,0);             //=====��ʼ��PWM 10KHZ������������� �����ʼ���������ӿ�
	PID_Init();                     //=====PID��ʼ��
	
	MBOT_EXTI_Init();               //=====MPU6050 5ms��ʱ�жϳ�ʼ��

	while(1)
	{
		//����ݮ�ɷ����ٶȣ��Ƕ�,�����ٶ��Ѿ�����1000
		if(sendCount==0)//����  14.4ms  ����һ������ 70Hz ����
		{
			//������Ҫһ������ʱ
			usartSendData(USART1,(short)leftSpeedNow,(short)rightSpeedNow,(short)yaw,sendCtrlFlag);  //1ms
			//��������ʱ�ã�������ע��
			pcShow();                                                           //2.2ms����float����
			sendCount++;
		}
		else
		{
			sendCount++;
			if(sendCount==25)
				sendCount=0;
		}
		//��ȡ�Ƕ�		
		getAngle(&yaw,&yaw_acc_error);                                                      
	} 
}

//�жϷ�����
void USART1_IRQHandler()
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
 	 {
		 USART_ClearITPendingBit(USART1,USART_IT_RXNE);//��������жϱ�־λ
		 usartReceiveOneData(USART1,&leftSpeedSet,&rightSpeedSet,&receCtrlFlag);
		 leftSpeedSet*=0.4;
		 rightSpeedSet*=0.4;
		 
	 }
}

