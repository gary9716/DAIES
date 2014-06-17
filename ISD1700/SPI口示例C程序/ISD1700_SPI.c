/************************************************************************
* ISD1700 89C51 µ•∆¨ª˙C ”Ô—‘ æ¿˝≥Ã–Ú*
* π¶ƒ‹£∫ 1700 –æ∆¨SPI π§◊˜ƒ£ Ωœ¬∑≈“Ù°¢¬º“Ù°¢≤¡≥˝µ»π¶ƒ‹µƒ±‡≥Ã æ¿˝ *
*************************************************************************/
//***********************Õ∑Œƒº˛***************************//
#include "REG51.h"
//************************∫Í∂®“Â***************************//
#define uchar unsigned char
#define uint unsigned int
//**************ISD1700 ◊¥Ã¨ºƒ¥Ê∆˜º∞∏˜∏ˆ±Í÷æŒª∂®“Â*****************//
unsigned char bdata SR0_L; // SR0 ºƒ¥Ê∆˜
unsigned char bdata SR0_H;
unsigned char bdata SR1; // SR1 ºƒ¥Ê∆˜
unsigned char APCL=0,APCH=0; // APC ºƒ¥Ê∆˜
unsigned char PlayAddL=0,PlayAddH=0; // ∑≈“Ù÷∏’ÎµÕŒª£¨∏ﬂŒª
unsigned char RecAddL=0,RecAddH=0; // ¬º“Ù÷∏’ÎµÕŒª£¨∏ﬂŒª
sbit CMD=SR0_L^0; // SPI ÷∏¡Ó¥ÌŒÛ±Í÷æŒª
sbit FULL=SR0_L^1; // –æ∆¨¥Ê¥¢ø’º‰¬˙±Í÷æ
sbit PU=SR0_L^2; // …œµÁ±Í÷æŒª
sbit EOM=SR0_L^3; // EOM ±Í÷æŒª
sbit INTT=SR0_L^4; // ≤Ÿ◊˜ÕÍ≥…±Í÷æŒª
sbit RDY=SR1^0; // ◊º±∏Ω” ’÷∏¡Ó±Í÷æŒª
sbit ERASE=SR1^1; // ≤¡≥˝±Í÷æŒª
sbit PLAY=SR1^2; // ≤•∑≈±Í÷æŒª
sbit REC=SR1^1; // ¬º“Ù±Í÷æŒª
/**************************************************
* ISD1700 SPI ÷∏¡Ó∫Ø ˝…˘√˜ *
**************************************************/
unsigned char ISD_SendData(unsigned char dat);
unsigned char ISD_Devid (void);
void ISD_PU(void);
void ISD_STOP(void);
void ISD_Reset(void);
void ISD_Clr_Int(void);
void ISD_Rd_Status(void);
void ISD_Rd_Playptr(void);
void ISD_PD(void);
void ISD_Rd_Recptr(void);
void ISD_Play(void);
void ISD_Rec(void);
void ISD_Erase(void) ;
void ISD_G_Erase(void);
void ISD_Rd_APC(void);
void ISD_WR_APC2(unsigned char apcdatl,apcdath);
void ISD_WR_NVCFG(unsigned char apcdatl,apcdath);
void ISD_LD_NVCFG(void);
void ISD_FWD(void);
void ISD_CHK_MEM(void);
void ISD_EXTCLK(void);
void ISD_SET_PLAY (unsigned char Saddl,Saddh,Eaddl,Eaddh);
void ISD_SET_Rec (unsigned char Saddl,Saddh,Eaddl,Eaddh);
void ISD_SET_Erase(unsigned char Saddl,Saddh,Eaddl,Eaddh);
//**********************∂Àø⁄∂®“Â**************************//
sbit SS=P1^0;
sbit SCK=P1^1;
sbit MOSI=P1^2;
sbit MISO=P1^3;
sbit LED= P0^7;
sbit Key_AN= P3^2;
sbit Key_STOP= P3^3;
sbit Switch_PR=P1^4; //PLAY=0;REC=1;
//********************±Í÷æŒª∂®“Â***************************//
uchar bdata flag;
sbit PR_flag=flag^1; // ∑≈“Ù/¬º“Ù±Í÷æŒª£∫0=∑≈“Ù£¨1=¬º“Ù°£
sbit Erase_flag=flag^4;
sbit Stop_flag=flag^7;
//********************∆‰À¸∫Ø ˝…˘√˜**********************//
void Cpu_Init(void); // œµÕ≥≥ı ºªØ
void ISD_Init(void); // ISD1700 ≥ı ºªØ
void ISDWORK (void);
void delay(unsigned int t); // ms º∂—”≥Ÿ
/*************************** ÷˜∫Ø ˝ *****************************/
void main(void)
{ uchar i;
Cpu_Init(); // CPU º∞œµÕ≥±‰¡ø≥ı ºªØ
delay(1);
ISD_Init(); // ISD ≥ı ºªØ
while(1)
{if (Key_AN==0)
{delay(10);
if (Key_AN==0)
{
if (Switch_PR==1)
PR_flag=1;
else PR_flag=0;
ISDWORK();
}
}
if (Key_STOP==0)
{ delay(20);
if (Key_STOP==0)
{ for (i=100;i>0;i--)
{if (Key_STOP==1)
{flag=0x80;
ISDWORK();
ISD_Init();
break;
}
delay(30);
}
if (flag==0x80)
{flag=00;
continue;}
Erase_flag=1;
ISDWORK();
}
}
}
}
/******************************************************************************
*****
∫Ø ˝‘≠–Õ: void IniCpuIO()
π¶ƒ‹£∫ CPU º∞œµÕ≥±‰¡ø≥ı ºªØ
*******************************************************************************
****/
void Cpu_Init(void)
{
P0=P1=P2=P3=0xff;
TMOD=0x01; // ∂® ±∆˜≥ı ºªØ
EA=0; // πÿ±’÷–∂œ
flag=0; // ±Í÷æŒªπÈ0
LED=1;
}
/******************************************************************************
*****
∫Ø ˝‘≠–Õ: void ISD_Init(void);
π¶ƒ‹£∫ œµÕ≥≥ı ºªØ°£
*******************************************************************************
****/
void ISD_Init(void)
{ uchar i=2;
SS=1;
SCK=1;
MOSI=0;
do
{
ISD_PU(); // …œµÁ
delay(50);
ISD_Rd_Status(); // ∂¡»°◊¥Ã¨
}while(CMD||(!PU)); // if(CMD_Err==1)||(PU!=1))‘Ú‘Ÿ¥Œ∑¢ÀÕ…œµÁ÷∏¡Ó
ISD_WR_APC2(0x40,0x04); // Ω´0x0440£®–æ∆¨≥ˆ≥ßƒ¨»œ÷µ£¨ø…∏˘æ›–Ë“™∏¸∏ƒ ˝÷µ£©–¥»ÎAPC ºƒ¥Ê∆˜
do
{ISD_Rd_Status(); // µ»¥˝RDY Œª÷√1
}while(RDY==0);
do{LED=0;
delay(300);
LED=1;
delay(300);
i--;}while(i>0);
}
/**************************************************************************
* ∫Ø ˝‘≠–Õ: void ISDWORK (void)
* π¶ ƒ‹: ∂‘ISD1700 Ω¯––µƒœ‡πÿ≤Ÿ◊˜
**************************************************************************/
void ISDWORK (void)
{uchar i;
switch (flag)
{case 0x00: //∑≈“Ù
do{ISD_Rd_Status();
}while(RDY==0);
delay(10);
ISD_Play();
delay(2);
LED=0;
do{ISD_Rd_Status();
if (Key_STOP==0)
{delay(20);
if (Key_STOP==0)
{ISD_Clr_Int();
ISD_STOP();
return;
}
}
}while(PLAY==1);
LED=1;
ISD_Clr_Int();
do{ISD_Rd_Status();
}while(RDY==0);
delay(10);
ISD_FWD();
do{ISD_Rd_Status();
}while(RDY==0);
break;
case 0x02:
ISD_Clr_Int();
do{ISD_Rd_Status();
}while(RDY==0);
LED=0;
ISD_Rec();
do{ISD_Rd_Status();
if(FULL==1)
{for (i=3;i>0;i--)
{LED=0;
delay(300);
LED=1;
delay(200);}
}
}while(Key_AN==0);
ISD_Clr_Int();
ISD_STOP();
LED=1;
break;
case 0x10:
do{
ISD_Rd_Status();
}while(RDY==0);
delay(2);
ISD_G_Erase();
do{
ISD_Rd_Status();
}while(RDY==0);
for (i=3;i>0;i--)
{LED=0;
delay(300);
LED=1;
delay(200);}
break;
case 0x80:
ISD_Clr_Int();
ISD_STOP();
ISD_Reset();
ISD_PD();
break;
}
}
/**************************************************************************
* ∫Ø ˝‘≠–Õ: unsigned char ISD_SendData(unsigned char dat);
* π¶ ƒ‹: œÚCPU ∑¢ÀÕ&∂¡ªÿ ˝æ›
**************************************************************************/
unsigned char ISD_SendData(unsigned char dat) // ±£÷§Õ‚≤øSS=1
{
unsigned char i,j,BUF_ISD=dat;
SCK=1; // ≥ı ºÃıº˛
SS=0; //  πƒ‹ISD1700 µƒSPI
for(j=4;j>0;j--) // —”≥Ÿ
{;}
for(i=0;i<8;i++) // ∑¢ÀÕ&Ω” ’8 Œª ˝æ›
{
SCK=0;
for(j=2;j>0;j--) // —”≥Ÿ
{;}
if(BUF_ISD&0x01) // Ω´BUF_ISD ÷–µƒ◊ÓµÕŒª ˝æ›∑¢ÀÕµΩMOSI ∂Àø⁄
MOSI=1;
else
MOSI=0;
BUF_ISD>>=1; // BUF_ISD ”““∆“ªŒª
if(MISO) // ÷∏ˆΩ” ’MISO ∂Àø⁄µƒ ˝æ›°£Ω´ ˝æ›¥Ê‘⁄BUF_ISDµƒ◊Ó∏ﬂŒª
BUF_ISD|=0x80;
SCK=1;
for(j=6;j>0;j--) // —”≥Ÿ
{;}
}
MOSI=0;
return(BUF_ISD); //∑µªÿΩ” ’µΩµƒ ˝æ›
}
/**************************************************************************
* ∫Ø ˝‘≠–Õ: void ISD_PU(void); ∑¢ÀÕ÷∏¡Ó∫Û–Ë“™—”≥Ÿ50ms
* π¶ ƒ‹: …œµÁ
**************************************************************************/
void ISD_PU(void)
{
ISD_SendData(0x01); // ∑¢ÀÕPU √¸¡Ó
ISD_SendData(0x00);
SS=1;
}
/**************************************************************************
* ∫Ø ˝‘≠–Õ: void ISD_STOP(void);
* π¶ ƒ‹: Õ£÷πµ±«∞≤Ÿ◊˜
**************************************************************************/
void ISD_STOP(void)
{
ISD_SendData(0x02); // ∑¢ÀÕSTOP √¸¡Ó
ISD_SendData(0x00);
SS=1;
}
/**************************************************************************
* ∫Ø ˝‘≠–Õ: void ISD_Reset(void); ∑¢ÀÕ÷∏¡Ó∫Û–Ë“™—”≥Ÿ50ms
* π¶ ƒ‹: ISD1700 ∏¥Œª
**************************************************************************/
void ISD_Reset(void)
{
ISD_SendData(0x03); // ∑¢ÀÕRESET √¸¡Ó
ISD_SendData(0x00);
SS=1;
}
/**************************************************************************
* ∫Ø ˝‘≠–Õ: void ISD_Clr_Int()(void);
* π¶ ƒ‹: «Â≥˝÷–∂œ±Í÷æ
**************************************************************************/
void ISD_Clr_Int(void)
{
ISD_SendData(0x04);
ISD_SendData(0x00);
SS=1;
}
/**************************************************************************
* ∫Ø ˝‘≠–Õ: void ISD_Rd_Status(void);
* π¶ ƒ‹: ∂¡»°◊¥Ã¨ºƒ¥Ê∆˜ƒ⁄»›
**************************************************************************/
void ISD_Rd_Status(void)
{ unsigned char i ;
ISD_SendData(0x05); // ∑¢ÀÕ
ISD_SendData(0x00);
ISD_SendData(0x00);
SS=1;
for(i=2;i>0;i--) //—”≥Ÿ
{;}
SR0_L=ISD_SendData(0x05); // ¥”MISO ∂¡≥ˆ◊¥Ã¨
SR0_H=ISD_SendData(0x00);
SR1=ISD_SendData(0x00);
SS=1;
}
/**************************************************************************
* ∫Ø ˝‘≠–Õ: void ISD_Rd_PlayPtr(void);
* π¶ ƒ‹: ∂¡»°∑≈“Ù÷∏’Î
**************************************************************************/
void ISD_Rd_Playptr(void)
{ unsigned char i;
ISD_SendData(0x06); // ∑¢ÀÕ
ISD_SendData(0x00);
ISD_SendData(0x00);
ISD_SendData(0x00);
SS=1;
for(i=2;i>0;i--) //—”≥Ÿ
{;}
SR0_L=ISD_SendData(0x06); // ¥”MISO ∂¡≥ˆ◊¥Ã¨
SR0_H=ISD_SendData(0x00);
PlayAddL=ISD_SendData(0x00); // ∂¡»°∑≈“Ù÷∏’ÎµÕ8 Œª
PlayAddH=ISD_SendData(0x00); // ∂¡»°∑≈“Ù÷∏’Î∏ﬂ8 Œª
SS=1;
}
/**************************************************************************
* ∫Ø ˝‘≠–Õ: void ISD_PD(void); ∑¢ÀÕ÷∏¡Ó∫Û–Ë“™—”≥Ÿ50ms
* π¶ ƒ‹: œ¬µÁ
**************************************************************************/
void ISD_PD(void)
{
ISD_SendData(0x07); // ∑¢ÀÕPD √¸¡Ó
ISD_SendData(0x00);
SS=1;
}
/**************************************************************************
* ∫Ø ˝‘≠–Õ: void ISD_Rd_RecPtr(void);
* π¶ ƒ‹: ∂¡»°¬º“Ù÷∏’Î
**************************************************************************/
void ISD_Rd_Recptr(void)
{ unsigned char i;
ISD_SendData(0x08); // ∑¢ÀÕ
ISD_SendData(0x00);
ISD_SendData(0x00);
ISD_SendData(0x00);
SS=1;
for(i=2;i>0;i--) //—”≥Ÿ
{;}
SR0_L=ISD_SendData(0x08); // ¥”MISO ∂¡≥ˆ◊¥Ã¨
SR0_H=ISD_SendData(0x00);
RecAddL=ISD_SendData(0x00); // ∂¡»°¬º“Ù÷∏’ÎµÕ8 Œª
RecAddH=ISD_SendData(0x00); // ∂¡»°¬º“Ù÷∏’Î∏ﬂ8 Œª
SS=1;
}
/**************************************************************************
* ∫Ø ˝‘≠–Õ: unsigned char ISD_Devid (void);
* π¶ ƒ‹: ∂¡»°APC ºƒ¥Ê∆˜ƒ⁄»›
**************************************************************************/
unsigned char ISD_Devid (void)
{ unsigned char DeviceNO,i;
ISD_SendData(0x09);
ISD_SendData(0x00);
ISD_SendData(0x00);
SS=1;
for(i=2;i>0;i--) //—”≥Ÿ
{;}
ISD_SendData(0x09);
ISD_SendData(0x00);
DeviceNO=ISD_SendData(0x00);
SS=1;
return DeviceNO;
}
/**************************************************************************
* ∫Ø ˝‘≠–Õ: void ISD_Play()(void);
* π¶ ƒ‹: ∑≈“Ù
**************************************************************************/
void ISD_Play(void)
{
ISD_SendData(0x40); // ∑¢ÀÕ∑≈“Ù√¸¡Ó
ISD_SendData(0x00);
SS=1;
}
/**************************************************************************
* ∫Ø ˝‘≠–Õ: void ISD_Rec()(void);
* π¶ ƒ‹: ¬º“Ù
**************************************************************************/
void ISD_Rec(void)
{
ISD_SendData(0x41); // ∑¢ÀÕ¬º“Ù√¸¡Ó
ISD_SendData(0x00);
SS=1;
}
/**************************************************************************
* ∫Ø ˝‘≠–Õ: void ISD_Erase(void);
* π¶ ƒ‹: ≤¡≥˝
**************************************************************************/
void ISD_Erase(void)
{
ISD_SendData(0x42); // ∑¢ÀÕ≤¡≥˝÷∏¡Ó
ISD_SendData(0x00);
SS=1;
}
/**************************************************************************
* ∫Ø ˝‘≠–Õ: void ISD_G_Erase(void);
* π¶ ƒ‹: »´ÃÂ≤¡≥˝
**************************************************************************/
void ISD_G_Erase(void)
{
ISD_SendData(0x43); // ∑¢ÀÕ»´ÃÂ≤¡≥˝√¸¡Ó
ISD_SendData(0x00);
SS=1;
}
/**************************************************************************
* ∫Ø ˝‘≠–Õ: void ISD_RD_APC(void);
* π¶ ƒ‹: ∂¡»°APC ºƒ¥Ê∆˜ƒ⁄»›
**************************************************************************/
void ISD_Rd_APC(void)
{ unsigned char i;
ISD_SendData(0x44);
ISD_SendData(0x00);
ISD_SendData(0x00);
ISD_SendData(0x00);
SS=1;
for(i=2;i>0;i--) //—”≥Ÿ
{;}
SR0_L=ISD_SendData(0x44);
SR0_H=ISD_SendData(0x00);
APCL=ISD_SendData(0x00);
APCH=ISD_SendData(0x00);
SS=1;
}
/**************************************************************************
* ∫Ø ˝‘≠–Õ: void ISD_WR_APC2(unsigned char apcdatl,apcdath);
* π¶ ƒ‹: …Ë÷√APC2
**************************************************************************/
void ISD_WR_APC2(unsigned char apcdatl,apcdath)
{
ISD_SendData(0x65);
ISD_SendData(apcdatl); //∑¢ÀÕµÕ8 Œª ˝æ›
ISD_SendData(apcdath); //∑¢ÀÕ∏ﬂ8 Œª ˝æ›
SS=1;
}
/**************************************************************************
* ∫Ø ˝‘≠–Õ: void ISD_WR_NVCFG(unsigned char apcdatl,apcdath);
* π¶ ƒ‹: Ω´APC  ˝æ›–¥»ÎNVCFG ºƒ¥Ê∆˜÷–
**************************************************************************/
void ISD_WR_NVCFG(unsigned char apcdatl,apcdath)
{
ISD_SendData(0x46);
ISD_SendData(apcdatl); //∑¢ÀÕµÕ8 Œª ˝æ›
ISD_SendData(apcdath); //∑¢ÀÕ∏ﬂ8 Œª ˝æ›
SS=1;
}
/**************************************************************************
* ∫Ø ˝‘≠–Õ: void ISD_LD_NVCFG(void);
* π¶ ƒ‹: Ω´NVCFG  ˝æ›‘ÿ»ÎAPC ºƒ¥Ê∆˜÷–
**************************************************************************/
void ISD_LD_NVCFG(void)
{
ISD_SendData(0x47);
ISD_SendData(0x00);
SS=1;
}
/**************************************************************************
* ∫Ø ˝‘≠–Õ: void ISD_FWD(void);
* π¶ ƒ‹: ∑≈“Ù÷∏’Îœ¬Ã¯“ª∂Œ£¨÷∏œÚœ¬“ª∂Œ”Ô“Ùµƒ∆ ºµÿ÷∑
**************************************************************************/
void ISD_FWD(void)
{
ISD_SendData(0x48);
ISD_SendData(0x00);
SS=1;
}
/**************************************************************************
* ∫Ø ˝‘≠–Õ: void ISD_CHK_MEM()(void);
* π¶ ƒ‹: ø’º‰ºÏ≤È
**************************************************************************/
void ISD_CHK_MEM(void)
{
ISD_SendData(0x49);
ISD_SendData(0x00);
SS=1;
}
/**************************************************************************
* ∫Ø ˝‘≠–Õ: void ISD_EXTCLK(void);
* π¶ ƒ‹: ∆Ù”√/πÿ±’ Õ‚≤ø ±÷”ƒ£ Ω
**************************************************************************/
void ISD_EXTCLK(void)
{
ISD_SendData(0x4a);
ISD_SendData(0x00);
SS=1;
}
/**************************************************************************
* ∫Ø ˝‘≠–Õ: void ISD_SET_PLAY( unsigned char Saddl£¨Saddh£¨Eaddl£¨Eaddh)
* π¶ ƒ‹: ∂®µ„≤•∑≈
*
**************************************************************************/
void ISD_SET_PLAY( unsigned char Saddl,Saddh,Eaddl,Eaddh)
{
ISD_SendData(0x80);
ISD_SendData(0x00);
ISD_SendData(Saddl); // ø™ ºµÿ÷∑µÕ8 Œª
ISD_SendData(Saddh); // ø™ ºµÿ÷∑∏ﬂ8 Œª
ISD_SendData(Eaddl); // Ω· ¯µÿ÷∑µÕ8 Œª
ISD_SendData(Eaddh); // Ω· ¯µÿ÷∑∏ﬂ8 Œª
ISD_SendData(0x00);
SS=1;
}
/**************************************************************************
* ∫Ø ˝‘≠–Õ: void ISD_SET_Rec( unsigned char Saddl£¨Saddh£¨Eaddl£¨Eaddh)
* π¶ ƒ‹: ∂®µ„¬º“Ù
*
**************************************************************************/
void ISD_SET_Rec( unsigned char Saddl,Saddh,Eaddl,Eaddh)
{
ISD_SendData(0x81);
ISD_SendData(0x00);
ISD_SendData(Saddl); // ø™ ºµÿ÷∑µÕ8 Œª
ISD_SendData(Saddh); // ø™ ºµÿ÷∑∏ﬂ8 Œª
ISD_SendData(Eaddl); // Ω· ¯µÿ÷∑µÕ8 Œª
ISD_SendData(Eaddh); // Ω· ¯µÿ÷∑∏ﬂ8 Œª
ISD_SendData(0x00);
SS=1;
}
/**************************************************************************
* ∫Ø ˝‘≠–Õ: void ISD_SET_Erase( unsigned char Saddl£¨Saddh£¨Eaddl£¨Eaddh)
* π¶ ƒ‹: ∂®µ„¬º“Ù
*
**************************************************************************/
void ISD_SET_Erase( unsigned char Saddl,Saddh,Eaddl,Eaddh)
{
ISD_SendData(0x82);
ISD_SendData(0x00);
ISD_SendData(Saddl); // ø™ ºµÿ÷∑µÕ8 Œª
ISD_SendData(Saddh); // ø™ ºµÿ÷∑∏ﬂ8 Œª
ISD_SendData(Eaddl); // Ω· ¯µÿ÷∑µÕ8 Œª
ISD_SendData(Eaddh); // Ω· ¯µÿ÷∑∏ﬂ8 Œª
ISD_SendData(0x00);
SS=1;
}
/****************************************************
* ∫Ø ˝‘≠–Õ: void delay(unsigned int t)
* π¶ ƒ‹: ms º∂—”≥Ÿ
****************************************************/
void delay(unsigned int t) //ms º∂—”≥Ÿ,12M æß’Ò
{
for(;t>0;t--)
{TH0=0xfc; //TMOD: º∆ ˝∆˜0, π§◊˜∑Ω Ω 1, (65536-X)*1¶Ã s=1ms,X=64536=FC18
TL0=0x18;
TR0=1;
while(TF0!=1)
{;}
TF0=0;
TR0=0;
}
}