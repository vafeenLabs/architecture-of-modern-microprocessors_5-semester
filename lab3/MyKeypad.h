#include <EEPROM.h>

// namespace Keypad
// {
void initialize(int *rows, int *columns);
static void inputColumn0();
static void inputColumn1();
static void inputColumn2();
static void inputColumn3();
void findRow();

char getKey();
long getMemory();
void recordMemory(long variable);
const int DIMENSIONS = 4;
const char KEYS[DIMENSIONS][DIMENSIONS] = {
	{'1', '2', '3', '+'},
	{'4', '5', '6', '-'},
	{'7', 'M', 'R', '*'},
	{'C', '0', '=', '/'}};

int rowsPins[DIMENSIONS];
int columnsPins[DIMENSIONS];

volatile bool isButtonPressed = false;
volatile int column;
char pressedKey;

void inputColumn0()
{
	column = 0;
	isButtonPressed = true;
}

void inputColumn1()
{
	column = 1;
	isButtonPressed = true;
}

void inputColumn2()
{
	column = 2;
	isButtonPressed = true;
}

void inputColumn3()
{
	column = 3;
	isButtonPressed = true;
}

void releaseButton()
{
	Serial.println("release button");
	attachInterrupt(digitalPinToInterrupt(columnsPins[0]), inputColumn0, FALLING);
	attachInterrupt(digitalPinToInterrupt(columnsPins[1]), inputColumn1, FALLING);
	attachInterrupt(digitalPinToInterrupt(columnsPins[2]), inputColumn2, FALLING);
	attachInterrupt(digitalPinToInterrupt(columnsPins[3]), inputColumn3, FALLING);
}

void findRow()
{
	detachInterrupt(digitalPinToInterrupt(columnsPins[0]));
	detachInterrupt(digitalPinToInterrupt(columnsPins[1]));
	detachInterrupt(digitalPinToInterrupt(columnsPins[2]));
	detachInterrupt(digitalPinToInterrupt(columnsPins[3]));

	int totalPersistentCurrent = 0;
	int iterations = 0;
	do
	{
		int current = digitalRead(columnsPins[column]) == LOW;
		totalPersistentCurrent += current;
		++iterations;
		if (iterations > 200)
		{
			pressedKey = ' ';
			goto end;
		}
		delay(3);
	} while (totalPersistentCurrent < 100);

	for (int i = 0; i < DIMENSIONS; ++i)
	{
		digitalWrite(rowsPins[i], LOW);
	}

	for (int i = 0; i < DIMENSIONS; ++i)
	{
		digitalWrite(rowsPins[i], HIGH);
		if (digitalRead(columnsPins[column]) == HIGH)
		{
			pressedKey = KEYS[i][column];
		}
		digitalWrite(rowsPins[i], LOW);
	}
end:
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		attachInterrupt(digitalPinToInterrupt(columnsPins[i]), releaseButton, RISING);
	}

	// for (int i = 0;i< DIMENSIONS;++i)
	// {
	//   digitalWrite(columnsPins[i],HIGH);
	// }

	// attachInterrupt(digitalPinToInterrupt(columnsPins[0]), inputColumn0, FALLING);
	// attachInterrupt(digitalPinToInterrupt(columnsPins[1]), inputColumn1, FALLING);
	// attachInterrupt(digitalPinToInterrupt(columnsPins[2]), inputColumn2, FALLING);
	// attachInterrupt(digitalPinToInterrupt(columnsPins[3]), inputColumn3, FALLING);
}

void initialize(int *rows, int *columns)
{
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		rowsPins[i] = rows[i];
		columnsPins[i] = columns[i];
		pinMode(rowsPins[i], OUTPUT);
		digitalWrite(rowsPins[i], LOW);
		pinMode(columnsPins[i], INPUT_PULLUP);
	}
	attachInterrupt(digitalPinToInterrupt(columnsPins[0]), inputColumn0, FALLING);
	attachInterrupt(digitalPinToInterrupt(columnsPins[1]), inputColumn1, FALLING);
	attachInterrupt(digitalPinToInterrupt(columnsPins[2]), inputColumn2, FALLING);
	attachInterrupt(digitalPinToInterrupt(columnsPins[3]), inputColumn3, FALLING);

	for (int i = 0; i < DIMENSIONS; ++i)
	{
		attachInterrupt(digitalPinToInterrupt(columnsPins[i]), releaseButton, RISING);
	}
}

long getMemory()
{
	long var;
	EEPROM.get(0, var);
	return var;
}

void recordMemory(long variable)
{
	EEPROM.put(0, variable);
}

char getKey()
{

	if (!isButtonPressed)
	{
		return ' ';
	}
	else
	{
		isButtonPressed = false;
		findRow();
		char result = pressedKey;
		return result;
	}
}

//}
