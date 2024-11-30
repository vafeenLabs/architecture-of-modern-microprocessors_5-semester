#include <Arduino.h>

// ����� ��� ��������� ������ ������ ������/�������
void setKeypadPins(int rows[4], int cols[4]);

// ������ ����������:
// 0 1 2 &
// 3 4 5 |
// 6 7   ^
//   s = >

// ��������� ������� ������
char getActivityKey();
