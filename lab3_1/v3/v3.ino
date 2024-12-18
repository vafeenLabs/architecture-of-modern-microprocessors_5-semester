#include "KeyPad.h"
#include <Wire.h>
#include <avr/interrupt.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

#define RESULT_ADDRESS_F 0
#define RESULT_ADDRESS 4
const byte ROW_NUM = 4;
const byte COLUMN_NUM = 4;

int menuState = 1;

int memoryValue = 0;
int memoryValue2 = 0;
int deletedValue1 = 0;
int deletedValue2 = 0;
int result = 0;
int deletedValue = 0;
bool v=false;

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pin_rows[ROW_NUM] = {2, 3, 4, 9};             
byte pin_column[COLUMN_NUM] = {6, 7, 8, 18};  

KeyPad keypad((keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);
const byte interruptPin = pin_column[3];
const byte interruptPinA = pin_rows[0];
const byte interruptPinD = pin_rows[3];
const byte interruptPinB = pin_rows[1];
volatile char Interrupt = ' ';

LiquidCrystal_I2C lcd(0x27, 2, 16);
volatile bool interruptFlag = true;
void handleInterrupt() {
  if (!interruptFlag)
  {
    interruptFlag = true;
    char key = keypad.getKey();
    if (key == 'A') {
      Interrupt = 'A';
      Serial.print("I-A");
    } else if (key == 'D') {
      Interrupt = 'D';
      Serial.print("I-D");
    } else if (key == 'B') {
      Interrupt = 'B';
      Serial.print("I-B");
    }
    //interruptFlag = false;
  }
}

void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);
  uint8_t val = EEPROM.read(RESULT_ADDRESS_F);
  if (val != 255) {
  EEPROM.get(RESULT_ADDRESS, result);
  }
  Wire.begin();
  
  lcd.init();
  lcd.backlight();
  Serial.print("HI");
  updateMainMenuState();
  
  Interrupt = 'B';
}
void processMenu()
{
  if (interruptFlag)  {
    interruptFlag = false;
    switch (Interrupt) {
      case 'A':
        menuState += (menuState > 1)?-1:0;
        updateMainMenuState();
      
        break;

      case 'D':
        menuState += (menuState < 3)?1:0;
        updateMainMenuState();
        
        break;

      case 'B':
        updateMenuState();
        switch (menuState) {
          case 1:
            processMenu1();
            break;
          case 3:
            processMenu3();
            break;
        }
        updateMainMenuState();
        break;
    }
    Interrupt = ' ';
  }
}

void loop() {
  processMenu();
}

void changePrintValue(bool v) {
  lcd.setCursor(6, 0);
  lcd.print(v ? "2" : "1");
}
void printCurValue()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Value  :");
  changePrintValue(v);
  lcd.setCursor(0, 1);
  lcd.print(memoryValue);
}
void processMenu1() 
{
  printCurValue();
  
  char key = '\0';
  while(key != 'B')
  {
    detachInterrupt(digitalPinToInterrupt(interruptPin));
    key = keypad.getKey(); 
    attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);
    
     switch (key) {
      case '0'...'9':
        memoryValue = memoryValue * 10 + (key - '0');
        lcd.setCursor(0, 1);
        lcd.print(memoryValue);
        break;
      case '#':
        resetMemoryValues();
        printCurValue();
        break;
      case '*':
        calculateResult();
        resetMemoryValues();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Result:");
        lcd.setCursor(0, 1);
        lcd.print(result);
        delay(1500);
        printCurValue();
        break;
      case 'A':
        swapMemoryValues();
        v = !v;
        printCurValue();
        break;
      case 'C':
        saveResultToMemory();
        displaySaveMessage();
        delay(600);
        printCurValue();
        break;
    }
  }
  Interrupt = ' ';
  updateMainMenuState();
}

void processMenu3() 
{
  char key = '\0';
  while(key != 'B')
  {
    detachInterrupt(digitalPinToInterrupt(interruptPin));
    key = keypad.getKey();
    attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);
    switch (key) {
      case '#':
        deleteMemoryValues();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(String(menuState) + ":");
        lcd.setCursor(0, 1);
        lcd.print("Done");
        delay(1000);
        break;
      case 'C':
        undoMemoryValues();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(String(menuState) + ":");
        lcd.setCursor(0, 1);
        lcd.print("Canceled");
        delay(1000);
        break;
    }
  }
  Interrupt = ' ';
  updateMainMenuState();
}

void updateMainMenuState() 
{
  lcd.clear();
  
  if (menuState == 2) {
    lcd.setCursor(0, 0);
    lcd.print("Menu "+String(menuState) + ":");
    int printV1=(!v)?memoryValue:memoryValue2;
    int printV2=(!v)?memoryValue2:memoryValue;
    lcd.setCursor(0, 1);
    lcd.print("F");
    lcd.setCursor(1, 1);
    lcd.print(printV1);

    lcd.setCursor(5, 1);
    lcd.print("S");
    lcd.setCursor(6, 1);
    lcd.print(printV2);

    lcd.setCursor(10, 1);
    lcd.print("R:");
    lcd.setCursor(12, 1);
    lcd.print(result);
    handleInterrupt();
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Menu "+String(menuState) + ":");
    lcd.setCursor(0, 1);
    lcd.print("B-Enter/Exit");
    handleInterrupt();
  }
}

void updateMenuState() 
{
  lcd.clear();
  
  if (menuState == 1) {
    lcd.setCursor(17, 0);
    lcd.print("C-save result");
    lcd.setCursor(17, 1);
    lcd.print("A-save value ");
    displayScrollingText("   C-save result");
    delay(2000);
  }
  else if (menuState == 3) {
    lcd.setCursor(15, 0);
    lcd.print("     Wanna reset?   ");
    lcd.setCursor(15, 1);
    lcd.print("     #-Yes, C-Cancel");
    displayScrollingText("menu 3: wanna reset?");
  }
}

void resetMemoryValues() {
  memoryValue = 0;
  memoryValue2 = 0;
}

void calculateResult() {
  result = memoryValue | memoryValue2;
}

void swapMemoryValues() {
  int tmp = memoryValue;
  memoryValue = memoryValue2;
  memoryValue2 = tmp;
}

void saveResultToMemory() {
  EEPROM.put(RESULT_ADDRESS, result);
  EEPROM.put(RESULT_ADDRESS_F, 0);
}

void displaySaveMessage() {
  lcd.clear();
  lcd.setCursor(17, 1);
  lcd.print("result " + String(result) + " saved");
  
  String message = "result      savedresult";
  displayScrollingText(message);
  delay(100);
}

void deleteMemoryValues() {
  deletedValue = result;
  deletedValue1 = memoryValue;
  deletedValue2 = memoryValue2;
  memoryValue=0;
  memoryValue2=0;
  result = 0;
  EEPROM.update(RESULT_ADDRESS_F, uint8_t(0xFF));
}

void undoMemoryValues() {
  memoryValue = deletedValue1;
  memoryValue2 = deletedValue2;
  result = deletedValue;
  saveResultToMemory();
}

void displayScrollingText(String text) {
  for (int i = 0; i < text.length(); i++) {
    delay(50);
    lcd.scrollDisplayLeft();
  }
}
