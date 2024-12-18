#include <Arduino.h>
#include <TM1637Display.h>
#include <DHT.h>

#define CLK_PIN 2 // Пин клока подключается к пину D2 на Arduino
#define DIO_PIN 3 // Пин данных подключается к пину D3 на Arduino

#define DHT_PIN 4 // Пин датчика DHT11 подключается к пину D4 на Arduino
#define DHT_TYPE DHT11 // Тип датчика DHT11

TM1637Display display(CLK_PIN, DIO_PIN);
DHT dht(DHT_PIN, DHT_TYPE);

#define SEG_A 0b00000001
#define SEG_B 0b00000010
#define SEG_C 0b00000100
#define SEG_D 0b00001000
#define SEG_E 0b00010000
#define SEG_F 0b00100000
#define SEG_G 0b01000000
#define SEG_DP 0b10000000

#define LETTER_H SEG_E | SEG_F | SEG_G | SEG_B | SEG_C
#define LETTER_F SEG_A | SEG_E| SEG_F | SEG_G

void setup() {
  display.setBrightness(5); // Настройка яркости дисплея
  dht.begin(); // Инициализация датчика DHT11
}

void loop() {
  float temperatureC = readTemperature();
  float temperatureF = convertToFahrenheit(temperatureC);
  float humidity = readHumidity();

  displayData(temperatureF, humidity);
  delay(2000); // Пауза 2 секунды
}

float readTemperature() {
  float temperature = dht.readTemperature();
  return temperature;
}

float readHumidity() {
  float humidity = dht.readHumidity();
  return humidity;
}

float convertToFahrenheit(float temperatureC) {
  float temperatureF = temperatureC * 1.8 + 32; // Преобразование температуры в фаренгейты
  return temperatureF;
}

void displayData(float temperature, float humidity) {
  int tempInt = int(temperature); // Целая часть температуры
  int tempDec = int((temperature - tempInt) * 100); // Десятичная часть температуры
  int humInt = int(humidity); // Целая часть влажности
  int humDec = int((humidity - humInt) * 100); // Десятичная часть влажности

  int tempDisplay = tempInt * 100 + tempDec; // Объединяем целую и десятичную части температуры в одно число
  display.showNumberDec(tempDisplay, false, 4, 0); // Выводим число соединенных цифр на дисплей
  display.showNumberDecEx(tempDisplay, (0b00000001 << 7), true, 4, 0); // Выводим число соединенных цифр на дисплей с флагом десятичной точки
  delay(2000); // Пауза 2 секунды
  display.clear(); // Очистка дисплея


  uint8_t segmentsF[] = {LETTER_F};
  display.setSegments(segmentsF, sizeof(segmentsF));
  delay(2000); // Пауза 2 секунды
  display.clear(); // Очистка дисплея

  int humDisplay = humInt * 100 + humDec; // Объединяем целую и десятичную части влажности в одно число
  display.showNumberDec(humDisplay, false, 4, 0); // Выводим число соединенных цифр на дисплей
  display.showNumberDecEx(humDisplay, (0b00000001 << 7), true, 4, 0); // Выводим число соединенных цифр на дисплей с флагом десятичной точки
  delay(2000); // Пауза 2 секунды
  display.clear(); // Очистка дисплея

uint8_t segmentsH[] = {LETTER_H};
display.setSegments(segmentsH, sizeof(segmentsH));
delay(2000); // Пауза 2 секунды
}
