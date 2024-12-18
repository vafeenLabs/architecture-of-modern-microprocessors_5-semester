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
#define SYMBOL_DEGREE SEG_G | SEG_F | SEG_A | SEG_B
#define SYMBOL_PERCENT_START SEG_G | SEG_F | SEG_A | SEG_B | SEG_C
#define SYMBOL_PERCENT_END SEG_C | SEG_F | SEG_D | SEG_E | SEG_G

void setup() {
  display.setBrightness(5); // Настройка яркости дисплея
  dht.begin(); // Инициализация датчика DHT11
}

void loop() {
  float temperatureC = readTemperature();
  float temperatureF = convertToFahrenheit(temperatureC);
  float humidity = readHumidity();

  displayData(temperatureF, humidity);
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
  int humInt = int(humidity); // Целая часть влажности
  
  display.showNumberDec(tempInt, false, 4, 0); 
  uint8_t segmentsF[] = {LETTER_F, SYMBOL_DEGREE};
  display.setSegments(segmentsF, sizeof(segmentsF));
  delay(2000); // Пауза 2 секунды
  display.clear(); // Очистка дисплея

  display.showNumberDec(humInt, false, 4, 0); 
  uint8_t segmentsH[] = {SYMBOL_PERCENT_START, SYMBOL_PERCENT_END};
  display.setSegments(segmentsH, sizeof(segmentsH));
  delay(2000); // Пауза 2 секунды
}
