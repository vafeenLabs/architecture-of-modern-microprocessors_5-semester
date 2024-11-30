#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

#define ROWS 4
#define COLS 4

char resultChar = 'R';
char clearChar = 'C';
char keys[ROWS][COLS] = {
    {'1', '2', '3', '+'},
    {'4', '5', '6', '-'},
    {'7', '8', '9', '*'},
    {resultChar, '0', clearChar, '/'}};

int rowPins[ROWS] = {2, 3, 4, 5}; // строки
int colPins[COLS] = {6, 7, 8, 9}; // столбцы

LiquidCrystal_I2C lcd(0x27, 16, 2); // Адрес I2C и размеры экрана

int first = -1;
int second = -1;
char op = '0';
float result = -1.0;
float limit = 1000;
float address = 0;

void writeFloatToEEPROM(float val) // запись в ЕЕПРОМ
{

    byte *x = (byte *)&val;

    for (byte i = 0; i < 4; i++)
        EEPROM.write(i + address, x[i]);
}

float readFloatFromEEPROM() // чтение из ЕЕПРОМ
{
    byte x[4];
    for (byte i = 0; i < 4; i++)
        x[i] = EEPROM.read(i + address);

    float *y = (float *)&x;

    return y[0];
}

void setup()
{
    lcd.begin(16, 2);
    lcd.backlight();

    for (int i = 0; i < ROWS; i++)
    {
        pinMode(rowPins[i], OUTPUT);
        digitalWrite(rowPins[i], HIGH);
    }

    for (int i = 0; i < COLS; i++)
    {
        pinMode(colPins[i], INPUT_PULLUP);
    }

    lcd.print("Start");
    delay(1000);
    lcd.clear();
}

char getKey()
{
    for (int i = 0; i < ROWS; i++)
    {
        digitalWrite(rowPins[i], LOW);

        for (int j = 0; j < COLS; j++)
        {
            if (digitalRead(colPins[j]) == LOW)
            {
                delay(100);
                if (digitalRead(colPins[j]) == LOW)
                {
                    while (digitalRead(colPins[j]) == LOW)
                        ; // Ожидание отпускания кнопки
                    digitalWrite(rowPins[i], HIGH);
                    return keys[i][j];
                }
            }
        }

        digitalWrite(rowPins[i], HIGH);
    }
    return '\0'; // Возврат пустого символа, если ничего не нажато
}

void printAll()
{
    lcd.clear();
    if (first != -1)
    {
        lcd.print(first);
        if (op != '0')
        {
            lcd.print(op);
            if (second != -1)
            {
                lcd.print(second);
                if (!isResultEmpty())
                {
                    lcd.setCursor(0, 1);
                    lcd.print(result);
                }
            }
        }
    }
}

void addNumberToOperand(int &operand, char number)
{
    if (operand == -1)
        operand = 0;
    if (operand < 999)
    {
        operand = (operand * 10) + (number - '0');
    }
}
void restoreOperand()
{

    if (isEmpty(first))
    {
        lcd.print("Restore=");
        first = readFloatFromEEPROM();
        lcd.print(first);
    }
    else if (isEmpty(second) && op != '0')
    {
        lcd.print("Restore=");
        second = readFloatFromEEPROM();
        lcd.print(second);
    }
    delay(1000);
    printAll();
}

void makeEmptyOperand(int &operand)
{
    operand = -1;
}
void makeResultEmpty()
{
    result = -1.0;
}

bool isEmpty(float opeand)
{
    return opeand == -1.0;
}
bool isResultEmpty()
{
    return result == -1.0;
}
bool isFloatEmpty(float operand)
{
    return operand == -1.0;
}
void makeFloatEmpty(float &operand)
{
    operand = -1.0;
}
void testKey()
{
    char key = getKey();
    if (key != '\0')
    {
        lcd.print(key);
    }
}
void clearOperands()
{
    makeEmptyOperand(first);
    makeEmptyOperand(second);
    makeResultEmpty();
    op = '0';
}
void loop()
{
    // testKey();
    loop1();
}
void loop1()
{
    char key = getKey();
    if (key != '\0')
    {

        if (key >= '0' && key <= '9')
        {
            makeResultEmpty();
            if (op == '0')
            {
                addNumberToOperand(first, key);
            }
            else
            {
                addNumberToOperand(second, key);
            }

            printAll();
        }
        if (key == '+' || key == '-' || key == '*' || key == '/')
        {
            if (!isEmpty(first))
            {
                op = key;
                makeEmptyOperand(second);
                printAll();
            }
        }
        // else if (key == '#' && resultStored)
        // {
        //     writeFloatToEEPROM(0, savedResult);
        //     savedResult = 0;
        //     lcd.clear();
        //     lcd.print("Saved");
        //     resultStored = false;
        //     delay(1500);
        //     lcd.clear();
        // }
        // else if (key == '#' && !resultStored)
        // {
        //     lcd.clear();

        //     savedResult = readFloatFromEEPROM(0);
        //     resultStored = true;
        //     lcd.print(savedResult);
        // }
        else if (key == clearChar)
        {
            clearOperands();
            printAll();
        }
        else if (key == resultChar)
        {
            if (isFloatEmpty(first) || isFloatEmpty(second))
            {
                restoreOperand();
            }
            else if (!isFloatEmpty(first) && !isFloatEmpty(second) && isResultEmpty())
            {
                if (!isEmpty(first) && !isEmpty(second) && op != '0')
                {
                    switch (op)
                    {
                    case '+':
                        result = first + second;
                        break;
                    case '-':
                        result = first - second;
                        break;
                    case '*':
                        result = first * second;
                        break;
                    case '/':
                        if (second != -1 && second != 0)
                        {
                            result = (float)first / (float)second;
                        }
                        else
                        {
                            lcd.clear();
                            lcd.print("Error: Деление на 0");
                            delay(1500);
                            lcd.clear();
                            makeEmptyOperand(second);
                        }
                        break;
                    }
                    printAll();
                }
            }
            else
            {
                writeFloatToEEPROM(result);
                lcd.clear();
                lcd.print("Saved");
                delay(1000);
                lcd.clear();
                clearOperands();
            }
        }
    }
}

// else
// {
//     float operand = 0;
//     bool isSecond = false;
//     lcd.setCursor(0, 1);
//     lcd.print(key);

//     while (true)
//     {
//         char newKey = getKey();
//         if (newKey == '#' && !isSecond)
//         {
//             operand = readFloatFromEEPROM(0);
//             lcd.print(operand);
//             isSecond = true;
//         }
//         else if (newKey >= '0' && newKey <= '9')
//         {
//             lcd.print(newKey);

//             isSecond = true;
//         }
//         else if (newKey == '#' && isSecond)
//         {

//             case 'C':
//                 lcd.clear();
//                 savedResult = 0;
//                 resultStored = false;
//                 lcd.print("Reset");
//                 break;
//             }
//             lcd.clear();
//             lcd.print(savedResult);
//             isSecond = false;
//             operand = 0;
//             break;
//         }
//     }
// }
