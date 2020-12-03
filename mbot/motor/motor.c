#include "motor.h"

//�����ֵ��PWM����
int motorLeft     = 0;
int motorRight    = 0;         

//����1000֮����ٶ�ʵʱֵ
int leftSpeedNow  = 0; 
int rightSpeedNow = 0; 

//����1000֮����ٶ��趨ֵ
int leftSpeedSet  = 0; 
int rightSpeedSet = 0; 

//����ң�����
u8  Flag_Qian     = 0;
u8  Flag_Hou      = 0;
u8  Flag_Left     = 0;
u8  Flag_Right    = 0;
u8  Flag_sudu     = 2;
u8  Flag_useApp   = 0;      


/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ���������PWM������PWM
����  ֵ����
**************************************************************************/
void Set_Pwm(int motorLeft,int motorRight)
{
	if(motorLeft>0){
		AIN2=0, AIN1=1;
	}else{
	AIN2=1,	AIN1=0;
	}
	PWMA=myabs(motorLeft);
	if(motorRight>0){
		BIN1=0,	BIN2=1;}
	else{
	BIN1=1,	BIN2=0;
	}
	PWMB=myabs(motorRight);	
}

/**************************************************************************
�������ܣ��쳣�رյ��
��ڲ�������ѹ
����  ֵ��1���쳣  0������
**************************************************************************/
u8 Turn_Off(int voltage)
{
	u8 temp =0;
	if(voltage<1110)//��ص�ѹ����11.1V�رյ��
	{	                                             
		temp=1;
		motorLeft=0;
		motorRight=0;		
		AIN1=0;                                            
		AIN2=0;
		BIN1=0;
		BIN2=0;
	}
	else
	{
		temp=0;
	}
	return temp;			
}


/**************************************************************************
��������:APPָ���ж�
��ڲ���:
�� �� ֵ:
Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0
**************************************************************************/
void App_main()
{
	if(Flag_useApp==1)
	{
		if(Flag_Qian==1)
		{
			if(Flag_sudu==2)//Ĭ�ϵ���
			{
				leftSpeedSet  = 120; 
				rightSpeedSet = 120; 			
			}
			else if(Flag_sudu==1)//����
			{
				leftSpeedSet  = 350; 
				rightSpeedSet = 350; 			
			}		
		}
		else if(Flag_Hou==1)
		{

			if(Flag_sudu==2)//Ĭ�ϵ���
			{
				leftSpeedSet  = -120; 
				rightSpeedSet = -120; 			
			}
			else if(Flag_sudu==1)//����
			{
				leftSpeedSet  = -350; 
				rightSpeedSet = -350; 				
			}	
		}
		else if(Flag_Left==1)
		{

			if(Flag_sudu==2||Flag_sudu==1)//Ĭ�ϵ���
			{
				leftSpeedSet  = -100; 
				rightSpeedSet = 100;			
			}
		}
		else if(Flag_Right==1)
		{

			if(Flag_sudu==2||Flag_sudu==1)//Ĭ�ϵ���
			{
				leftSpeedSet  = 100; 
				rightSpeedSet = -100;			
			}
		}
		else
		{
			leftSpeedSet  = 0; 
			rightSpeedSet = 0; 
		}	
	}
}


/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����int
����  ֵ��unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	int temp;
	if(a<0)  
	  temp=-a;  
	else 
	  temp=a;
	return temp;
}

/**************************************************************************
�������ܣ�����3�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/
void USART3_IRQHandler(void)
{	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //���յ�����
	{	  
	 static	int uart_receive=0;//����������ر���
	 uart_receive=USART_ReceiveData(USART3); 
			if(uart_receive==0x4A)//????
		{
			if(Flag_sudu==2) //???(???)
				Flag_sudu=1;   //???
			else
				Flag_sudu=2;  
		}	
		
		if(uart_receive==0x4B)
		{
			if(Flag_useApp==0)//?????APP
				Flag_useApp=1;
			else
				Flag_useApp=0;
		}
		
	  if(uart_receive>10)  //Ĭ��ʹ��appΪ��MiniBalanceV3.5 ��ΪMiniBalanceV3.5��ң��ָ��ΪA~H ��HEX������10
    {			
			if(uart_receive==0x5A)	Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////ɲ��
			else if(uart_receive==0x41)	Flag_Qian=1,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////ǰ
			else if(uart_receive==0x45)	Flag_Qian=0,Flag_Hou=1,Flag_Left=0,Flag_Right=0;//////////////��
			else if(uart_receive==0x42||uart_receive==0x43||uart_receive==0x44)	
														Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=1;  //��
			else if(uart_receive==0x46||uart_receive==0x47||uart_receive==0x48)	    //��
														Flag_Qian=0,Flag_Hou=0,Flag_Left=1,Flag_Right=0;
			else Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////ɲ��
  	}
		if(uart_receive<10)     //����appΪ��MiniBalanceV1.0  ��ΪMiniBalanceV1.0��ң��ָ��ΪA~H ��HEX��С��10
		{			
			Flag_sudu=1;//�л������ٵ�
			if(uart_receive==0x00)	Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////ɲ��
			else if(uart_receive==0x06)	Flag_Qian=1,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////ǰ
			else if(uart_receive==0x05)	Flag_Qian=0,Flag_Hou=1,Flag_Left=0,Flag_Right=0;//////////////��
			else if(uart_receive==0x02||uart_receive==0x03||uart_receive==0x04)	
														Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=1;  //��
			else if(uart_receive==0x06||uart_receive==0x07||uart_receive==0x08)	    //��
														Flag_Qian=0,Flag_Hou=0,Flag_Left=1,Flag_Right=0;
			else Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////ɲ��
  	}	
	}  											 
} 



