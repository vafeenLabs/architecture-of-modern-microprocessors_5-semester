#include <EEPROM.h>
#include <Arduino.h>
#include "GetKey.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Адрес 0x27 для I2C

String firstOperand;
String secondOperand;
String oper;
String prevOper;
int rows[4] = {2, 3, 4, 5};
int cols[4] = {6, 7, 8, 9};
int hiAddrResult = 0;
int lowAddrResult = 1;
long intFirstOperand = 0;
long intSecondOperand = 0;
long result;
char lastKey = '\0';
static long lastKeyPressed = millis();
byte hi;
byte low; 
int maxLengthString = 6;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  setKeypadPins(rows, cols);
}

void loop() {  
  char keyVal = getActivityKey();

  if (millis() - lastKeyPressed > 500) {
    lastKey = '\0';
  }

  if (keyVal) { 
    lastKeyPressed = millis();
    if (keyVal == '=') {
      calculationStage();
    } else if (keyVal == 's') {
      stageSavedResultOrAutoFillingData();
    } else if (lastKey != keyVal) {
      stageFillingData(keyVal);
      lastKey = keyVal;
    }
  }
}

void calculationStage() {
  long calculatedResult = calculateResult(octalToDecimal(intFirstOperand), octalToDecimal(intSecondOperand));
  if (secondOperand == "") {
    result = intFirstOperand;
    // lcd.clear();
    // lcd.print(intFirstOperand + oper + intSecondOperand + "=" + result);
    Serial.println(intFirstOperand + oper + "=" + result);
  } else {
    result = decimalToOctal(calculatedResult);
    // lcd.clear();
    // lcd.print(intFirstOperand + oper + intSecondOperand + "=" + result);
    Serial.println(intFirstOperand + oper + intSecondOperand + "=" + result);
  }
  prevOper = "=";
  ClearOperands();
}

void stageFillingData(char keyVal) {
  if (keyVal == '&' || keyVal == '|' || keyVal == '^'){
    oper = keyVal;
    maxLengthString = 6;
    // lcd.clear();
    // lcd.print(intFirstOperand + oper);
    Serial.println(intFirstOperand + oper);
  } else if (keyVal == '>') {
    oper = "->";
    maxLengthString = 6;
    // lcd.clear();
    // lcd.print(intFirstOperand + oper);
    Serial.println(intFirstOperand + oper);
  } else if (oper != '\0') {
    if (secondOperand.length() < maxLengthString) {
      if (keyVal == '0' && secondOperand == "0") {
        maxLengthString = 7;
      } else {
        secondOperand.concat(keyVal);
        intSecondOperand = secondOperand.toInt();
      }
      // lcd.clear();
      // lcd.print(intFirstOperand + oper + intSecondOperand);
      Serial.println(intFirstOperand + oper + intSecondOperand);
    }
  } else {
    if (firstOperand.length() < maxLengthString) {
      if (keyVal == '0' && firstOperand == "0") {
        maxLengthString = 7;
      } else {
        firstOperand.concat(keyVal);  
        intFirstOperand = firstOperand.toInt();
      }
      // lcd.clear();
      // lcd.print(intFirstOperand);
      Serial.println(intFirstOperand);
    }
  };
}

void stageSavedResultOrAutoFillingData() {
  if (prevOper == "=") {
    hi = highByte(result);
    low = lowByte(result);
    EEPROM.update(hiAddrResult, hi);
    EEPROM.update(lowAddrResult, low);
    prevOper = "s";
  } else {
    if (firstOperand == "") {
      hi = EEPROM.read(hiAddrResult);
      low = EEPROM.read(lowAddrResult);
      firstOperand = word(hi, low);
      intFirstOperand = firstOperand.toInt();
      // lcd.clear();
      // lcd.print(intFirstOperand);
      Serial.println(intFirstOperand);
    } else if (secondOperand == "") {
      hi = EEPROM.read(hiAddrResult);
      low = EEPROM.read(lowAddrResult);
      secondOperand = word(hi, low);
      intSecondOperand = secondOperand.toInt();
      // lcd.clear();
      // lcd.print(intFirstOperand + oper + intSecondOperand);
      Serial.println(intFirstOperand + oper + intSecondOperand);
    }
  }
}


long octalToDecimal(long octal) {
    long decimal = 0;
    long base = 1;

    while (octal > 0) {
        long lastDigit = octal % 10;
        octal = octal / 10;
        decimal += lastDigit * base;
        base = base * 8;
    }

    return decimal;
}

long decimalToOctal(long decimal) {
    long octal = 0;
    long base = 1;

    while (decimal > 0) {
        long lastDigit = decimal % 8;
        decimal = decimal / 8;
        octal += lastDigit * base;
        base = base * 10;
    }

    return octal;
}

long calculateResult(long fO, long sO) {
  long res;
  switch (oper[0]) {
  case '&':
    res = fO & sO;
    break;
  case '|':
    res = fO | sO;
    break;
  case '^':
    res = fO ^ sO;
    break;
  case '-':
    res = (~fO) | sO;
    break;
  default:
    return -1;
  }
  return res;
}

void ClearOperands() {
  firstOperand = "";
  secondOperand = "";
  oper = "";
  lastKey = '\0';
  intFirstOperand = 0;
  intSecondOperand = 0;
  maxLengthString = 6;
}

