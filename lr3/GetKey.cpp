#include "GetKey.h"

const long timeOffset = 300;
int rowPins[4];
int colPins[4];
char mapKeys[4][4] = { {'0', '1', '2', '&'},
                       {'3', '4', '5', '|'},
                       {'6', '7', '\0', '^'},
                       {'\0', 's', '=', '>'} };

void setKeypadPins(int rows[4], int cols[4]) {
    for (int i = 0; i < 4; i++) {
        rowPins[i] = rows[i];
        colPins[i] = cols[i];
        pinMode(rowPins[i], 1);
        digitalWrite(rowPins[i], HIGH);
        pinMode(colPins[i], INPUT_PULLUP);
    };
};

char getActivityKey() {
    char valuePressedKey;
    static long now = 0;
    long timeDifference = millis() - now;

    for (int r = 0; r < 4; r++) {
        digitalWrite(rowPins[r], LOW);
        for (int c = 0; c < 4; c++) {
            if (!digitalRead(colPins[c]) && timeDifference > timeOffset) {
                valuePressedKey = mapKeys[r][c];
                now = millis();
            } else {
                valuePressedKey = '\0';
            }
            if (valuePressedKey != '\0') {
                return valuePressedKey;
            };
        };
        digitalWrite(rowPins[r], HIGH);
    }
    return '\0';
};
