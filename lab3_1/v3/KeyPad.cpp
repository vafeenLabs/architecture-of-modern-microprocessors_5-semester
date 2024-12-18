#pragma once
#include "KeyPad.h"
#include <Wire.h>
#include <avr/interrupt.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#import <Arduino.h>

KeyPad::KeyPad(char _keys[4][4], uint8_t* _pin_rows, uint8_t* _pin_column, uint8_t _rows, uint8_t _columns) : rows(_rows), columns(_columns), last_row_pin(rows - 1), last_column_pin(columns - 1),
lastDebounceTime(0), lastKey('\0') {

    for (uint8_t i = 0; i < rows; i++) {
        for (uint8_t j = 0; j < columns; j++) {
            keys[i][j] = _keys[i][j];
        }
    }

    pin_rows = new uint8_t[rows];
    for (uint8_t row = 0; row < rows; row++) {
        pin_rows[row] = _pin_rows[row];
        pinMode(pin_rows[row], OUTPUT);
        digitalWrite(pin_rows[row], LOW);
    }

    pin_column = new uint8_t[columns];
    for (uint8_t column = 0; column < columns; column++) {
        pin_column[column] = _pin_column[column];
        pinMode(pin_column[column], INPUT_PULLUP);
    }

}

    
KeyPad::~KeyPad() {
    delete[] pin_rows;
    delete[] pin_column;
}

char KeyPad::getKey() {
    //noInterrupts();

    for (uint8_t column = 0; column < columns; column++) {
        pinMode(pin_column[column], OUTPUT);
        digitalWrite(pin_column[column], LOW);

        for (uint8_t row = 0; row < rows; row++) {
            pinMode(pin_rows[row], INPUT_PULLUP);
            delayMicroseconds(10);

            if (digitalRead(pin_rows[row]) == LOW) {
                delay(100);
                // Проверяем состояние кнопки еще раз, чтобы исключить ложное срабатывание
                if (digitalRead(pin_rows[row]) == LOW) {
                    pinMode(pin_column[column], INPUT_PULLUP);
                    char key = keys[row][column];
                    delay(50);

                    // Обрабатываем дребезг контактов кнопки
                    unsigned long currentTime = millis();
                    if (key != lastKey && (currentTime - lastDebounceTime) > DEBOUNCE_DELAY) {
                        lastDebounceTime = currentTime;
                        lastKey = key;
                        Serial.println("Key pressed: " + String(key));
                        return key;
                    }
                    /*else {
                      Serial.println("Key ignored: " + String(key));
                    }*/
                }
            }

            pinMode(pin_rows[row], OUTPUT);
            digitalWrite(pin_rows[row], LOW);
        }

        pinMode(pin_column[column], INPUT_PULLUP);
    }
    //interrupts();
    lastKey = '\0';// Сбрасываем последнюю нажатую кнопку
    return '\0';
}