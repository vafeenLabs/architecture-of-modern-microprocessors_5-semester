#include <Arduino.h>
#include "TM1637Display.h"
#include "DHT.h"

// Пины подключения для дисплея и датчика
#define CLK 50  // Пин для CLK TM1637
#define DIO 52  // Пин для DIO TM1637
#define DHTPIN 48 // Пин для датчика DHT11
#define DHTTYPE DHT11 // Тип датчика DHT11

// Пауза между переключениями данных на дисплее
#define DISPLAY_DELAY 2000

// Инициализация дисплея и датчика
TM1637Display display(CLK, DIO);
DHT dht(DHTPIN, DHTTYPE);

// Кодировка сегментов для влажности и температуры
const uint8_t HUMIDITY_SEG0 = SEG_A | SEG_F | SEG_B | SEG_G | SEG_D; // AFBGD
const uint8_t HUMIDITY_SEG1 = SEG_A | SEG_E | SEG_G | SEG_C | SEG_D; // AEGCD

const uint8_t TEMPERATURE_SEG0 = SEG_F | SEG_A | SEG_G | SEG_E;      // FAGE
const uint8_t TEMPERATURE_SEG1 = SEG_A | SEG_F | SEG_B | SEG_G;      // AFBG

void setup() {
    Serial.begin(9600); // Инициализация серийного порта для отладки
    display.setBrightness(0x0f); // Максимальная яркость дисплея
    dht.begin(); // Инициализация датчика DHT11
}

void loop() {
    // Чтение влажности и температуры
    float humidity = dht.readHumidity();
    float temperatureC = dht.readTemperature(); // Температура в Цельсиях
    float temperatureF = temperatureC * 9.0 / 5.0 + 32.0; // Перевод температуры в Фаренгейты

    // Проверка на ошибки при чтении с датчика
    if (isnan(humidity) || isnan(temperatureC)) {
        Serial.println("Не удалось считать данные с датчика DHT11");
        display.showNumberDec(9999, false);  // Если ошибка, выводим 9999 на дисплей
    } else {
        // Вывод данных в консоль
        Serial.print("Влажность: ");
        Serial.print(humidity);
        Serial.print("%\t");
        Serial.print("Температура: ");
        Serial.print(temperatureC);
        Serial.println("°C");

        // Отображение температуры на дисплее
        uint8_t tempSegments[] = {TEMPERATURE_SEG0, TEMPERATURE_SEG1, display.encodeDigit((int)temperatureF / 10), display.encodeDigit((int)temperatureF % 10)};
        display.setSegments(tempSegments); // Выводим температуру
        delay(DISPLAY_DELAY);  // Пауза между выводом

        // Отображение влажности на дисплее
        uint8_t humSegments[] = {HUMIDITY_SEG0, HUMIDITY_SEG1, display.encodeDigit((int)humidity / 10), display.encodeDigit((int)humidity % 10)};
        display.setSegments(humSegments); // Выводим влажность
        delay(DISPLAY_DELAY); // Пауза перед следующим циклом
    }
}