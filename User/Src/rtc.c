#include "rtc.h"
#include "dwt.h"
#include "led.h"
#include "lcd.h"
#include "ui_Driver.h"
//#include "delay.h"
//#include "usart.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK STM32H7������
//RTC ��������	    
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2018/6/10
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2014-2024
//All rights reserved	
//********************************************************************************
//�޸�˵��
//��
////////////////////////////////////////////////////////////////////////////////// 	 

//�ȴ�RSFͬ��
//����ֵ:0,�ɹ�;1,ʧ��;
u8 RTC_Wait_Synchro(void)
{ 
	u32 retry=0XFFFFF; 
	//�ر�RTC�Ĵ���д����
	RTC->WPR=0xCA;
	RTC->WPR=0x53; 
	RTC->ISR&=~(1<<5);		//���RSFλ 
	while(retry&&((RTC->ISR&(1<<5))==0x00))retry--;//�ȴ�Ӱ�ӼĴ���ͬ�� 
    if(retry==0)return 1;	//ͬ��ʧ�� 
	RTC->WPR=0xFF;			//ʹ��RTC�Ĵ���д����  
	return 0;
}
//RTC�����ʼ��ģʽ
//����ֵ:0,�ɹ�;1,ʧ��;
u8 RTC_Init_Mode(void)
{ 
	u32 retry=0XFFFFF; 
	if(RTC->ISR&(1<<6))return 0; 
	RTC->ISR|=1<<7;			//����RTC��ʼ��ģʽ
	while(retry&&((RTC->ISR&(1<<6))==0x00))retry--;//�ȴ�����RTC��ʼ��ģʽ�ɹ� 
    if(retry==0)return 1;	//ͬ��ʧ��
	else return 0; 			//ͬ���ɹ� 
}
//RTCд�������SRAM
//BKPx:�����Ĵ������,��Χ:0~31
//data:Ҫд�������,32λ����
void RTC_Write_BKR(u32 BKRx,u32 data)
{
	u32 temp=0;  
	temp=RTC_BASE+0x50+BKRx*4;   
	(*(u32*)temp)=data; 
}
//RTC��ȡ������SRAM
//BKPx:�����Ĵ������,��Χ:0~19
//����ֵ:��ȡ��������
u32 RTC_Read_BKR(u32 BKRx)
{
	u32 temp=0; 
	temp=RTC_BASE+0x50+BKRx*4;  
	return (*(u32*)temp);		//���ض�ȡ����ֵ
}

//ʮ����ת��ΪBCD��
//val:Ҫת����ʮ������
//����ֵ:BCD��
u8 RTC_DEC2BCD(u8 val)
{
	u8 bcdhigh = 0; 
	while(val>=10)
	{
		bcdhigh++;
		val-=10;
	} 
	return ((u8)(bcdhigh<<4)|val);
}
//BCD��ת��Ϊʮ��������
//val:Ҫת����BCD��
//����ֵ:ʮ��������
u8 RTC_BCD2DEC(u8 val)
{
	u8 temp=0;
	temp=(val>>4)*10;
	return (temp+(val&0X0F));
}
//RTCʱ������
//hour,min,sec:Сʱ,����,����
//ampm:AM/PM,0=AM/24H,1=PM.
//����ֵ:0,�ɹ�
//       1,�����ʼ��ģʽʧ�� 
u8 RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm)
{
	u32 temp=0;
	//�ر�RTC�Ĵ���д����
	RTC->WPR=0xCA;
	RTC->WPR=0x53; 
	if(RTC_Init_Mode())return 1;//����RTC��ʼ��ģʽʧ��
	temp=(((u32)ampm&0X01)<<22)|((u32)RTC_DEC2BCD(hour)<<16)|((u32)RTC_DEC2BCD(min)<<8)|(RTC_DEC2BCD(sec));
	RTC->TR=temp;
	RTC->ISR&=~(1<<7);			//�˳�RTC��ʼ��ģʽ 
	return 0; 
}
//RTC��������
//year,month,date:��(0~99),��(1~12),��(0~31)
//week:����(1~7,0,�Ƿ�!)
//����ֵ:0,�ɹ�
//       1,�����ʼ��ģʽʧ�� 
u8 RTC_Set_Date(u8 year,u8 month,u8 date,u8 week)
{
	u32 temp=0;
 	//�ر�RTC�Ĵ���д����
	RTC->WPR=0xCA;
	RTC->WPR=0x53; 
	if(RTC_Init_Mode())return 1;//����RTC��ʼ��ģʽʧ��
	temp=(((u32)week&0X07)<<13)|((u32)RTC_DEC2BCD(year)<<16)|((u32)RTC_DEC2BCD(month)<<8)|(RTC_DEC2BCD(date)); 
	RTC->DR=temp;
	RTC->ISR&=~(1<<7);			//�˳�RTC��ʼ��ģʽ 
	return 0; 
}
//��ȡRTCʱ��
//*hour,*min,*sec:Сʱ,����,���� 
//*ampm:AM/PM,0=AM/24H,1=PM.
void RTC_Get_Time(u8 *hour,u8 *min,u8 *sec,u8 *ampm)
{
	u32 temp=0;
 	while(RTC_Wait_Synchro());	//�ȴ�ͬ��  	 
	temp=RTC->TR;
	*hour=RTC_BCD2DEC((temp>>16)&0X3F);
	*min=RTC_BCD2DEC((temp>>8)&0X7F);
	*sec=RTC_BCD2DEC(temp&0X7F);
	*ampm=temp>>22; 
}
//��ȡRTC����
//*year,*mon,*date:��,��,��
//*week:����
void RTC_Get_Date(u8 *year,u8 *month,u8 *date,u8 *week)
{
	u32 temp=0;
 	while(RTC_Wait_Synchro());	//�ȴ�ͬ��  	 
	temp=RTC->DR;
	*year=RTC_BCD2DEC((temp>>16)&0XFF);
	*month=RTC_BCD2DEC((temp>>8)&0X1F);
	*date=RTC_BCD2DEC(temp&0X3F);
	*week=(temp>>13)&0X07; 
}
//RTC��ʼ��
//Ĭ�ϳ���ʹ��LSE,��LSE����ʧ�ܺ�,�л�ΪLSI.
//ͨ��BKP�Ĵ���0��ֵ,�����ж�RTCʹ�õ���LSE/LSI:
//��BKP0==0X5050ʱ,ʹ�õ���LSE
//��BKP0==0X5051ʱ,ʹ�õ���LSI
//����ֵ:0,��ʼ���ɹ�;
//       1,�����ʼ��ģʽʧ��;
//ע��:�л�LSI/LSE������ʱ��/���ڶ�ʧ,�л�������������.
u8 RTC_Init(void)
{  
	u16 ssr;
	u16 bkpflag=0;
	u16 retry=200; 
	u32 tempreg=0; 
	PWR->CR1|=1<<8;					//DBP=1,������дʹ�� 
	bkpflag=RTC_Read_BKR(0);		//��ȡBKP0��ֵ
	if(bkpflag!=0X5050)				//֮ǰʹ�õĲ���LSE
	{ 
		RCC->CSR|=1<<0;				//LSI����ʹ��
		while(!(RCC->CSR&0x02));	//�ȴ�LSI���� 
		RCC->BDCR|=1<<0;			//���Կ���LSE 
		while(retry&&((RCC->BDCR&0X02)==0))//�ȴ�LSE׼����
		{
			retry--;
			delay_ms(5);
		}
		tempreg=RCC->BDCR;			//��ȡBDCR��ֵ
		tempreg&=~(3<<8);			//����8/9λ 
		if(retry==0)tempreg|=1<<9;	//LSE����ʧ��,����LSI. 
		else tempreg|=1<<8;			//ѡ��LSE,��ΪRTCʱ��   
		tempreg|=1<<15;				//ʹ��RTCʱ�� 
		RCC->BDCR=tempreg;			//��������BDCR�Ĵ���
 		//�ر�RTC�Ĵ���д����
		RTC->WPR=0xCA;
		RTC->WPR=0x53; 
		RTC->CR=0;
		if(RTC_Init_Mode())
		{
			RCC->BDCR=1<<16;		//��λBDCR 
			delay_ms(10);
			RCC->BDCR=0;			// ������λ
			return 2;				//����RTC��ʼ��ģʽʧ��
		}
		RTC->PRER=0XFF;				//RTCͬ����Ƶϵ��(0~7FFF),����������ͬ����Ƶ,�������첽��Ƶ,Frtc=Fclks/((Sprec+1)*(Asprec+1))
		RTC->PRER|=0X7F<<16;		//RTC�첽��Ƶϵ��(1~0X7F)
		RTC->CR&=~(1<<6);			//RTC����Ϊ,24Сʱ��ʽ
		RTC->ISR&=~(1<<7);			//�˳�RTC��ʼ��ģʽ
		RTC->WPR=0xFF;				//ʹ��RTC�Ĵ���д����  
		if(bkpflag!=0X5051)			//BKP0�����ݼȲ���0X5050,Ҳ����0X5051,˵���ǵ�һ������,��Ҫ����ʱ������.
		{
			RTC_Set_Time(23,59,56,0);	//����ʱ��
			RTC_Set_Date(18,7,29,7);	//��������
			//RTC_Set_AlarmA(7,0,0,10);	//��������ʱ��
		}
		if(retry==0)RTC_Write_BKR(0,0X5051);	//����Ѿ���ʼ������,ʹ��LSI
		else RTC_Write_BKR(0,0X5050);			//����Ѿ���ʼ������,ʹ��LSE
	}else
	{
		retry=10;		//����10��SSR��ֵ��û�仯,��LSE����.
		ssr=RTC->SSR;	//��ȡ��ʼֵ
		while(retry)	//���ssr�Ĵ����Ķ�̬,���ж�LSE�Ƿ�����
		{ 
			delay_ms(10);
			if(ssr==RTC->SSR)retry--;	//�Ա�
			else break;
		}  
		if(retry==0)	//LSE����,������õȴ��´ν�����������
		{
			RTC_Write_BKR(0,0XFFFF);	//��Ǵ����ֵ
			RCC->BDCR=1<<16;			//��λBDCR 
			delay_ms(10);
			RCC->BDCR=0;				//������λ
		}
	}
	//RTC_Set_WakeUp(4,0);				//����WAKE UP�ж�,1�����ж�һ�� 
	return 0;
}
//��������ʱ��(����������,24Сʱ��)
//week:���ڼ�(1~7)
//hour,min,sec:Сʱ,����,����
void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec)
{ 
	//�ر�RTC�Ĵ���д����
	RTC->WPR=0xCA; 
	RTC->WPR=0x53;
	RTC->CR&=~(1<<8);		//�ر�����A
	while((RTC->ISR&0X01)==0);//�ȴ�����A�޸����� 
	RTC->ALRMAR=0;			//���ԭ������
	RTC->ALRMAR|=1<<30;		//���������� 
	RTC->ALRMAR|=0<<22;		//24Сʱ��
	RTC->ALRMAR|=(u32)RTC_DEC2BCD(week)<<24;//��������
	RTC->ALRMAR|=(u32)RTC_DEC2BCD(hour)<<16;//Сʱ����
	RTC->ALRMAR|=(u32)RTC_DEC2BCD(min)<<8;	//��������
	RTC->ALRMAR|=(u32)RTC_DEC2BCD(sec);		//��������  
	RTC->ALRMASSR=0;						//��ʹ��SUB SEC
	RTC->CR|=1<<12;			//��������A�ж�
	RTC->CR|=1<<8;			//��������A 
	RTC->WPR=0XFF;			//��ֹ�޸�RTC�Ĵ��� 
	
	RTC->ISR&=~(1<<8);		//���RTC����A�ı�־
	EXTI_D1->PR1=1<<17;		//���LINE17�ϵ��жϱ�־λ  
	EXTI_D1->IMR1|=1<<17;	//����line17�ϵ��ж� 
	EXTI->RTSR1|=1<<17;		//line17���¼��������ش��� 
	MY_NVIC_Init(2,2,RTC_Alarm_IRQn,2); //��ռ2�������ȼ�2����2 
}
//�����Ի��Ѷ�ʱ������
//wksel:000,RTC/16;001,RTC/8;010,RTC/4;011,RTC/2;
//      10x,ck_spre,1Hz;11x,1Hz,��cntֵ����2^16(��cnt+2^16)
//ע��:RTC����RTC��ʱ��Ƶ��,��RTCCLK!
//cnt:�Զ���װ��ֵ.����0,�����ж�.
void RTC_Set_WakeUp(u8 wksel,u16 cnt)
{ 
	//�ر�RTC�Ĵ���д����
	RTC->WPR=0xCA; 
	RTC->WPR=0x53;
	RTC->CR&=~(1<<10);			//�ر�WAKE UP
	while((RTC->ISR&0X04)==0);	//�ȴ�WAKE UP�޸�����
	RTC->CR&=~(7<<0);			//���ԭ��������
	RTC->CR|=wksel&0X07;		//�����µ�ֵ
	RTC->WUTR=cnt;				//����WAKE UP�Զ���װ�ؼĴ���ֵ
	RTC->ISR&=~(1<<10);			//���RTC WAKE UP�ı�־
	RTC->CR|=1<<14;				//����WAKE UP ��ʱ���ж�
	RTC->CR|=1<<10;				//����WAKE UP ��ʱ����
	RTC->WPR=0XFF;				//��ֹ�޸�RTC�Ĵ��� ��
	EXTI_D1->PR1=1<<19;			//���LINE19�ϵ��жϱ�־λ  
	EXTI_D1->IMR1|=1<<19;		//����line19�ϵ��ж� 
	EXTI->RTSR1|=1<<19;			//line19���¼��������ش��� 
	MY_NVIC_Init(2,2,RTC_WKUP_IRQn,2); //��ռ2�������ȼ�2����2 
}

//RTC�����жϷ�����
void RTC_Alarm_IRQHandler(void)
{    			    
	if(RTC->ISR&(1<<8))		//ALARM A�ж�?
	{
		RTC->ISR&=~(1<<8);	//����жϱ�־
		//printf("ALARM A!\r\n");
	}   
	EXTI_D1->PR1|=1<<17;	//����ж���17���жϱ�־   											 
}
//extern void ui_Tim_Dispaly(u816 x,u16 y,u16 fc,u16 bc,char font,u8 on);
//RTC WAKE UP�жϷ�����
void RTC_WKUP_IRQHandler(void)
{    
	static u8 led1sta;		
	if(RTC->ISR&(1<<10))	//WK_UP�ж�?
	{ 
		RTC->ISR&=~(1<<10);	//����жϱ�־
		LED1(led1sta^=1); 
		//ui_Tim_Dispaly(270,12,GRAY1,BLACK,12,led1sta^=1);
	}   
	EXTI_D1->PR1|=1<<19;	//����ж���19���жϱ�־ 	    
}
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�	  
//������������ڼ�
//��������:���빫�����ڵõ�����(ֻ����1901-2099��)
//year,month,day������������ 
//����ֵ�����ں�(1~7,������1~����)																						 
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{	
	u16 temp2;
	u8 yearH,yearL;
	yearH=year/100;	yearL=year%100; 
	// ���Ϊ21����,�������100  
	if (yearH>19)yearL+=100;
	// ����������ֻ��1900��֮���  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	temp2%=7;
	if(temp2==0)temp2=7;
	return temp2;
}	












