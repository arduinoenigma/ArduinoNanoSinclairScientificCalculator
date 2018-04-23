// Arduino Port of
//
// TI calculator simulator
// Ken Shirriff, http://righto.com/ti
// Based on patent US3934233
//
// The goal of this project is to run the following simulator: http://righto.com/sinclair
// on an arduino nano powered custom pcb resembling the original Sinclair Scientific Calculator
// @arduinoenigma 2018
//
// tedious but fast, most lines compile to 1 instruction.
// this whole file compiles to less than 1KB of code
//

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

GPIO<BOARD::D5>  SegmentA;
GPIO<BOARD::D8>  SegmentB;
GPIO<BOARD::D10> SegmentC;
GPIO<BOARD::D19> SegmentD;
GPIO<BOARD::D9>  SegmentE;
GPIO<BOARD::D6>  SegmentF;
GPIO<BOARD::D14> SegmentG;
GPIO<BOARD::D18> SegmentDP;

GPIO<BOARD::D2>  Digit1;
GPIO<BOARD::D3>  Digit2;
GPIO<BOARD::D4>  Digit3;
GPIO<BOARD::D17> Digit4;
GPIO<BOARD::D16> Digit5;
GPIO<BOARD::D7>  Digit6;
GPIO<BOARD::D15> Digit7;
GPIO<BOARD::D11> Digit8;
GPIO<BOARD::D12> Digit9;

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

void allDigitInput() {
  Digit1.input();
  Digit2.input();
  Digit3.input();
  Digit4.input();
  Digit5.input();
  Digit6.input();
  Digit7.input();
  Digit8.input();
  Digit9.input();
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

byte lastSelectDigit = 0;

void selectDigit(byte digit) {

  lastSelectDigit = digit;

  allDigitOff();
  allDigitInput();

  switch (digit) {
    case 1:
      Digit1.high();
      Digit1.output();
      break;
    case 2:
      Digit2.high();
      Digit2.output();
      break;
    case 3:
      Digit3.high();
      Digit3.output();
      break;
    case 4:
      Digit4.high();
      Digit4.output();
      break;
    case 5:
      Digit5.high();
      Digit5.output();
      break;
    case 6:
      Digit6.high();
      Digit6.output();
      break;
    case 7:
      Digit7.high();
      Digit7.output();
      break;
    case 8:
      Digit8.high();
      Digit8.output();
      break;
    case 9:
      Digit9.high();
      Digit9.output();
      break;
  }
}

void displaySelfTest(bool longtest = false) {

  char c = 0;

  //kitt stuff
  byte pos = 0;
  byte p1 = 0;
  byte p2 = 0;
  char sign = 0;

  bool kitt = false;

  // show 8.8.8.8.8.8.8.8.8.
  for (byte t = 0; t < 75; t++) {
    for (byte j = 0; j < 9; j++) {
      outputDigit(8, true);
      selectDigit(j + 1);
      c = readKey();
      if (c == '8') {
        longtest = true;
      }
      if ((longtest == true ) && (c == '2')) {
        kitt = true;
      }
      if (c == 'C') {
        goto exitfn;
      }
      delay(2);
    }
  }

  if (kitt) {
    goto kitt;
  }

  if (!longtest) {
    goto exitfn;
  }

  // cycle through 0..9 - on all digits at the same time
  for (byte i = 0; i < 12; i++) {
    for (byte t = 0; t < 100; t++) {
      for (byte j = 0; j < 9; j++) {
        outputDigit((i != 11) ? i : 99, (i == 11));
        selectDigit(j + 1);
        if (readKey() == 'C') {
          goto exitfn;
        }
        delay(2);
      }
    }
  }

  // show 12345678
  for (byte t = 0; t < 100; t++) {
    for (byte j = 0; j < 9; j++) {
      outputDigit(j, false);
      selectDigit(j + 1);
      if (readKey() == 'C') {
        goto exitfn;
      }
      delay(2);
    }
  }

  // move the dot from left to right on 12345678
  for (byte i = 0; i < 9; i++) {
    for (byte t = 0; t < 100; t++) {
      for (byte j = 0; j < 9; j++) {
        outputDigit(j, i == j);
        selectDigit(j + 1);
        if (readKey() == 'C') {
          goto exitfn;
        }
        delay(2);
      }
    }
  }

  // move dot from right to left on blank display
  for (byte j = 9; j > 0; j--) {
    for (byte t = 0; t < 100; t++) {
      for (byte i = 0; i < 9; i++) {
        outputDigit(99, i == (j - 1));
        selectDigit(i + 1);
        if (readKey() == 'C') {
          goto exitfn;
        }
        delay(2);
      }
    }
  }

  // cycle through 0..9 - on each digit individually
  for (byte j = 0; j < 9; j++) {
    for (byte i = 0; i < 12; i++) {
      for (byte t = 0; t < 20; t++) {
        for (byte k = 0; k < 9; k++) {
          outputDigit( ((k == j) && (i != 11)) ? i : 99, ((k == j) && (i == 11)));
          selectDigit(k + 1);
          if (readKey() == 'C') {
            goto exitfn;
          }
          delay(2);
        }
      }
    }
  }

  goto exitfn;

kitt:

  for (byte i = 0; i < 249; i++)
  {
    p2 = p1;
    p1 = pos;
    pos += sign;

    if (pos == 0) {
      sign = 1;
    }

    if (pos == 8) {
      sign = -1;
    }

    for (byte t = 0; t < 25; t++) {
      for (byte j = 0; j < 9; j++) {
        c = readKey();
        if (c == 'C') {
          goto exitfn;
        }
        outputDigit(10, false);
        selectDigit(j + 1);
        if (j == pos)
        {
          delayMicroseconds(1000);
        } else if (j == p1)
        {
          delayMicroseconds(500);
        } else if (j == p2)
        {
          delayMicroseconds(100);
        } else
          delayMicroseconds(20);
      }
    }
  }

exitfn:
  allSegmentOff();
  allDigitOff();

}

//const char PrintableKeys[19] = "12+E0v-378X654/9^C";

const char KeysKN[10] = "156789234";
const char KeysKO[10] = "C/*^E0v+-";

byte readKey() {

  byte key = 0;

  if (analogRead(7) > 100)
  {
    SinclairData.keyStrobeKN = 1;
    key = KeysKN[lastSelectDigit - 1];
  }
  else
  {
    SinclairData.keyStrobeKN = 0;
  }

  if (analogRead(6) > 100)
  {
    SinclairData.keyStrobeKO = 1;
    key = KeysKO[lastSelectDigit - 1];
  }
  else
  {
    SinclairData.keyStrobeKO = 0;
  }

  // ensure C has priority
  if ((lastSelectDigit == 1) && (SinclairData.keyStrobeKO))
  {
    key = 'C';
  }

  /*
    Serial.print(lastSelectDigit);
    Serial.print(SinclairData.dActive);
    Serial.print(SinclairData.keyStrobeKN);
    Serial.print(SinclairData.keyStrobeKO);
    Serial.print(key);
    Serial.println("");
  */

  return key;
}

//since reakKey only reads the active column, this function sweeps through all columns and calls readKey
byte readKeys()
{
  byte k;
  byte key = 0;

  outputDigit(99);
  for (byte i = 1; i < 10; i++)
  {
    selectDigit(i);
    k = readKey();

    if (k)
    {
      key = k;
    }

  }

  return key;
}
