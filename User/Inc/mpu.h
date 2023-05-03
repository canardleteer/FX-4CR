#ifndef __MPU_H
#define __MPU_H	 
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK STM32H7������
//MPU�ڴ汣�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2018/6/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//********************************************************************************
//����˵�� 
//��
//////////////////////////////////////////////////////////////////////////////////	 


//MPU����ϸ���ù�ϵ���뿴����STM32H7����ֲ�.pdf��
//����ĵ���6.6��,Table 91.
//MPU���������������Զ��壨������stm32h7xx_hal_cortex.h��
//����MPU->RASR�Ĵ���AP[26:24]λ������ֵ
#define  MPU_REGION_NO_ACCESS      	((uint8_t)0x00U)		//�޷��ʣ���Ȩ&�û������ɷ��ʣ�
#define  MPU_REGION_PRIV_RW        	((uint8_t)0x01U) 		//��֧����Ȩ��д����
#define  MPU_REGION_PRIV_RW_URO    	((uint8_t)0x02U)  		//��ֹ�û�д���ʣ���Ȩ�ɶ�д���ʣ�
#define  MPU_REGION_FULL_ACCESS    	((uint8_t)0x03U)  		//ȫ���ʣ���Ȩ&�û����ɷ��ʣ�
#define  MPU_REGION_PRIV_RO        	((uint8_t)0x05U) 		//��֧����Ȩ������
#define  MPU_REGION_PRIV_RO_URO    	((uint8_t)0x06U)		//ֻ������Ȩ&�û���������д��
 
void MPU_Disable(void);
void MPU_Enable(void);
u8 MPU_Convert_Bytes_To_POT(u32 nbytes);
u8 MPU_Set_Protection(u32 baseaddr,u32 size,u32 rnum,u8 ap,u8 sen,u8 cen,u8 ben);
void MPU_Memory_Protection(void);
#endif
















