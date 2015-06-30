#include "HX711.h"
// HX711.DOUT	- pin #A1
// HX711.PD_SCK	- pin #A0
HX711 scale(A1, A0);		// parameter "gain" is ommited; the default value 128 is used by the library

#include "RTClib.h"
RTC_DS1307 rtc;

#include <Wire.h> 
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR 0x27
#define BACKLIGHT_PIN 3
#define En_pin 2
#define Rw_pin 1
#define Rs_pin 0
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7
LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin,BACKLIGHT_PIN,POSITIVE); 

// LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
LCD *myLCD = &lcd;

float adc_units;
float adc_units10;


void setup() {
  Serial.begin(9600);

  rtc.begin();
  if (! rtc.isrunning()) { Serial.println("RTC is NOT running!"); }
  
  lcd.begin(16,2);
  lcd.home();
    
  lcd.println(" eSTROPS starts ");
  Serial.println("eSTROPS starts");
  LCD_progress_bar (1, 10, 0, 160);
  for (int i=2; i<=6; i++){
    LCD_progress_bar (1, i*10, 0, 160);
    delay(1000);
  }
  
  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t\t");
  Serial.println(scale.read());
  LCD_progress_bar (1, 70, 0, 160);

  Serial.print("read average (20): \t");
  Serial.println(scale.read_average(20));  	// print the average of 20 readings from the ADC
  LCD_progress_bar (1, 80, 0, 160);

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(20));		// print the average of 20 readings from the ADC minus the tare weight (not set yet)
  LCD_progress_bar (1, 90, 0, 160);

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(20), 1);	// print the average of 20 readings from the ADC minus tare weight (not set) divided 
						// by the SCALE parameter (not set yet)
  LCD_progress_bar (1, 100, 0, 160);  

  scale.set_scale(23800.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  // scale.set_scale();
  LCD_progress_bar (1, 110, 0, 160);
  
  scale.tare(10);				        // reset the scale to 0
  LCD_progress_bar (1, 120, 0, 160);

  Serial.println("After setting up the scale:");

  Serial.print("read: \t\t\t");
  Serial.println(scale.read());                 // print a raw reading from the ADC
  LCD_progress_bar (1, 130, 0, 160);

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC
  LCD_progress_bar (1, 140, 0, 160);

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(20));		// print the average of 5 readings from the ADC minus the tare weight, set with tare()
  LCD_progress_bar (1, 150, 0, 160);

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(20), 1);        // print the average of 5 readings from the ADC minus tare weight, divided 
						// by the SCALE parameter set with set_scale
  LCD_progress_bar (1, 160, 0, 160);            

  Serial.println("Readings:");
  lcd.clear();
}

void loop() {
  DateTime now = rtc.now();
  //String str_DateTime = if (now.day()<10) { "0" } + String(now.day(), DEC);
  lcd.print(now.day(), DEC);
  lcd.print('.');
  if (now.month()<10) { lcd.print("0"); }
  lcd.print(now.month(), DEC);
  lcd.print('.');
  lcd.print(now.year(), DEC);
  lcd.print(' ');
  if (now.hour()<10) { lcd.print("0"); }
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  if (now.minute()<10) { lcd.print("0"); }
  lcd.print(now.minute(), DEC);
  lcd.setCursor(0,0);
  //lcd.print("30.06.2015 23:21");
  if (now.day()<10) { lcd.print("0"); }
  lcd.print(now.day(), DEC);
  lcd.print('.');
  if (now.month()<10) { lcd.print("0"); }
  lcd.print(now.month(), DEC);
  lcd.print('.');
  lcd.print(now.year(), DEC);
  lcd.print(' ');
  if (now.hour()<10) { lcd.print("0"); }
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  if (now.minute()<10) { lcd.print("0"); }
  lcd.print(now.minute(), DEC);

  lcd.setCursor(0,1);
  adc_units = scale.get_units();
  adc_units10 = scale.get_units(10);
  
  lcd.print("  0.6");
  lcd.print("kg ");
  lcd.print("-24");
  lcd.print((char)223);
  lcd.print(" 67");
  lcd.print("%");
  Serial.print("one reading:\t");
  Serial.print(adc_units);
  Serial.print("\t| average:\t");
  Serial.println(adc_units10);

  scale.power_down();			        // put the ADC in sleep mode
  delay(3000);
  scale.power_up();
  lcd.clear();
}

void LCD_progress_bar (int row, int var, int minVal, int maxVal)
{
  int block = map(var, minVal, maxVal, 0, 16);  // Block represent the current LCD space (modify the map setting to fit your LCD)
  int line = map(var, minVal, maxVal, 0, 80);   // Line represent the theoretical lines that should be printed 
  int bar = (line-(block*5));                   // Bar represent the actual lines that will be printed
  
  /* LCD Progress Bar Characters, create your custom bars */

  byte bar1[8] = { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10};
  byte bar2[8] = { 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18};
  byte bar3[8] = { 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C};
  byte bar4[8] = { 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E};
  byte bar5[8] = { 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F};
  lcd.createChar(1, bar1);
  lcd.createChar(2, bar2);
  lcd.createChar(3, bar3);
  lcd.createChar(4, bar4);
  lcd.createChar(5, bar5);
  
  for (int x = 0; x < block; x++)                        // Print all the filled blocks
  {
    lcd.setCursor (x, row);
    lcd.write (1023);
  }
  
  lcd.setCursor (block, row);                            // Set the cursor at the current block and print the numbers of line needed
  if (bar != 0) lcd.write (bar);
  if (block == 0 && line == 0) lcd.write (1022);   // Unless there is nothing to print, in this case show blank
  
  for (int x = 16; x > block; x--)                       // Print all the blank blocks
  {
    lcd.setCursor (x, row);
    lcd.write (1022);
  }
}
