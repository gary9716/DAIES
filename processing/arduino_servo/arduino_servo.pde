/*
arduino_servo

Demonstrates the control of servo motors connected to an Arduino board
running the StandardFirmata firmware.  Moving the mouse horizontally across
the sketch changes the angle of servo motors on digital pins 4 and 7.  For
more information on servo motors, see the reference for the Arduino Servo
library: http://arduino.cc/en/Reference/Servo

To use:
* Using the Arduino software, upload the StandardFirmata example (located
  in Examples > Firmata > StandardFirmata) to your Arduino board.
* Run this sketch and look at the list of serial ports printed in the
  message area below. Note the index of the port corresponding to your
  Arduino board (the numbering starts at 0).  (Unless your Arduino board
  happens to be at index 0 in the list, the sketch probably won't work.
  Stop it and proceed with the instructions.)
* Modify the "arduino = new Arduino(...)" line below, changing the number
  in Arduino.list()[0] to the number corresponding to the serial port of
  your Arduino board.  Alternatively, you can replace Arduino.list()[0]
  with the name of the serial port, in double quotes, e.g. "COM5" on Windows
  or "/dev/tty.usbmodem621" on Mac.
* Connect Servos to digital pins 4 and 7.  (The servo also needs to be
  connected to power and ground.)
* Run this sketch and move your mouse horizontally across the screen.
  
For more information, see: http://playground.arduino.cc/Interfacing/Processing
*/

import processing.serial.*;

import cc.arduino.*;

Arduino arduino;
final int servoPin = 4;

void setup() {
  size(360, 200);
  // Prints out the available serial ports.
  println(Arduino.list());
  
  // Alternatively, use the name of the serial port corresponding to your
  // Arduino (in double-quotes), as in the following line.
  //arduino = new Arduino(this, "/dev/tty.usbmodem1411", 57600);
  try {
    arduino = new Arduino(this, "/dev/tty.usbmodem1421", 57600);
  }
  catch(Exception e) {
    try {
      arduino = new Arduino(this, "/dev/tty.usbmodem1411", 57600);
    }
    catch(Exception e2) {
      exit();
    }
  }
}

boolean counterClockWiseMode = false;
boolean clockWiseMode = false;

void delay(int delay)
{
  int time = millis();
  while(millis() - time <= delay);
}

void draw() {
  background(constrain(mouseX / 2, 0, 180));
  //println(mouseX);
  // Write an value to the servos, telling them to go to the corresponding
  // angle (for standard servos) or move at a particular speed (continuous
  // rotation servos).
  
  if(counterClockWiseMode) {
    arduino.servoWrite(servoPin, 90);
  }
  else if(clockWiseMode) {
    arduino.servoWrite(servoPin, 60);
  }
   
}

void keyReleased() {
  
  if( key == 'z') {
    // Configure digital pins 4 and 7 to control servo motors.
    clockWiseMode = false;
    arduino.pinMode(servoPin, Arduino.SERVO);
    counterClockWiseMode = true;
  }
  if( key == 'x' ){
    counterClockWiseMode = false;
    arduino.pinMode(servoPin, Arduino.SERVO);
    clockWiseMode = true;
  }
  if( key == 'c' ) {
    counterClockWiseMode = false;
    clockWiseMode = false;
    arduino.pinMode(servoPin, Arduino.INPUT);
  }
  
}
