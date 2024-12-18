#include <Wire.h>
#include <avr/interrupt.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

const unsigned long DEBOUNCE_DELAY = 50;

class KeyPad {
private:
    uint8_t rows;
    uint8_t columns;
    char keys[4][4];
    uint8_t* pin_rows;
    uint8_t* pin_column;
    uint8_t last_column_pin;
    uint8_t last_row_pin;
    unsigned long lastDebounceTime;
    char lastKey;

public:
    KeyPad(char _keys[4][4], uint8_t* _pin_rows, uint8_t* _pin_column, uint8_t _rows, uint8_t _columns);

    ~KeyPad();

    char getKey();

};

