/*
Дмитрий Осипов. http://www.youtube.com/user/d36073?feature=watch
v.01 Arduino регистратор SD card read write DS1302 Real Time DHT11 thermometer
Version 0.1 2014/07/22
Сохранение на карту памяти, данных от датчиков, с фиксацией времени.
То есть, делаем датчик регистратор.
Что нам понадобится.
1) Arduino.
2) SD Card Memory Shield Module SPI.
3) Часы для Arduino, RTC DS1302 Real Time Clock Module.
4) Датчик температуры и влажности, temperature sensors DHT11 thermometer.
-) Скачать 2 Библиотеки, "DS1302" и "DHT".
----------
Принцип работы:
При обнаружении изменения температуры на +- 1 градус, записываем на SD Card
текущую дату и время и показания с датчика температуры и влажности.
Для просмотра данных записанных в файл на SD Card,
надо отправить через Serial монитор цифру 1.
Количество и типы подключённых датчиков можно увеличить.
*Примечание, опытным путём обнаружил сбои записи даты и времени.
Решение - не подключайте +5v к "DS1302 Real Time Clock Module",
достаточно только "-" / землю.
Скачать sketch.
v.01 Arduino регистратор SD card read write DS1302 Real Time DHT11 thermometer
----------------------------
Подробную видео инструкцию выложу здесь.
v.01 Arduino регистратор SD card read write DS1302 Real Time DHT11 thermometer
============================================
***************************************************************
Вспомогательные видео инструкции, sketch, программы, библиотеки.
Библиотеки:
Скачать библиотеку "DS1302" для RTC DS1302 Real Time Clock Module.
https://yadi.sk/d/xMK-iHMeWzgQ8
Скачать библиотеку "DHT" для temperature sensors DHT11.
http://yadi.sk/d/L5ooqNXVJJM8u
--------------
Видео инструкции:
ч.3/3 Arduino ANDROID Bluetooth термометр temperature sensors DHT11 thermometer
https://www.youtube.com/watch?v=t3wWsTcDS44&list=UU7aH7HVqDvwB1xNHfSl-fDw
Часы для Arduino RTC DS1302 Real Time Clock Module library "DS1302"
https://www.youtube.com/watch?v=jG0sbB4tD04&list=UU7aH7HVqDvwB1xNHfSl-fDw
Arduino SD карта тест Storage Board Mciro SD TF Card Memory Shield Module SPI
https://www.youtube.com/watch?v=NDxK7uF8tVc&list=UU7aH7HVqDvwB1xNHfSl-fDw
***************************************************************
Arduino UNO PINS.
* SD card attached to SPI bus as follows:
PINS для SD Card Module.
** MOSI - pin 11
** MISO - pin 12
** CLK/SCK - pin 13
** CS - pin 10 Можно изменять.
*/
#include <SD.h> // библиотека для SD Card.
#include <DS1302.h> // это скачанная библиотека для DS1302 Real Time Clock Module.
#include "DHT.h" // это скачанная библиотека для temperature sensors DHT11.
// ------------------------
// PINS для DS1302 Real Time Clock Module.
// ** RST - pin 2
// ** DAT - pin 3
// ** CLK - pin 4
// ! +5v не подключаем достаточно только "-" / землю.
DS1302 rtc(2, 3, 4);
// ------------------------
#define DHTPIN 7 // PIN №7 для датчика температуры и влажности DHT 11.
#define DHTTYPE DHT11 // DHT 11.
DHT dht(DHTPIN, DHTTYPE);
// ------------------------
File myFile; // SD card.
int SaveTemperature; // Для фиксации температуры.
// ------------------------
void setup()
{
  Serial.begin(9600);
  dht.begin(); // Включаем библиотеку "DHT".
  SaveTemperature = dht.readTemperature(); // Фиксируем текущую температуру.
  // ----------------
  // Тестируем SD card на работоспособность.
  Serial.print("Initializing SD card...");
  if (!SD.begin(10)) // Здесь можно изменить № pin ** CS - pin 10.
  {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  // ----------------
  SDcardWrite(); // Запускаем функцию записи на SD card.
}
// ***************************************
void SDcardWrite() // Делаем функцию, для записи на SD card.
{
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  // Создаём новый, или открываем существующий файл на SD card,
  // например с именем dimasens.txt
  // максимум "8" символов может иметь имя файла 12345678.123
  myFile = SD.open("dimasens.txt", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to dimasens.txt...");
    // начинаем записывать на SD card, строку в файл dimasens.txt
    myFile.print(rtc.getDateStr()); // Текущую дату.
    myFile.print(" -- ");
    myFile.print(rtc.getTimeStr()); // текущие время.
    myFile.print(" -- ");
    myFile.print("Humidity: ");
    myFile.print(dht.readHumidity()); // Значения влажности.
    myFile.print(" %\t");
    myFile.print("Temperature: ");
    myFile.print(dht.readTemperature()); // Значения температуры.
    myFile.println(" *C");
    myFile.close(); // close the file:
    Serial.println("done.");
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening dimasens.txt");
  }
}
// -------------------
// Делаем функцию для чтения с SD card и вывода в Serial монитор.
void SDcardRead()
{
  // re-open the file for reading:
  myFile = SD.open("dimasens.txt");
  if (myFile) {
    Serial.println("dimasens.txt:");
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening dimasens.txt");
  }
}
// ************************************
void loop()
{
  // Для того чтоб бессмысленно не записывать на SD card данные с датчиков,
  // сделаем проверку.
  // Только если прошлое значение температуры не равно текущему значению,
  // то есть произошло изменение температуры.
  if (SaveTemperature != dht.readTemperature())
    // Можно и по-другому, увеличивая порог, обнаружения изменения температуры.
    // if (SaveTemperature >= (dht.readTemperature()+2) || SaveTemperature <= (dht.readTemperature()-2))
  {
    // Serial.println(rtc.getDateStr());
    // Serial.println(dht.readHumidity());
    // Serial.println(dht.readTemperature());
    SDcardWrite(); // Запускаем функцию записи на SD card.
    // Снова фиксируем текущее значение температуры.
    SaveTemperature = dht.readTemperature();
  }
  // Для вывода в Serial монитор данных с SD card.
  if (Serial.available() > 0) {
    // Если отправляем через Serial монитор цифру 1
    // запускаем функцию SDcardRead.
    // То есть выводим в Serial монитор всё содержимое файла dimasens.txt
    if (Serial.read() - 48 == 1)SDcardRead();
  }
}
