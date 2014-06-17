//开机播放"下面演示小车避障实验,请按K1键开始"
//然后按下K1键,小车开始前进,当遇到障碍特时,小车发出"太危险了",然后开始避障


#include <at89x51.h>                      //包含51单片机相关的头文件
#define uchar unsigned char
#define uint unsigned int
sbit LeftLed=P2^4;                        //定义前方左侧指示灯端口
sbit RightLed=P2^5;                       //定义前方右侧指示灯端口
sbit LeftIR=P3^6;                         //定义前方左侧红外探头端口
sbit RightIR=P3^5;                        //定义前方右侧红外探头端口
sbit FontIR=P3^7;                         //定义前方正前方红外探头端口
sbit IN1=P1^0;						//左轮前进后退控制
sbit IN3=P1^1;						//右轮前进后退控制
sbit EN1=P1^2;						//左轮转动速度控制
sbit EN2=P1^3;						//右轮转动速度控制
sbit BEEP=P1^6;						//蜂鸣器
uint tim=0;	
sbit K1=P2^4;

sbit play=P0^0;
sbit fwd=P0^1; 
bit up_flag=0;
bit down_flag=0;
bit stop_flag=0;

/********以下是延时函数********/
void Delay_ms(uint xms)				
{
	uint i,j;
	for(i=xms;i>0;i--)		      //i=xms即延时约xms毫秒
		for(j=110;j>0;j--);
}
/*********以下是蜂鸣器响一声函数********/
void  beep()
{
  BEEP=0;      		 	//蜂鸣器响
  Delay_ms(100); 
  BEEP=1;           		//关闭蜂鸣器
  Delay_ms(100);    
}




void Delay()                              //定义机器人调转子时间子程序
{ unsigned int DelayTime=50000;           //定义机器人转弯时间变量
  while(DelayTime--);                     //机器人转弯循环
  BEEP=!BEEP;                               //蜂鸣器闪响
  return;
}

void ControlCar(unsigned char ConType)    //定义电机控制子程序
{
// EN1=1;   
// EN2=1;  
 switch(ConType)                          //判断用户设定电机形式
 {
  	case 1:  								//前进                         
  	{
		ET0=1; IN1=1;IN3=1;LeftLed=1;RightLed=1;//小车前进,左右两指示灯熄灭  
    	break;
  	}
  	case 2: 								//后退    
  	{ 
		ET0=1; IN1=0;IN3=0;LeftLed=0;RightLed=0;//小车后退,左右两指示灯点亮
    	break;
  	}
  	case 3: 									//左转                     
  	{ 
		ET0=1; IN1=0;IN3=1;LeftLed=0;RightLed=1;//小车左转,左指示灯点亮
		break;
  	}
  	case 4: 									//右转 
  	{ 
		ET0=1; IN1=1;IN3=0;LeftLed=1;RightLed=0;//小车右转,右指示灯点亮
		break;
  	}
  	case 5: 								//停止
  	{
		ET0=0; EN1=1;EN2=1;IN1=1;IN3=1;LeftLed=1;RightLed=1;//关闭定时器T0,小车停止,左右两指示灯熄灭    
		break;                                //退出当前选择
  	}
 }
}

void timer_init()  
{
	TMOD=0x01;             //定时器0工作在方式1
	ET0=1;                 //定时器0中断允许
    	TH0=0xfc;         //定时1ms
    	TL0=0x66;
    	TR0=1;                 //定时器0开始计数
    	EA=1;                  //系统中断允许
}

void main()                               //主程序入口
{
  bit RunFlag=0;                          //定义小车运行标志位
timer_init();
  ControlCar(5);                          //小车初始时处于停止状态
		fwd=0;
	beep();
	fwd=1;
 Delay_ms(2000);
	play=0;								//播放第一段"小车避障实验现在开始,做得不好,请多多指导!"
	beep();
	play=1;
Delay_ms(8000);
		fwd=0;
	beep();
	fwd=1;
while(K1);

 while(1)                                //程序主循环
  {
ControlCar(1); 
   if(FontIR==0 )//|| LeftIR==1 || RightIR==1) //判断前方,左侧,右侧是否有信号输入  
   {ControlCar(2);                        //让小车后退
	Delay();                              //开始小车后退延时子程序
	Delay();                              //决定了小车后退的步数
//	Delay();                              //改变它可以改小车后退的距离
//	Delay();                              //这里只是一个延时
//
	play=0;								//播放第二段"太危险了!"
	beep();
	play=1;
    ControlCar(3);                        //改变小车为左转方向
	Delay();                              //开始小车左转延时子程序
	Delay();                              //改变它可以改变小车旋转的角度
//	Delay();                              //同上
//	Delay();                              //同上
	ControlCar(1);                        //改变小车为前进运行状态
	//SB1=1;                                //这个多一步将蜂鸣器关闭
   }
   
  }
}

void Timer0(void) interrupt 1        //定时器0中断处理函数入口
{
	TH0=0xfc;         //定时1ms
    TL0=0x66;	
	EA=0;                               //关中断
	tim=tim+1;                        //时间计数加1
	if(tim<20){EN1=0;EN2=0;}     	//若tim小于20,则允许小车转动	      
	else {EN1=1;EN2=1;}				//若tim大于20,则小车停止转动
	if(tim==100)tim=0;				//占空比为20%	
	EA=1;                              //开中断
}



