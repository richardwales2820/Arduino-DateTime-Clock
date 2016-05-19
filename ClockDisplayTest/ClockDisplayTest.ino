/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>
#include <Servo.h>
#include "MSPClock.h"

MSPClock clock(0, 0, 7, 1, 1, 2016);
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Servo s;
int pos = 0, elapsedSec = 0;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  Serial.begin(9600);
  s.attach(6);
  s.write(0);
  delay(2000);
}

void loop() {

  if (Serial.read() == 'A')
  {
      updateClock();
  }

  updateLcd();
  //clock.tickTock();
  fastTock();
  updateServo();
  //s.write(pos++);
  //delay(10);
}

void fastTock()
{
  int i;
  for (i = 0; i < 26; i++)
    clock.tickTock();
}

void updateClock()
{
  clock.sec = Serial.parseInt();
  
  while (Serial.read() != 'B') 
  { 
  }
  clock.min = Serial.parseInt();

  while (Serial.read() != 'C')
  {
  }
  clock.hour = Serial.parseInt();

  while (Serial.read() != 'D')
  {
  }
  clock.day = Serial.parseInt();

  while (Serial.read() != 'E')
  {
  }
  clock.month = Serial.parseInt();

  while (Serial.read() != 'F')
  {
  }
  clock.year = Serial.parseInt();

  clock.checkLeap();
}

void updateServo()
{
  if ((clock.hour >= 7) && (clock.hour <= 20))
  {
    elapsedSec++;

    if (elapsedSec % 10 == 0)
     {
      pos += 1;
      s.write(pos);
     }
  }
  else
  {
    pos = 0;
    s.write(0);
  }
}

void updateLcd()
{
  lcd.setCursor(0, 0);
    // print the number of seconds since reset:
  lcd.print(clock.hour);
  lcd.print(":");
  lcd.print(clock.min);
  lcd.print(":");
  lcd.print(clock.sec);
  lcd.print("      ");
  
  lcd.setCursor(0,1);

  lcd.print(clock.month);
  lcd.print("-");
  lcd.print(clock.day);
  lcd.print("-");
  lcd.print(clock.year);
  lcd.print("      ");
}

