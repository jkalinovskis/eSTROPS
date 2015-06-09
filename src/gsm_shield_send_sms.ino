//GSM RX => arduino D8
//GSM TX => arduino D7
//GSM GND (4 pin uz pirma dzeltena skaoties no audio/mic puses) => arduino GND
//Barosana 1A 5V


#include <GPRS_Shield_Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#define PIN_TX    7
#define PIN_RX    8
#define BAUDRATE  9600
#define PHONE_NUMBER "+37128379281"
#define MESSAGE  "sveiciens no GSM shield"

GPRS gprsTest(PIN_TX,PIN_RX,BAUDRATE);//RX,TX,BaudRate

void setup() {
  Serial.begin(9600);
  while(!gprsTest.init()) {
      delay(1000);
      Serial.print("init error\r\n");
  }  
  Serial.println("gprs init success");
  Serial.println("start to send message ...");
  gprsTest.sendSMS(PHONE_NUMBER,MESSAGE); //define phone number and text
}

void loop() {
  //nothing to do
}
