#include <ISD1700.h>

// This code has been borrowed from http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1234149784

ISD1700 chip(10); // Initialize chipcorder with
                  // SS at Arduino's digital pin 10

// SCK_PIN   13
// MISO_PIN  12
// MOSI_PIN  11

#define FT_PIN 0

void setup()
{
  Serial.begin(9600);
  //Serial.println("Sketch is starting up");
  pinMode(FT_PIN,OUTPUT);
  digitalWrite(FT_PIN,HIGH);
  chip.wr_apc2(word(B00000100,B10000000));
}

void loop()
{
  char c;
  if(Serial.available())
  {
    /* Power Up */
    chip.pu();
    c = Serial.read();
    switch(c)
    {
       case 'A':
         Serial.println(chip.rd_apc(), BIN);
         break;
       case 'Y':
         chip.play();
         break;
       case 'P':
         chip.stop();
         digitalWrite(FT_PIN,HIGH);
         break;
       case 'E':
         chip.erase();
         break;
       case 'R':
         //digitalWrite(FT_PIN,LOW);
         chip.rec();
         break;
       case 'F':
         chip.fwd();
         break;
       case 'Z':
         chip.g_erase();
         break;
       case 'I':
         Serial.println(chip.devid(), BIN);
         break;
       case 'Q':
         chip.wr_apc2(word(B00000100,B10000000));
         break;
       case 'X':
         chip.reset();
         break;
    }
    Serial.print("Status---> ");
    Serial.print(chip.CMD_ERR()? "CMD_ERR ": " ");
    Serial.print(chip.PU()? "PU ": " ");
    Serial.print(chip.RDY()? "RDY ": "Not_RDY");
    Serial.println();
    delay(1000);
  }
}
