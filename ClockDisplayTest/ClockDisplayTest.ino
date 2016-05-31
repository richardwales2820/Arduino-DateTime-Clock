/*
 * The LCD circuit:
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
 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 *
 *
 * Servo attached to 5V, GND, and digital pin 6
 */

// include the library code from LCD, Servo, and MSPClock:
#include <LiquidCrystal.h>
#include <Servo.h>
#include "MSPClock.h"

// Declare and initialize a clock object
// Format: MSPClock VARIABLE_NAME(SEC, MIN, HOUR, DAY, MONTH, YEAR);
MSPClock clock(0, 0, 7, 1, 1, 2016);

// initialize the LCD object with the numbers of the interface pins specified in the comments above
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// initialize new Servo object "s"
Servo s;

// pos holds the angular position of the servo
// elapsedSec holds how many seconds have passed since the beginning of sunrise
int pos = 0, elapsedSec = 0;
int secondsPerDegree = 0;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Begin serial communication for work with the computer
  Serial.begin(9600);

  // Attach Pin 6 to the servo object
  s.attach(6);

  // Set the servo to angular position 0
  s.write(0);

  // Wait 2 seconds to fully move the servo to the start position before running any other code
  delay(2000);

  setupTimesAndInterval();
}

void loop() {

  // Checks if the first tag byte "A" has been sent to the Arduino
  if (Serial.read() == 'A')
  {
      // if it has, go into this function
      updateClock();
  }

  // have the LCD display the current, updated datetime
  updateLcd();

  // Increment the clock by 1 second
  clock.tickTock();

  // Increment the clock by multiple seconds for debugging purposes
  //fastTock();

  // Move the servo accordingly depending on the time
  updateServo();

  // Wait 1 second to do everything again
  delay(1000);
}

void setupTimesAndInterval()
{
  clock.riseAndShine();
  secondsPerDegree = clock.getSecondsPerDegree();
}

// This is a function that tickTocks many times to simulate a full day in a shorter time
void fastTock()
{
  int i;

  // Incrememnt 26 seconds by calling tickTock() 26 times
  // For debugging purposes only
  for (i = 0; i < 26; i++)
    clock.tickTock();
}

// Updates the clock's variables according to values sent over serial
// From a PyQt application sent with pyserial
void updateClock()
{
  // The "A" tag was already found in the loop() function so we know
  // we can parse the seconds bytes now
  clock.sec = Serial.parseInt();


  // Until the "B" tag has been sent, wait
  // After the loop breaks and the "B"tag was sent, parse the next bytes as the minutes value
  while (Serial.read() != 'B') 
  { 
  }
  clock.min = Serial.parseInt();
  
  // Same process as above, repeated...
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

  while (Serial.read() != 'G')
  {
  }
  clock.dayOfWeek = Serial.parseInt();
  
  clock.checkLeap();
  clock.dstDone = 0;
  setupTimesAndInterval();
}

// Checks the time every second to updates the servo position
void updateServo()
{
  // TODO: add sunrise and sunset hours
  if ((clock.hour >= clock.sunriseHour && clock.min >= clock.sunriseMinute) && 
      (clock.hour <= clock.sunsetHour && clock.min <= clock.sunsetMinute))
  {
    // increase how many seconds have passed since sunrise
    elapsedSec++;

    // if the servo needs to move 1 degree every 10 seconds,
    // check if 10 seconds have passed since last time
    // TODO: change to something like [(sunsetTime - sunriseTime).toSeconds() / 180 degrees]
    if (elapsedSec % secondsPerDegree == 0)
    {
      // Update pos variable and write servo to the new pos
      pos += 1;
      s.write(pos);
    }
  }

  // If not in daylight hours, have servo hold its position at 0 degrees
  else
  {
    pos = 0;
    s.write(0);
  }
}

// Has the LCD display the current time from the clock object
void updateLcd()
{
  lcd.setCursor(0, 0);
    // print the number of seconds since reset:
  lcd.print(clock.hour);
  lcd.print(":");
  lcd.print(clock.min);
  lcd.print(":");
  lcd.print(clock.sec);
  // This blank print below is there to ensure that characters that were printed to the far right 
  // that are no longer present in the data get overwritten
  // EXAMPLE: LCD displays this on the top row: 0:20:59  
  // next second it shows: 0:21:09 instead of 0:21:00  WHY? Because it printed the values in clock but doesn't clear the screen each time it write
  // This empty lcd.print("      ") ensures that the top line gets "cleared" to avoid this issue
  lcd.print("      ");
  
  lcd.setCursor(0,1);

  lcd.print(clock.month);
  lcd.print("-");
  lcd.print(clock.day);
  lcd.print("-");
  lcd.print(clock.year);
  lcd.print("      ");
}

