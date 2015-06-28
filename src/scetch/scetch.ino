#include <SPI.h>
#include <Wire.h>
#include <OLED_I2C.h>
#include "RTClib.h"
#include <DHT11.h>

#define OLED_RESET 4
#define DHT11pin 2
RTC_DS1307 rtc;
DHT11 dht11(DHT11pin);

OLED  myOLED(SDA, SCL, 8);
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];

void setup()   {
  Serial.begin(9600);
  Wire.begin();

  myOLED.begin();
  myOLED.setFont(SmallFont);

  // Splash screen, version, date
  display.setCursor(0, 20);
  display.setTextSize(3);
  display.print("eSTROPS");
  display.setCursor(10, 45);
  display.setTextSize(1);
  display.print("v0.0.1 (07.06.2015)");
  display.display();
  delay(5000);
  display.clearDisplay();

  // Reading devices, printout state
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.println("Setup:");
  display.display();
  display.setTextSize(1);
  
  
  // RTC - check!
  display.print("RTC - ");
  if ( rtc.isrunning() ) {
  display.println("Ok! ");
  } else {
    display.println("Error! ");  
  }
  display.display();

  // DHT11 - check!
  display.print("DHT11 - ");
  int err;
  float temp, humi;
  if((err=dht11.read(humi, temp))==0) {
  display.println("Ok! ");
  } else {
    display.print("Error! No: ");
    display.println(err);  
  }
  display.display();

  
  // SD Card - Check!
  
  delay(2000);
  display.clearDisplay();

}

void loop() {

  DateTime now = rtc.now();

  Serial.print(now.day(), DEC);
  Serial.print('.');
  Serial.print(now.month(), DEC);
  Serial.print('.');
  Serial.print(now.year(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);

  display.print(now.day(), DEC);
  display.print('.');
  display.print(now.month(), DEC);
  display.print('.');
  display.print(now.year(), DEC);
  display.println(' ');
  display.print(now.hour(), DEC);
  display.print(':');
  display.print(now.minute(), DEC);
  display.print(':');
  display.print(now.second(), DEC);
  display.println();
  
  display.setTextSize(1);
  display.setCursor(0, 20);

  int err;
  float temp, humi;
  if((err=dht11.read(humi, temp))==0) {

  display.print("temp  C : ");
  display.println(temp);
  display.print("humid % : ");
  display.println(humi);
  }

  display.display();


  delay(1000);

}
