//��������"������ʾС������ʵ��,�밴K1����ʼ"
//Ȼ����K1��,С����ʼǰ��,�������ϰ���ʱ,С������"̫Σ����",Ȼ��ʼ����


#include <at89x51.h>                      //����51��Ƭ����ص�ͷ�ļ�
#define uchar unsigned char
#define uint unsigned int
sbit LeftLed=P2^4;                        //����ǰ�����ָʾ�ƶ˿�
sbit RightLed=P2^5;                       //����ǰ���Ҳ�ָʾ�ƶ˿�
sbit LeftIR=P3^6;                         //����ǰ��������̽ͷ�˿�
sbit RightIR=P3^5;                        //����ǰ���Ҳ����̽ͷ�˿�
sbit FontIR=P3^7;                         //����ǰ����ǰ������̽ͷ�˿�
sbit IN1=P1^0;						//����ǰ�����˿���
sbit IN3=P1^1;						//����ǰ�����˿���
sbit EN1=P1^2;						//����ת���ٶȿ���
sbit EN2=P1^3;						//����ת���ٶȿ���
sbit BEEP=P1^6;						//������
uint tim=0;	
sbit K1=P2^4;

sbit play=P0^0;
sbit fwd=P0^1; 
bit up_flag=0;
bit down_flag=0;
bit stop_flag=0;

/********��������ʱ����********/
void Delay_ms(uint xms)				
{
	uint i,j;
	for(i=xms;i>0;i--)		      //i=xms����ʱԼxms����
		for(j=110;j>0;j--);
}
/*********�����Ƿ�������һ������********/
void  beep()
{
  BEEP=0;      		 	//��������
  Delay_ms(100); 
  BEEP=1;           		//�رշ�����
  Delay_ms(100);    
}




void Delay()                              //��������˵�ת��ʱ���ӳ���
{ unsigned int DelayTime=50000;           //���������ת��ʱ�����
  while(DelayTime--);                     //������ת��ѭ��
  BEEP=!BEEP;                               //����������
  return;
}

void ControlCar(unsigned char ConType)    //�����������ӳ���
{
// EN1=1;   
// EN2=1;  
 switch(ConType)                          //�ж��û��趨�����ʽ
 {
  	case 1:  								//ǰ��                         
  	{
		ET0=1; IN1=1;IN3=1;LeftLed=1;RightLed=1;//С��ǰ��,������ָʾ��Ϩ��  
    	break;
  	}
  	case 2: 								//����    
  	{ 
		ET0=1; IN1=0;IN3=0;LeftLed=0;RightLed=0;//С������,������ָʾ�Ƶ���
    	break;
  	}
  	case 3: 									//��ת                     
  	{ 
		ET0=1; IN1=0;IN3=1;LeftLed=0;RightLed=1;//С����ת,��ָʾ�Ƶ���
		break;
  	}
  	case 4: 									//��ת 
  	{ 
		ET0=1; IN1=1;IN3=0;LeftLed=1;RightLed=0;//С����ת,��ָʾ�Ƶ���
		break;
  	}
  	case 5: 								//ֹͣ
  	{
		ET0=0; EN1=1;EN2=1;IN1=1;IN3=1;LeftLed=1;RightLed=1;//�رն�ʱ��T0,С��ֹͣ,������ָʾ��Ϩ��    
		break;                                //�˳���ǰѡ��
  	}
 }
}

void timer_init()  
{
	TMOD=0x01;             //��ʱ��0�����ڷ�ʽ1
	ET0=1;                 //��ʱ��0�ж�����
    	TH0=0xfc;         //��ʱ1ms
    	TL0=0x66;
    	TR0=1;                 //��ʱ��0��ʼ����
    	EA=1;                  //ϵͳ�ж�����
}

void main()                               //���������
{
  bit RunFlag=0;                          //����С�����б�־λ
timer_init();
  ControlCar(5);                          //С����ʼʱ����ֹͣ״̬
		fwd=0;
	beep();
	fwd=1;
 Delay_ms(2000);
	play=0;								//���ŵ�һ��"С������ʵ�����ڿ�ʼ,���ò���,����ָ��!"
	beep();
	play=1;
Delay_ms(8000);
		fwd=0;
	beep();
	fwd=1;
while(K1);

 while(1)                                //������ѭ��
  {
ControlCar(1); 
   if(FontIR==0 )//|| LeftIR==1 || RightIR==1) //�ж�ǰ��,���,�Ҳ��Ƿ����ź�����  
   {ControlCar(2);                        //��С������
	Delay();                              //��ʼС��������ʱ�ӳ���
	Delay();                              //������С�����˵Ĳ���
//	Delay();                              //�ı������Ը�С�����˵ľ���
//	Delay();                              //����ֻ��һ����ʱ
//
	play=0;								//���ŵڶ���"̫Σ����!"
	beep();
	play=1;
    ControlCar(3);                        //�ı�С��Ϊ��ת����
	Delay();                              //��ʼС����ת��ʱ�ӳ���
	Delay();                              //�ı������Ըı�С����ת�ĽǶ�
//	Delay();                              //ͬ��
//	Delay();                              //ͬ��
	ControlCar(1);                        //�ı�С��Ϊǰ������״̬
	//SB1=1;                                //�����һ�����������ر�
   }
   
  }
}

void Timer0(void) interrupt 1        //��ʱ��0�жϴ��������
{
	TH0=0xfc;         //��ʱ1ms
    TL0=0x66;	
	EA=0;                               //���ж�
	tim=tim+1;                        //ʱ�������1
	if(tim<20){EN1=0;EN2=0;}     	//��timС��20,������С��ת��	      
	else {EN1=1;EN2=1;}				//��tim����20,��С��ֹͣת��
	if(tim==100)tim=0;				//ռ�ձ�Ϊ20%	
	EA=1;                              //���ж�
}



