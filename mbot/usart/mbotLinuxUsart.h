#ifndef __MBOTLINUXUSART__
#define __MBOTLINUXUSART__
#include "stm32f10x.h"	

extern unsigned char sendCtrlFlag;
extern unsigned char receCtrlFlag;

#define START   0X11

//��linux���ղ��������ݵ�������ַ��
extern int usartReceiveOneData(USART_TypeDef* USARTx,int *p_leftSpeedSet,int *p_rightSpeedSet,unsigned char *p_crtlFlag);   
//��װ���ݣ�����USART1_Send_String�����ݷ��͸�linux
extern void usartSendData(USART_TypeDef* USARTx,short leftVel, short rightVel,short angle,unsigned char ctrlFlag); 
//����ָ���ַ�����ĺ���
void USART_Send_String(USART_TypeDef* USARTx,unsigned char *p,unsigned short sendSize);     
//�����λѭ������У�飬�õ�У��ֵ��һ���̶�����֤���ݵ���ȷ��
unsigned char getCrc8(unsigned char *ptr, unsigned short len); 

#endif
