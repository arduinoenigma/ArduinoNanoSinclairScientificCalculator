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
// To test the up/down buttons press:
// C up X up / up + up - 5.4105-01
// C dn X dn / dn + dn - 4.4200-01
//
// Sinclair Scientific Glitches implemented:
// dot stays on when display turns off, dot gets brighter
// pressing 0 makes dot brighter, other keys not so much
// pressing a key glitches the number displayed,
// pressing 7 glitches more numbers than pressing 1
// pressing C dims the display
//
// Glitches not implemented:
// 1 is brighter than 0
// - in mantissa is brighter
//
// Other Features:
// press 0 when powering up to display short display self test.
// press 8 when short display self test is showing for longer self test, press C to exit.
// press 1 when powering up for slower than normal speed, display stays lit while computing a result.
// press 2 when powering up for normal speed with display on.
// press 3 when powering up for faster speed with display on.
//
// Notes:
// Step() runs all instructions in 148uS, if an instruction is faster, it delays until 148us passes.
// Adjusted refresh delay in updateDisplay() so execution time of ArcCos 0.0001 matches real hardware.
// step() function get called every 340uS for an effective clock speed of 2.9kHz
//
// uses this fast pin library:
// https://github.com/mikaelpatel/Arduino-GPIO
//

#include "GPIO.h"
#include <SoftwareSerial.h>

extern void allSegmentOutput();
extern void allSegmentOff();
extern void allSegmentInput();
extern void allDigitOutput();
extern void allDigitOff();
extern void allKeyRowOff();
extern void allKeyRowIdle();
extern byte outputDigit(signed char, bool dp = false);
extern void selectDigit(byte);
extern void display();
extern void displaySelfTest(bool longtest = false);
extern byte readKey();

extern void step();

//These are the 320 instructions that performed all the tasks on the Sinclair Scientific.
//the rest of this program is to execute and interface these instructions to todays hardware

const unsigned int objectCode[] PROGMEM = {
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

// OPS WITH K: 1:AAKA 2:AAKC 5:ACKA 6:ACKB 9: 11:
//const unsigned long LISTOPSWITHK = 000111100000001111010101001100110b;
const unsigned long LISTOPSWITHK = 1007135334;

signed char digits[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

// keysKN and keysKO must match PrintableKeys on DisplayAndKeys.ino
//const char PrintableKeys[19] = "12+E0v-378X654/9^C";

const char keysKN[10] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 0};
const char keysKO[10] = {'C', 'v', '+', '-', '/', 'X', '^', 'E', '0', 0};
//const char keysKP[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// to use with keyStrobe
#define KN 1
#define KO 2
#define KP 3

// to use with keyPressed
#define DK 1

// ORIGINAL SINCLAIR DATA
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

  byte dActive = 1;          // Currently active D value 1-11. d[dActive-1] == 0
  signed char cc = 0;
  byte keyPressed = 0;

  byte keyStrobe = 0;        // 'KO' or 'KP' if a keyboard input line is active, i.e. dActive and keyPressed match in the key matrix
  unsigned int address = 0;  // PROGRAM COUNTER
  byte display = 1;          // Flag for display on
  char mask[MASK_LENGTH];

  byte showwork = 0;         // when 0, display flag controls LED, when 1, LED always on
  byte speed = 1;            // 0 - slow 1 - normal 2 - fast
  byte steptime = 148;       // the step() function will take this many microseconds to execute, uses a delay to eat remaining time if it finishes faster.
} SinclairData;

boolean resetinprogress = false;

void updateDisplay()
{
  byte displayon = SinclairData.display + SinclairData.showwork;

  bool dp = false; // needed for logic below, keep initialized to false
  bool dp1;
  bool dp2;

  byte showdigit;
  byte digitoff = 99;
  byte digiton = 99;
  byte digitpos = 0;

  switch (SinclairData.dActive)
  {
    case 1:
      digiton = (SinclairData.a[0] == 0) ? 99 : 10;
      break;

    case 2:
      digiton = SinclairData.a[4];
      break;

    case 3:
      digiton = SinclairData.a[5];
      break;

    case 4:
      digiton = SinclairData.a[6];
      break;

    case 5:
      digiton = SinclairData.a[7];
      break;

    case 6:
      digiton = SinclairData.a[8];
      break;

    case 7:
      digiton = (SinclairData.a[1] == 0) ? 99 : 10;
      break;

    case 8:
      digiton = SinclairData.a[2];
      break;

    case 9:
      digiton = SinclairData.a[3];
      break;
  }

  dp2 = !dp;   // !false

  // SINCLAIR behavior: turn decimal point on automatically at fixed position
  // dot stays on whether the rest of the display is on or off
  // this logic attempts to make optimizer do variable copy on dp1 = dp2 and dp1 = dp below.
  if (SinclairData.dActive == 2) {
    dp1 = dp2; // true
  }
  else
  {
    dp1 = dp;  // false
  }

  // attempt to have this block take same time to run whether a digit or blank is displayed
  if (displayon) {
    showdigit = digiton;
    digitpos = SinclairData.dActive - 1;
    dp = dp1; // true or false depending on digit shown
  }
  else {
    // SINCLAIR behavior: dot goes brighter when display is off.
    showdigit = digitoff;
    digitpos = 1;
    dp = dp2; // always true
  }

  byte segmentslit;
  segmentslit = outputDigit(showdigit, dp);
  //delayMicroseconds(segmentslit << 2);
  //delay(segmentslit);
  selectDigit(digitpos);

  // adjust case 1 for the following:
  // Sin 1 takes about 7.3 seconds.
  // Arccos .2 takes about 13.6 seconds
  // Arctan 1 takes about 6.6 seconds
  // Arccos of a very small value (e.g. 0.0001) goes into an almost-infinite loop and takes 1 minute, 38 seconds to complete

  if (resetinprogress == false)
  {
    switch (SinclairData.speed)
    {
      case 0:
        delayMicroseconds(2000);
        break;
      case 1:
        delayMicroseconds(155);
        break;
      case 2:
        delayMicroseconds(50);
        break;
    }
  }
  else
  {
    // SINCLAIR behavior: makes the display dim when C is pressed, SinclairData.steptime must be reduced below 148 for this delay to take effect.
    delayMicroseconds(50);
  }
}

void setup()
{
  // put your setup code here, to run once:

  //Serial.begin(250000); // Cannot even call this function if displays are enabled. conflicts with use of D0 and D1, see UseSoftwareSerial.ino

  allSegmentOff();
  allSegmentOutput();

  allDigitOff();
  allDigitOutput();

  char key = readKey();
  SinclairData.showwork = 1;  //takes effect only on 1,2,3

  switch (key)
  {
    case '0':
      displaySelfTest();
      SinclairData.speed = 1;
      SinclairData.showwork = 0; //normal speed, display off during calculations
      break;
    case '1':
      SinclairData.speed = 0;  //slow, display on
      break;
    case '2':
      SinclairData.speed = 1;  //normal, display on
      break;
    case '3':
      SinclairData.speed = 2;  //fast, display on
      break;
    default:
      SinclairData.speed = 1;
      SinclairData.showwork = 0; //normal speed, display off during calculations
      break;
  }
}

// timing the loop function using the micros() function, finds it executes every 340uS, for a 2.9kHz clock speed

void loop()
{
  // put your main code here, to run repeatedly:

  char key = readKey();

  SinclairData.keyStrobe = 0;

  if (key != 0)
  {
    byte dActive = SinclairData.dActive - 1;

    if (key == keysKN[dActive]) {
      SinclairData.keyStrobe = KN;
    }

    if (key == keysKO[dActive]) {
      SinclairData.keyStrobe = KO;
    }

    if (key == 'C') {
      if (resetinprogress == false) {
        SinclairData.address = 0;
        SinclairData.keyStrobe = 0;
        SinclairData.dActive = 1;
        SinclairData.steptime = 10; // speed up step() function so updateDisplay() function can control brightness via smaller delayMicrososeconds()
        resetinprogress = true;
      }
    }
  }

  updateDisplay();
  step();

  if (resetinprogress)
  {
    if (SinclairData.address > 8) {
      resetinprogress = false;
      SinclairData.steptime = 148;
    }
  }
}
