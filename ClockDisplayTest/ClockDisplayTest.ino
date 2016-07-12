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
#include <string.h>
#define RIGHT 0
#define UP 1
#define DOWN 2
#define LEFT 3
#define SELECT 4
#define NONE 5

// Declare and initialize a clock object
// Format: MSPClock VARIABLE_NAME(SEC, MIN, HOUR, DAY, MONTH, YEAR);
MSPClock clock(0, 0, 7, 1, 1, 2016);

// initialize the LCD object with the numbers of the interface pins specified in the comments above
LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7);

// initialize new Servo object "s"
Servo s;

// pos holds the angular position of the servo
// elapsedSec holds how many seconds have passed since the beginning of sunrise
uint8_t pos = 0, elapsedSec = 0;
uint8_t secondsPerDegree = 0;
char *dayName[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
int readButtons()
{
  int key = analogRead(0);

  if (key > 1000) return NONE;
  if (key < 50) return RIGHT;
  if (key < 250) return UP;
  if (key < 450) return DOWN;
  if (key < 650) return LEFT;
  if (key < 850) return SELECT;
  
}

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
int checkkey = 100;

void loop() {

  //if (clock.hour == 0)
  //  setupTimesAndInterval();
  // Checks if the first tag byte "A" has been sent to the Arduino
  checkkey = readButtons();
  if (checkkey == SELECT)
  {
      // if it has, go into this function
      updateClock();
  }

  // have the LCD display the current, updated datetime
  updateLcd();

  // Increment the clock by 1 second
  //clock.tickTock();

  // Increment the clock by multiple seconds for debugging purposes
  fastTock();

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
  uint8_t i;

  // Incrememnt 26 seconds by calling tickTock() 26 times
  // For debugging purposes only
  for (i = 0; i < secondsPerDegree; i++)
    clock.tickTock();
}

// Updates the clock's variables according to values sent over serial
// From a PyQt application sent with pyserial
void updateClock()
{
  // Until the "B" tag has been sent, wait
  // After the loop breaks and the "B"tag was sent, parse the next bytes as the minutes value
  int key;
  int delayTime = 300;
  
  key = 100;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Seconds");
  delay(2000);
  
  while (key != SELECT) 
  {
    lcd.setCursor(0, 1);
    key = readButtons();
    
    if (key == UP)
    {
      clock.sec++;
      delay(delayTime);
    }
    if (key == DOWN)
    {
      clock.sec--; 
      delay(delayTime);
    }
    lcd.print(clock.sec);
    lcd.print("       ");
  }

  key = 100;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Minutes");
  delay(2000);
  
  while (key != SELECT) 
  {
    lcd.setCursor(0, 1);
    key = readButtons();
    
    if (key == UP)
    {
      clock.min++;
      delay(delayTime);
    }
    if (key == DOWN)
    {
      clock.min--; 
      delay(delayTime);
    }
    lcd.print(clock.min);
    lcd.print("       ");
  }

  key = 100;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hour");
  delay(2000);
  
  while (key != SELECT) 
  {
    lcd.setCursor(0, 1);
    key = readButtons();
    
    if (key == UP)
    {
      clock.hour++;
      delay(delayTime);
    }
    if (key == DOWN)
    {
      clock.hour--; 
      delay(delayTime);
    }
    lcd.print(clock.hour);
    lcd.print("       ");
  }

  key = 100;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Day");
  delay(2000);
  
  while (key != SELECT) 
  {
    lcd.setCursor(0, 1);
    key = readButtons();
    
    if (key == UP)
    {
      clock.day++;
      delay(delayTime);
    }
    if (key == DOWN)
    {
      clock.day--; 
      delay(delayTime);
    }
    lcd.print(clock.day);
    lcd.print("       ");
  }

  key = 100;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Month");
  delay(2000);
  
  while (key != SELECT) 
  {
    lcd.setCursor(0, 1);
    key = readButtons();
    
    if (key == UP)
    {
      clock.month++;
      delay(delayTime);
    }
    if (key == DOWN)
    {
      clock.month--; 
      delay(delayTime);
    }
    lcd.print(clock.month);
    lcd.print("       ");
  }

  key = 100;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Year");
  delay(2000);
  
  while (key != SELECT) 
  {
    lcd.setCursor(0, 1);
    key = readButtons();
    
    if (key == UP)
    {
      clock.year++;
      delay(delayTime);
    }
    if (key == DOWN)
    {
      clock.year--; 
      delay(delayTime);
    }
    lcd.print(clock.year);
    lcd.print("       ");
  }

  key = 100;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Day of Week");
  delay(2000);
  
  while (key != SELECT) 
  {
    lcd.setCursor(0, 1);
    key = readButtons();
    
    if (key == UP)
    {
      clock.dayOfWeek++;
      delay(delayTime);
    }
    if (key == DOWN)
    {
      clock.dayOfWeek--; 
      delay(delayTime);
    }
    if (clock.dayOfWeek < 0)
      clock.dayOfWeek = 6;
    else if (clock.dayOfWeek > 6)
      clock.dayOfWeek = 0;
    lcd.print(dayName[clock.dayOfWeek]);
    lcd.print("       ");
  }
  
  clock.checkLeap();
  clock.dstDone = 0;
  setupTimesAndInterval();

  updateLcd();
}

// Checks the time every second to updates the servo position
void updateServo()
{
  // TODO: add sunrise and sunset hours
  if ((clock.hour >= clock.sunriseHour && clock.min >= clock.sunriseMinute) && 
      (clock.hour <= clock.sunsetHour && clock.min <= clock.sunsetMinute))
  {
    // increase how many seconds have passed since sunrise
    //elapsedSec++;

    // if the servo needs to move 1 degree every 10 seconds,
    // check if 10 seconds have passed since last time
    // TODO: change to something like [(sunsetTime - sunriseTime).toSeconds() / 180 degrees]
    //if (elapsedSec % secondsPerDegree == 0)
    //{
      // Update pos variable and write servo to the new pos
      pos += 1;
      //s.write(pos);
    //}
  }

  // If not in daylight hours, have servo hold its position at 0 degrees
  else
  {
    pos = 0;
   // s.write(0);
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
  //lcd.print("=");
  //lcd.print((int)clock.sunriseHour);
  //lcd.print(":");
  //lcd.print((int)clock.sunriseMinute);
  
  lcd.setCursor(0,1);

  lcd.print(clock.month);
  lcd.print("-");
  lcd.print(clock.day);
  lcd.print("-");
  lcd.print(clock.year);
  lcd.print(" ");
  lcd.print(dayName[clock.dayOfWeek]);
  lcd.print("     ");
  //lcd.print("=");
  //lcd.print((int)clock.sunsetHour);
  //lcd.print(":");
  //lcd.print((int)clock.sunsetMinute);
}

