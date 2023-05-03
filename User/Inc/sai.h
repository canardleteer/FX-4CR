#ifndef __SAI_H
#define __SAI_H
#include "sys.h"    									
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK STM32H7������
//SAI ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2018/8/15
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2014-2024
//All rights reserved	
//********************************************************************************
//����˵��
//V1.1 20180817
//1,����SAIB_Init��SAIA_RX_DMA_Init��SAI_Rec_Start��SAI_Rec_Stop�Ⱥ���	
//2,����:sai_ltdc_clkset����,��������LTDCʱ��,ȷ�����޸�pll3n��ʱ��,���ı�LTDC������ʱ��
////////////////////////////////////////////////////////////////////////////////// 	
 
extern void (*sai_tx_callback)(void);		//sai tx�ص�����ָ��  
extern void (*sai_rx_callback)(void);		//sai rx�ص�����

void SAIA_Init(u8 mode,u8 cpol,u8 datalen);
void SAIB_Init(u8 mode,u8 cpol,u8 datalen);
u8 SAIA_SampleRate_Set(u32 samplerate);
void SAIA_TX_DMA_Init(u32* buf0,u32* buf1,u16 num,u8 width);
void SAIB_RX_DMA_Init(u32* buf0,u32* buf1,u16 num,u8 width);
void SAI_Play_Start(void); 
void SAI_Play_Stop(void);  
void SAI_Rec_Start(void);
void SAI_Rec_Stop(void);
#endif



















