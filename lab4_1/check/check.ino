#define S0 4                    // Указываем вывод S0
#define S1 5                    // Указываем вывод S1
#define S2 6                    // Указываем вывод S2
#define S3 7                    // Указываем вывод S3
#define sensorOut 8             // Указываем вывод Out
#define OE 9
int frequency = 0;              // Создаем переменную frequency

void setup() {
  pinMode(S0, OUTPUT);          // Устанавливаем S0 как выход
  pinMode(S1, OUTPUT);          // Устанавливаем S1 как выход
  pinMode(S2, OUTPUT);          // Устанавливаем S2 как выход
  pinMode(S3, OUTPUT);          // Устанавливаем S3 как выход
  pinMode(sensorOut, INPUT);    // Устанавливаем sensorOut как выход

  pinMode(OE, OUTPUT);
  digitalWrite(OE, LOW); // Включение выходного сигнала датчика

  // Установка масштабирования частоты до 100% 
  digitalWrite(S0,HIGH);        // Вывод S0 в HIGH   
  digitalWrite(S1,LOW);        // Вывод S1 в HIGH
  
  Serial.begin(9600);           // Открываем последовательный порт
}
void loop() {
  // Установка фотодиодов с красным фильтром для чтения
  digitalWrite(S2,LOW);         // Вывод S2 в LOW
  digitalWrite(S3,LOW);         // Вывод S3 в LOW
  frequency = pulseIn(sensorOut, LOW);        // Чтение выходной частоты
  Serial.print("R= ");                        // Отправка текста в порт
  Serial.print(frequency);                    // Печать показаний красного цвета
  Serial.print("  ");                         // Отправка текста в порт
  delay(100);                                 // Пауза

  // Установка фотодиодов с зеленым фильтром для чтения
  digitalWrite(S2,HIGH);        // Вывод S2 в HIGH
  digitalWrite(S3,HIGH);        // Вывод S3 в HIGH
  frequency = pulseIn(sensorOut, LOW);        // Чтение выходной частоты
  Serial.print("G= ");                        // Отправка текста в порт
  Serial.print(frequency);                    // Печать показаний зеленого цвета
  Serial.print("  ");                         // Отправка текста в порт
  delay(100);                                 // Пауза

    // Установка фотодиодов с синим фильтром для чтения
  digitalWrite(S2,LOW);         // Вывод S2 в LOW
  digitalWrite(S3,HIGH);        // Вывод S3 в HIGH
  frequency = pulseIn(sensorOut, LOW);        // Чтение выходной частоты
  Serial.print("B= ");                        // Отправка текста в порт
  Serial.print(frequency);                    // Печать показаний синего цвета
  Serial.println("  ");                       // Отправка текста в порт
  delay(100);                                 // Пауза
}
