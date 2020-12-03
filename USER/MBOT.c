#include "sys.h"

//====================自己加入的头文件===============================
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
程序功能：ROS小车底层代码（全部）
程序编写：公众号：小白学移动机器人
其他    ：如果对代码有任何疑问，可以私信小编，一定会回复的。
=====================================================================
------------------关注公众号，获得更多有趣的分享---------------------
===================================================================*/
int main(void)
{ 
	//发送计数
	char sendCount=0;
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//禁用JTAG 启用 SWD
	
	MY_NVIC_PriorityGroupConfig(2);	//=====设置中断分组
	
	delay_init();	    	        //=====延时函数初始化
	
	LED_Init();                     //=====LED初始化    程序灯	
	
	usart1_init(115200);	        //=====串口1初始化  树莓派
	usart3_init(9600);              //=====串口3初始化  蓝牙

	IIC_Init();                     //=====IIC初始化    读取MPU6050数据
	MPU6050_initialize();           //=====MPU6050初始化	
	DMP_Init();                     //=====初始化DMP 

	Encoder_Init_TIM2();            //=====初始化编码器1接口
	Encoder_Init_TIM4();            //=====初始化编码器2接口
	
	MY_ADC_Init();                  //=====adc初始化    电池电量检测
	
	Motor_Init(7199,0);             //=====初始化PWM 10KHZ，用于驱动电机 如需初始化驱动器接口
	PID_Init();                     //=====PID初始化
	
	MBOT_EXTI_Init();               //=====MPU6050 5ms定时中断初始化

	while(1)
	{
		//给树莓派发送速度，角度,这里速度已经乘以1000
		if(sendCount==0)//发送  14.4ms  发送一次数据 70Hz 左右
		{
			//发送需要一定的延时
			usartSendData(USART1,(short)leftSpeedNow,(short)rightSpeedNow,(short)yaw,sendCtrlFlag);  //1ms
			//蓝牙调试时用，不调试注释
			pcShow();                                                           //2.2ms单个float数据
			sendCount++;
		}
		else
		{
			sendCount++;
			if(sendCount==25)
				sendCount=0;
		}
		//获取角度		
		getAngle(&yaw,&yaw_acc_error);                                                      
	} 
}

//中断服务函数
void USART1_IRQHandler()
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
 	 {
		 USART_ClearITPendingBit(USART1,USART_IT_RXNE);//首先清除中断标志位
		 usartReceiveOneData(USART1,&leftSpeedSet,&rightSpeedSet,&receCtrlFlag);
		 leftSpeedSet*=0.4;
		 rightSpeedSet*=0.4;
		 
	 }
}

