#include <LiquidCrystal_I2C.h>
#include "MyKeypad.h"
#include <EEPROM.h>

#define I2C_ADDR 0x27
#define LCD_COLUMNS 16
#define LCD_LINES 2

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);

const int MAX_LENGTH = 6;

char key = ' ';
char operation = ' ';
String inputString = "";
String variableString = "";

long memory;
long variable;
int rows[4] = {2, 3, 4, 5};
int cols[4] = {6, 7, 8, 9};
void setup()
{
	lcd.init();
	lcd.clear();
	lcd.backlight();
	lcd.setCursor(0, 0);
	lcd.print("begin");
	delay(500);
	lcd.clear();
	// lcd.backlight(false);
	Serial.begin(9600);

	//	Keypad::initialize(rows, cols);
	initialize(rows, cols);
}

long toDecimal(String str)
{
	long decimal = 0;
	long multiplier = 1;
	int sign = 1;

	int stop = 0;
	if (str[stop] == '-')
	{
		++stop;
		sign *= -1;
	}

	for (int i = str.length() - 1; i >= stop; --i)
	{
		decimal += (str[i] - '0') * multiplier;
		multiplier *= 8;
	}
	decimal *= sign;
	return decimal;
}

void toMemory(String str)
{
	long decimal = toDecimal(str);
	memory = decimal;
}

long decimalToOctal(long decimalNumber)
{
	int rem, i = 1;
	long octalNumber = 0;
	while (decimalNumber != 0)
	{
		rem = decimalNumber % 8;
		decimalNumber /= 8;
		octalNumber += rem * i;
		i *= 10;
	}
	return octalNumber;
}

int calculate()
{
	int result;
	switch (operation)
	{
	case '+':
	{
		result = memory + variable;
		break;
	}
	case '-':
	{
		result = memory - variable;
		break;
	}
	case '*':
	{
		result = memory * variable;
		break;
	}
	case '/':
	{
		result = memory / variable;
		break;
	}
	}

	return result;
}

void loop()
{
	//	key = Keypad::getKey();
	key = getKey();
	if (key != ' ')
	{
		if ((key == '+' || key == '-' || key == '*' || key == '/'))
		{
			if (inputString == "")
			{
				operation = ' ';
			}
			else
			{
				if (operation == ' ')
				{
					if (inputString.length() < MAX_LENGTH)
					{
						toMemory(inputString);
						operation = key;
						variableString = "";
						inputString += " ";
						inputString += key;
					}
					else
					{
						variableString = "";
						inputString = "overflow";
					}
				}
			}
		}
		else if (key == '=')
		{
			if (variableString.length() < MAX_LENGTH)
			{
				variable = toDecimal(variableString);
				Serial.print("variable ");
				Serial.println(variable);
				Serial.print("mem ");
				Serial.println(memory);
				int result = calculate();
				memory = result;
				Serial.print("result ");
				Serial.println(memory);
				inputString = String(decimalToOctal(memory));
				operation = ' ';
			}
			else
			{
				variableString = "overflow";
			}
		}
		else if (key == 'C')
		{
			if (operation != ' ')
			{
				operation = ' ';
				variableString = "";
				if (memory != 0)
				{
					inputString = (String(decimalToOctal(memory)));
				}
				else
				{
					inputString = "";
				}
			}
			else
			{
				inputString = "";
			}
		}
		else if (key == 'M')
		{
			//	long var = Keypad::getMemory();
			long var = getMemory();
			if (operation != ' ')
			{
				variable = var;
				if (variable != 0)
					variableString = (String(decimalToOctal(variable)));
			}
			else
			{
				memory = var;
				if (memory != 0)
					inputString = String(decimalToOctal(memory));
			}
		}
		else if (key == 'R')
		{
			if (operation = ' ')
			{
				memory = toDecimal(inputString);
			}
			// Keypad::recordMemory((memory));
			recordMemory(memory);
		}
		else
		{
			if (operation == ' ')
			{
				if (!(key == '0' && inputString.length() == 0))
					inputString += key;
			}
			else
			{
				if (!(key == '0' && variableString.length() == 0))
					variableString += key;
			}
		}

		lcd.clear();
		lcd.print(inputString);
		if (operation != ' ')
		{
			lcd.setCursor(1, 1);
			lcd.print(variableString);
		}
	}
}
