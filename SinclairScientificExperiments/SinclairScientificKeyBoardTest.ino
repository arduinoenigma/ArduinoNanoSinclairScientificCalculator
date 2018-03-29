
#include "GPIO.h"

//
// A0 = D14
// A1 = D15
// A2 = D16
// A3 = D17
// A4 = D18
// A5 = D19
// A6 = D20
// A7 = D21
//

GPIO<BOARD::D14> SegmentA;
GPIO<BOARD::D15> SegmentB;
GPIO<BOARD::D16> SegmentC;
GPIO<BOARD::D17> SegmentD;
GPIO<BOARD::D18> SegmentE;
GPIO<BOARD::D19> SegmentF;
GPIO<BOARD::D3> SegmentG;
GPIO<BOARD::D2> SegmentDP;

GPIO<BOARD::D12> Digit1;
GPIO<BOARD::D11> Digit2;
GPIO<BOARD::D10> Digit3;
GPIO<BOARD::D9> Digit4;
GPIO<BOARD::D8> Digit5;
GPIO<BOARD::D7> Digit6;
GPIO<BOARD::D6> Digit7;
GPIO<BOARD::D5> Digit8;
GPIO<BOARD::D4> Digit9;

void allSegmentOutput() {
  SegmentA.output();
  SegmentB.output();
  SegmentC.output();
  SegmentD.output();
  SegmentE.output();
  SegmentF.output();
  SegmentG.output();
  SegmentDP.output();
}

void allSegmentOff() {
  SegmentA.high();
  SegmentB.high();
  SegmentC.high();
  SegmentD.high();
  SegmentE.high();
  SegmentF.high();
  SegmentG.high();
  SegmentDP.high();
}

void allSegmentInput() {
  SegmentA.input();
  SegmentB.input();
  SegmentC.input();
  SegmentD.input();
  SegmentE.input();
  SegmentF.input();
  SegmentG.input();
  SegmentDP.input();

  SegmentA.high();
  SegmentB.high();
  SegmentC.high();
  SegmentD.high();
  SegmentE.high();
  SegmentF.high();
  SegmentG.high();
  SegmentDP.high();
}

void allDigitOutput() {
  Digit1.output();
  Digit2.output();
  Digit3.output();
  Digit4.output();
  Digit5.output();
  Digit6.output();
  Digit7.output();
  Digit8.output();
  Digit9.output();
}

void allDigitOff() {
  Digit1.low();
  Digit2.low();
  Digit3.low();
  Digit4.low();
  Digit5.low();
  Digit6.low();
  Digit7.low();
  Digit8.low();
  Digit9.low();
}

byte outputDigit(signed char digit, bool decimalpoint = false) {

  byte segmentslit = 0;

  allDigitOff();

  if (decimalpoint)
  {
    segmentslit++;
    SegmentDP.low();
  }
  else
  {
    SegmentDP.high();
  }

  switch (digit) {
    case 0:
      SegmentA.low();
      SegmentB.low();
      SegmentC.low();
      SegmentD.low();
      SegmentE.low();
      SegmentF.low();
      SegmentG.high();
      segmentslit += 6;
      break;
    case 1:
      SegmentA.high();
      SegmentB.low();
      SegmentC.low();
      SegmentD.high();
      SegmentE.high();
      SegmentF.high();
      SegmentG.high();
      segmentslit += 2;
      break;
    case 2:
      SegmentA.low();
      SegmentB.low();
      SegmentC.high();
      SegmentD.low();
      SegmentE.low();
      SegmentF.high();
      SegmentG.low();
      segmentslit += 5;
      break;
    case 3:
      SegmentA.low();
      SegmentB.low();
      SegmentC.low();
      SegmentD.low();
      SegmentE.high();
      SegmentF.high();
      SegmentG.low();
      segmentslit += 5;
      break;
    case 4:
      SegmentA.high();
      SegmentB.low();
      SegmentC.low();
      SegmentD.high();
      SegmentE.high();
      SegmentF.low();
      SegmentG.low();
      segmentslit += 4;
      break;
    case 5:
      SegmentA.low();
      SegmentB.high();
      SegmentC.low();
      SegmentD.low();
      SegmentE.high();
      SegmentF.low();
      SegmentG.low();
      segmentslit += 5;
      break;
    case 6:
      SegmentA.low();
      SegmentB.high();
      SegmentC.low();
      SegmentD.low();
      SegmentE.low();
      SegmentF.low();
      SegmentG.low();
      segmentslit += 6;
      break;
    case 7:
      SegmentA.low();
      SegmentB.low();
      SegmentC.low();
      SegmentD.high();
      SegmentE.high();
      SegmentF.high();
      SegmentG.high();
      segmentslit += 3;
      break;
    case 8:
      SegmentA.low();
      SegmentB.low();
      SegmentC.low();
      SegmentD.low();
      SegmentE.low();
      SegmentF.low();
      SegmentG.low();
      segmentslit += 7;
      break;
    case 9:
      SegmentA.low();
      SegmentB.low();
      SegmentC.low();
      SegmentD.high();
      SegmentE.high();
      SegmentF.low();
      SegmentG.low();
      break;
      segmentslit += 5;
    case 10:
      SegmentA.high();
      SegmentB.high();
      SegmentC.high();
      SegmentD.high();
      SegmentE.high();
      SegmentF.high();
      SegmentG.low();
      segmentslit += 1;
      break;
    case 99:
      SegmentA.high();
      SegmentB.high();
      SegmentC.high();
      SegmentD.high();
      SegmentE.high();
      SegmentF.high();
      SegmentG.high();
      break;
    default:
      allSegmentOff();
      break;
  }

  return segmentslit;
}

byte lastDigit = 0;

void selectDigit(byte digit) {

  allDigitOff();

  lastDigit = digit;

  switch (digit) {
    case 0:
      Digit1.high();
      break;
    case 1:
      Digit2.high();
      break;
    case 2:
      Digit3.high();
      break;
    case 3:
      Digit4.high();
      break;
    case 4:
      Digit5.high();
      break;
    case 5:
      Digit6.high();
      break;
    case 6:
      Digit7.high();
      break;
    case 7:
      Digit8.high();
      break;
    case 8:
      Digit9.high();
      break;
  }
}

// 0..38 is 0
// 900 to 1000 is 1
void testKNKO()
{
  Serial.print("d:");
  Serial.print(lastDigit);
  Serial.print(" kn:");
  Serial.print(analogRead(6));
  Serial.print(" ko:");
  Serial.println(analogRead(7));
}

// http://forum.arduino.cc/index.php?topic=6549.msg51570#msg51570
// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

void setupFastADC()
{
  // http://forum.arduino.cc/index.php?topic=6549.msg51570#msg51570
  // set prescale to 16
  sbi(ADCSRA, ADPS2) ;
  cbi(ADCSRA, ADPS1) ;
  cbi(ADCSRA, ADPS0) ;
}

void testADC() {
  int start ;
  int i ;

  Serial.print("ADCTEST: ") ;
  start = millis() ;
  for (i = 0 ; i < 1000 ; i++)
    analogRead(6) ;
  Serial.print(millis() - start) ;
  Serial.println(" msec (1000 calls)") ;
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(250000);

  setupFastADC();
  testADC();

  allSegmentOff();
  allSegmentOutput();

  allDigitOff();
  allDigitOutput();

  /*
    for (byte i = 0 ; i < 10 ; i++)
    {
    Serial.println(i);
    outputDigit(i);
    selectDigit(0);
    delay(3000);
    }
  */
}

byte a = 99;
byte b = 1;
byte c = 0;

int t = 0;

byte incd(byte now)
{
  byte next = 1;
  switch (now) {
    case 0:
      next = 5;
      break;
    case 1:
      next = 0;
      break;
    case 5:
      next = 99;
      break;
    case 99:
      next = 1;
      break;
  }
  return next;
}

void loop() {
  // put your main code here, to run repeatedly:

  unsigned long entrytime = micros();

  outputDigit(a);
  selectDigit(0);
  testKNKO();
  delay(2);
  outputDigit(b);
  selectDigit(1);
  testKNKO();
  delay(2);
  outputDigit(c);
  selectDigit(2);
  testKNKO();
  delay(2);

  unsigned long exittime = micros();

  outputDigit(99);

  Serial.print("exec: ");
  Serial.println(exittime - entrytime);

  t++;
  if (t > 500)
  {
    t = 0;
    a = incd(a);
    b = incd(b);
    c = incd(c);
  }

}
