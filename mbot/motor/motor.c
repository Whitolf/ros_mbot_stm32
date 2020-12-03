#include "motor.h"

//左右轮电机PWM变量
int motorLeft     = 0;
int motorRight    = 0;         

//乘以1000之后的速度实时值
int leftSpeedNow  = 0; 
int rightSpeedNow = 0; 

//乘以1000之后的速度设定值
int leftSpeedSet  = 0; 
int rightSpeedSet = 0; 

//蓝牙遥控相关
u8  Flag_Qian     = 0;
u8  Flag_Hou      = 0;
u8  Flag_Left     = 0;
u8  Flag_Right    = 0;
u8  Flag_sudu     = 2;
u8  Flag_useApp   = 0;      


/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：左轮PWM、右轮PWM
返回  值：无
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
函数功能：异常关闭电机
入口参数：电压
返回  值：1：异常  0：正常
**************************************************************************/
u8 Turn_Off(int voltage)
{
	u8 temp =0;
	if(voltage<1110)//电池电压低于11.1V关闭电机
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
函数功能:APP指令判断
入口参数:
返 回 值:
Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0
**************************************************************************/
void App_main()
{
	if(Flag_useApp==1)
	{
		if(Flag_Qian==1)
		{
			if(Flag_sudu==2)//默认低速
			{
				leftSpeedSet  = 120; 
				rightSpeedSet = 120; 			
			}
			else if(Flag_sudu==1)//高速
			{
				leftSpeedSet  = 350; 
				rightSpeedSet = 350; 			
			}		
		}
		else if(Flag_Hou==1)
		{

			if(Flag_sudu==2)//默认低速
			{
				leftSpeedSet  = -120; 
				rightSpeedSet = -120; 			
			}
			else if(Flag_sudu==1)//高速
			{
				leftSpeedSet  = -350; 
				rightSpeedSet = -350; 				
			}	
		}
		else if(Flag_Left==1)
		{

			if(Flag_sudu==2||Flag_sudu==1)//默认低速
			{
				leftSpeedSet  = -100; 
				rightSpeedSet = 100;			
			}
		}
		else if(Flag_Right==1)
		{

			if(Flag_sudu==2||Flag_sudu==1)//默认低速
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
函数功能：绝对值函数
入口参数：int
返回  值：unsigned int
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
函数功能：串口3接收中断
入口参数：无
返回  值：无
**************************************************************************/
void USART3_IRQHandler(void)
{	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //接收到数据
	{	  
	 static	int uart_receive=0;//蓝牙接收相关变量
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
		
	  if(uart_receive>10)  //默认使用app为：MiniBalanceV3.5 因为MiniBalanceV3.5的遥控指令为A~H 其HEX都大于10
    {			
			if(uart_receive==0x5A)	Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////刹车
			else if(uart_receive==0x41)	Flag_Qian=1,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////前
			else if(uart_receive==0x45)	Flag_Qian=0,Flag_Hou=1,Flag_Left=0,Flag_Right=0;//////////////后
			else if(uart_receive==0x42||uart_receive==0x43||uart_receive==0x44)	
														Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=1;  //左
			else if(uart_receive==0x46||uart_receive==0x47||uart_receive==0x48)	    //右
														Flag_Qian=0,Flag_Hou=0,Flag_Left=1,Flag_Right=0;
			else Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////刹车
  	}
		if(uart_receive<10)     //备用app为：MiniBalanceV1.0  因为MiniBalanceV1.0的遥控指令为A~H 其HEX都小于10
		{			
			Flag_sudu=1;//切换至高速档
			if(uart_receive==0x00)	Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////刹车
			else if(uart_receive==0x06)	Flag_Qian=1,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////前
			else if(uart_receive==0x05)	Flag_Qian=0,Flag_Hou=1,Flag_Left=0,Flag_Right=0;//////////////后
			else if(uart_receive==0x02||uart_receive==0x03||uart_receive==0x04)	
														Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=1;  //左
			else if(uart_receive==0x06||uart_receive==0x07||uart_receive==0x08)	    //右
														Flag_Qian=0,Flag_Hou=0,Flag_Left=1,Flag_Right=0;
			else Flag_Qian=0,Flag_Hou=0,Flag_Left=0,Flag_Right=0;//////////////刹车
  	}	
	}  											 
} 



