#include <Arduino.h>
#include <TM1637Display.h>

// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3
#define LIT_T SEG_D | SEG_E | SEG_F | SEG_G
#define LIT_R SEG_E | SEG_G
#define LIT_A SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G
#define LIT_I SEG_B | SEG_C
#define LIT_N  SEG_C | SEG_E | SEG_G
#define LIT_L SEG_D | SEG_E | SEG_F
#define LIT_E SEG_A | SEG_D | SEG_E | SEG_F | SEG_G
#define LIT_F SEG_A | SEG_E | SEG_F | SEG_G

const uint8_t SEG_TRAIN[] = {
0x00, 0x00, 0x00, LIT_T, LIT_R, LIT_A, LIT_I, LIT_N, 0x00, LIT_L, LIT_E, LIT_F, LIT_T, 0x00, 0x00, 0x00, 0x00};

TM1637Display display(CLK, DIO);

void setup() {
}

void loop() {
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
  display.setBrightness(0x0f);
  //display.setSegments(data);
  delay(500);
//1
for(int i=100; i >= -100; i-=2) {
uint8_t data[] = {};
if (i > -1 || i <= -100) {
display.showNumberDec(i, true); // Expect: 0000
delay(100);
} else if (i > -10) {
data[0] = SEG_G;
data[3] = display.encodeDigit(-i);
display.setSegments(data);
delay(100);
} else if (i > -100) {
int tr_i = trunc(-i / 10);
data[0] = SEG_G;
data[2] = display.encodeDigit(tr_i);
data[3] = display.encodeDigit((-10*tr_i)-i);
display.setSegments(data);
delay(100);
}
};

//2
  // for(float d=5; d >= -5; d-=0.1) {  
  //   if ( d >= 0) {
  //     int a = d*100;
  //     int trunc_d = trunc(d);
  //     int dd = (d - trunc_d) * 10;
  //     data[0] = 0x00;
  //     data[1] = display.encodeDigit(trunc_d) | B10000000;
  //     data[2] = display.encodeDigit(dd);
  //     data[3] = display.encodeDigit(0);
  //     display.setSegments(data);
  //   } else {
  //     float b = ((-d*100) + 1) / 100;
  //     int trunc_d = trunc(b);
  //     float dd = (b*10 - trunc_d*10);
  //     data[0] = SEG_G;
  //     data[1] = display.encodeDigit(0);
  //     data[1] = display.encodeDigit(trunc_d) | B10000000;
  //     data[2] = display.encodeDigit(dd);
  //     data[3] = display.encodeDigit(0);
  //     display.setSegments(data);
  //   };
  //   delay(200);
  // };

  //3
  // size_t text_len = sizeof(SEG_TRAIN)/sizeof(uint8_t) - 4;
  // while (1)
  // {
  // for(int t=0; t < text_len; t++) {

  //       //uint8_t data[] = { SEG_TRAIN[t], SEG_TRAIN[t+1], SEG_TRAIN[t+2], SEG_TRAIN[0]  };
  //       display.setSegments(SEG_TRAIN + t);
  //       delay(200);

  // }
  //   //delay(400);
  // }
}
