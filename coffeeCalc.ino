//-497025
#include "src\HX711-master\src\HX711.h"
#include "src\pitches\pitches.h"
#include <Wire.h>
#include "src\NewliquidCrystal\LiquidCrystal_I2C.h"
#include "src\JC_Button-master\src\JC_Button.h"
#include "src\RunningAverage\RunningAverage.h"


#define BUZZER 8
#define BUTTON_PIN 2
#define LONG_PRESS 700
#define calibration_factor -496825
// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 10;
const int LOADCELL_SCK_PIN = 11;
//char data[16];
float CoffeeMass = 0;

int PreviousMinute = 0;
unsigned long CurrentMinute = 0; 
int PreviousSecond = 1;
unsigned long CurrentSecond = 0;

HX711 scale;
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address, if it's not working try 0x27.
Button myBtn(BUTTON_PIN);
RunningAverage myRA(5);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  myBtn.begin();              // initialize the button object
  lcd.begin(16, 2);  // iInit the LCD for 16 chars 2 lines
  lcd.backlight();   // Turn on the backligt (try lcd.noBaklight() to turn it off)
  lcd.setCursor(0, 0); //First line
  lcd.print("   CoffeeCalc   ");
  delay(500);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor);
  scale.tare();
  lcd.clear();
  tone(BUZZER, NOTE_GS7, 100);
}

void loop() {
  // put your main code here, to run repeatedly:
  //  Serial.print(scale.get_units()*453.592, 2); // in gr
  myBtn.read();
  if (myBtn.wasReleased()){
    scale.tare();
    tone(BUZZER, NOTE_CS7, 100);
    lcd.clear();
  }
  CoffeeMass = scale.get_units() * 453.592;
  myRA.addValue(CoffeeMass);
  CoffeeMass = myRA.getAverage();
  lcd.setCursor(0, 0);
  lcd.print(String(CoffeeMass,1)+" gr   ");
//  CoffeeMass = (float)scale.get_units() * 453.592, 1;
//  sprintf(data, dtostrf("%f\0" ), CoffeeMass);
//  Serial.println(data);
//  lcd.print(data);
  lcd.setCursor(0, 1);
  
  //STOPWATCH
  CurrentMinute = millis() / 60000;
  if(CurrentMinute > PreviousMinute)
    PreviousSecond = 0;
  lcd.print(CurrentMinute);
  lcd.print("m:");
  // print the number of seconds since reset
  CurrentSecond = millis() / 1000;
  lcd.print(PreviousSecond);
  lcd.print("s");
}
