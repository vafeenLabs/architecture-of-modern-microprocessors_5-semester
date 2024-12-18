#include <TM1637Display.h>
// 1 лаба Вариант 111

#define CLK_PIN 2
#define DIO_PIN 3

TM1637Display display(CLK_PIN, DIO_PIN);

#define LIT_D SEG_B | SEG_C | SEG_D | SEG_E | SEG_G
#define MINUS SEG_G

const uint8_t LETTER_H[] = {0b01110110};
const uint8_t LETTER_E[] = {0b01111001};
const uint8_t LETTER_L[] = {0b00111000};
const uint8_t LETTER_O[] = {0b00111111};
const uint8_t LETTER_N[] = {0b01010100};
const uint8_t LETTER_D[] = {0b00011110};
const uint8_t LETTER_U[] = {0b01111100};
const uint8_t LETTER_F[] = {0b01011100};
const uint8_t LETTER_Q[] = {0b01101111};
const uint8_t LETTER_A[] = {0b01110111};
const uint8_t LETTER_C[] = {0b00111001};
const uint8_t LETTER_K[] = {0b01100110};
const uint8_t LETTER_P[] = {0b01110011};
const uint8_t LETTER_T[] = {0b01111000};
const uint8_t LETTER_R[] = {0b01010000};
const uint8_t LETTER_I[] = {0b00010000};

const uint8_t minus[] = {MINUS};

void setup() {
  display.setBrightness(7); // Установка яркости дисплея (от 0 до 7)
}

void loop() {
  group1_variant1();
  group2_variant1();
  group3_variant1();
}

// Группа 1 - Вариант 1: вывод чисел от -100 до 100 по середине экрана без ведущих нулей с шагом 1 и паузой 0.15 секунд
void group1_variant1() {
  display.clear();
  for (int i = -100; i <= 100; i++) {
    display.showNumberDecEx(i, 0, false, 3, 0); // Вывод числа без ведущих нулей по середине экрана
    delay(150);
  }
}

// Группа 2 - Вариант 1: вывод вещественных чисел от -2 до 2 с шагом 0.2 и паузой 1 секунда
void group2_variant1() {

  float value = -2.0;
  display.clear();
  bool fMinus=false;
  while (round(value*100) <= 200) {
    int intValue = round(value * 100.0); // Округляем значение до целого числа соответствующего шагу 0.2

    if (intValue < 0 && !fMinus) {
        display.setSegments((minus+0));
        fMinus = true;
    }
    else if(intValue >= 0 && fMinus)
    {
      display.clear();
    }
    display.showNumberDecEx(abs(intValue), 0b10000000, true, 3, 1); // Выводим значение на дисплей

    delay(1000); // Пауза на 1 секунду

    value += 0.2;
  }
  
}

// Группа 3 - Вариант 1: вывод текста "hello node" с задержкой 1 секунда и сдвигом на 1 символ раз в 1 секунду
void group3_variant1() {
  const uint8_t helloNode[] = {0x0, 0x0, 0x0, 0x0, LETTER_H[0], LETTER_E[0], LETTER_L[0], LETTER_L[0], LETTER_O[0], 0x00, LETTER_N[0], LETTER_O[0], LIT_D/*LETTER_D[0]*/, LETTER_E[0], 0x0, 0x0, 0x0};
  int textLength = sizeof(helloNode) / sizeof(helloNode[0]);

  while (true) {
    for (int i = 0; i <= textLength - 4; i++) {
      display.setSegments((helloNode + i)); // Вывод одного символа текста
      delay(1000);
    }
  }
}
