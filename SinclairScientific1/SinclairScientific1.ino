// Arduino Port of
//
// TI calculator simulator
// Ken Shirriff, http://righto.com/ti
// Based on patent US3934233
//
//
// The goal of this project is to run the following simulator: http://righto.com/sinclair
// on an arduino nano powered custom pcb resembling the original Sinclair Scientific Calculator
// @arduinoenigma 2018

// TODO:
// match PrintableKeys to
// SinclairData.keyStrobe  -> activate KN KO KP
// SinclairData.keyPressed -> activate DK

#include "GPIO.h"

extern void display();
extern void allKeyRowOff();
extern void allSegmentOutput();
extern void allSegmentOff();
extern void allDigitOutput();
extern void allDigitOff();
extern void step();
extern char readKey();

unsigned int const objectCode[] PROGMEM = {
  1408, 1392, 1792, 1824, 1860, 1808, 1360, 1376,
  518, 1319, 1360, 1376, 9, 1360, 1908, 1072,
  1083, 1075, 1121, 1129, 1073, 1069, 1051, 1840,
  1955, 1840, 516, 1425, 552, 1430, 33, 1792,
  1398, 1631, 1920, 1683, 34, 2003, 1540, 4,
  1399, 1858, 1872, 1538, 4, 1329, 1335, 4,
  1349, 1347, 4, 1443, 676, 1431, 57, 1559,
  4, 1553, 59, 1443, 677, 1839, 1632, 2018,
  65, 2023, 1719, 72, 1730, 71, 1840, 1666,
  1751, 587, 1840, 1754, 78, 1840, 1718, 594,
  1924, 78, 2017, 1713, 89, 1540, 130, 1844,
  1841, 1652, 597, 130, 1730, 95, 1849, 1650,
  114, 1443, 675, 1355, 1345, 130, 1409, 1559,
  105, 1443, 750, 1839, 1632, 1844, 2023, 1719,
  92, 1538, 1537, 116, 1451, 1796, 791, 1908,
  1781, 637, 1722, 1540, 120, 1940, 1786, 119,
  1445, 820, 1754, 512, 1747, 145, 1860, 1751,
  142, 1686, 141, 1799, 1798, 1686, 1558, 132,
  132, 1908, 1751, 662, 1686, 1686, 1558, 152,
  1441, 614, 1392, 1334, 1408, 1750, 161, 1559,
  159, 1568, 4, 1351, 1355, 1686, 681, 1908,
  165, 1801, 1689, 1824, 1445, 180, 1447, 179,
  1568, 1819, 185, 1565, 1820, 1924, 2011, 1693,
  738, 1888, 1888, 1888, 2012, 1696, 1936, 1936,
  1936, 1872, 1872, 1872, 2012, 1584, 1724, 1920,
  1920, 1920, 1445, 183, 1561, 1447, 210, 1779,
  697, 220, 1451, 727, 1732, 185, 220, 1754,
  1844, 1764, 1844, 185, 1904, 1904, 1904, 1904,
  1904, 130, 1447, 233, 1616, 1600, 1808, 1411,
  100, 1451, 236, 1632, 1840, 130, 1445, 251,
  1750, 760, 1751, 759, 1908, 1686, 240, 1860,
  1794, 1864, 1864, 1824, 2003, 1636, 1924, 1924,
  1700, 1431, 823, 2009, 1787, 253, 1993, 2036,
  1723, 1920, 1920, 1920, 1920, 1588, 1844, 1445,
  814, 1600, 1479, 1447, 765, 1572, 1796, 1806,
  1764, 797, 1700, 1562, 280, 1571, 803, 1860,
  1631, 1892, 280, 1807, 1443, 808, 130, 10,
  1572, 1796, 1904, 1904, 1794, 61, 1572, 1796,
  1803, 1652, 1844, 117, 1908, 1335, 251, 1693,
  253, 1565, 1860, 1563, 2036, 1844, 1411, 790
};

#define NUMBER_OF_MASKS 16
#define MASK_LENGTH 12

const char masks [][MASK_LENGTH] PROGMEM = {
  {"00000000000"}, // M0
  {"5          "}, // M1
  {"  00       "}, // M2
  {"    1      "}, // M3
  {"    0000000"}, // M4
  {"          1"}, // M5
  {"  01       "}, // M6
  {" 5         "}, // M7
  {"000000     "}, // M8
  {"0001       "}, // M9
  {"    0000001"}, // M10
  {"     1     "}, // M11
  {"    00005  "}, // M12
  {"    00001  "}, // M13
  {"    4      "}, // M14
  {"    0      "}, // M15
};

//OPS WITH K: 1:AAKA 2:AAKC 5:ACKA 6:ACKB 9: 11:
//const unsigned long LISTOPSWITHK = 000111100000001111010101001100110b;
const unsigned long LISTOPSWITHK = 1007135334;

char key = 0;
char p = 0;
boolean resetRequested = false;

signed char digits[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
char PrintableKeys[19] = "12+E0v-378X654/9^C";

const char keysKN[10] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 0};
const char keysKO[10] = {'C', 'v', '+', '-', '/', 'X', '^', 'E', '0', 0};
//const char keysKP[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//to use with keyStrobe
#define KN 1
#define KO 2
#define KP 3

//to use with keyPressed
#define DK 1

//ORIGINAL SINCLAIR DATA
struct SinclairData_t
{
  boolean sinclair = true;

  // Important: Array order matches display order, not bit order.
  // I.e. a[0] is high-order digit S10, a[10] is low-order digit S0.
  signed char  a[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};    // Register A
  signed char  b[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};    // Register B
  signed char  c[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};    // Register C
  signed char af[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};    // Flags A
  signed char bf[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};    // Flags A

  signed char  d[11] = {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  // Currently active D value 1-11. d[dActive-1] == 0
  byte dActive = 1;
  signed char cc = 0;
  char ccMeaning[12];

  byte keyPressed = 0;
  // 'KO' or 'KP' if a keyboard input line is active, i.e. dActive and keyPressed match in the key matrix
  byte keyStrobe = 0;
  unsigned int address = 0; // PROGRAM COUNTER
  byte display = 1; // Flag for display on
  char mask[MASK_LENGTH];
  byte showDisplayScan = 0;
  byte idle = 0; // 1 if in idle loop
  byte speed = 0; // Speed for updates
  byte fastStep = 1; // 1 makes SYNC, etc take 1 cycle
  byte power = 1; // On
} SinclairData;

//this function is called by the display routines to show a digit for 2ms before moving to the next one.
//can do useful work here... instead of delay(2);
void backgroundWork() {

  stepAndScan();
}

//TODO: enable display digits 0 and 1, move 2..5 to 0.3 and add digits 7 and 8
void updateDisplay() {

  digits[0] = 0;
  digits[1] = 0;
  digits[2] = (SinclairData.a[0] == 0) ? 99 : 10;
  digits[3] = SinclairData.a[4];
  digits[4] = SinclairData.a[5];
  digits[5] = SinclairData.a[6]; //7 and 8 also when we get digits 0 and 1 working
  digits[6] = (SinclairData.a[1] == 0) ? 99 : 10;
  digits[7] = SinclairData.a[2];
  digits[8] = SinclairData.a[3];

  display();
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(250000);

  allKeyRowOff();

  allSegmentOutput();
  allSegmentOff();

  allDigitOutput();
  allDigitOff();

}

void loop() {
  // put your main code here, to run repeatedly:

  key = readKey();

  stepAndScan();

  updateDisplay();

}

void stepAndScan() {

  step();

  if ((resetRequested) && (SinclairData.dActive == 8)) {
    resetRequested = false;
    SinclairData.address = 0;
    SinclairData.keyStrobe = 0;
  }

  SinclairData.keyStrobe = 0;

  //Serial.println(SinclairData.dActive - 1);

  if (key != 0)
  {
    p = PrintableKeys[key - 1];
    //Serial.println(PrintableKeys[key - 1]);

    if (p == keysKN[SinclairData.dActive - 1]) {
      SinclairData.keyStrobe = KN;
      //Serial.println(F("KN"));
    }

    if (p == keysKO[SinclairData.dActive - 1]) {
      SinclairData.keyStrobe = KO;
      //Serial.println(F("KP"));
    }

    if (p == 'C') {
      resetRequested = true;
    }
  }
}
