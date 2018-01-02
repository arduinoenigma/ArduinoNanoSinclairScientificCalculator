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

//ORIGINAL SINCLAIR DATA

//this.rom = objectCode;
//this.sinclair = sinclair;
// Important: Array order matches display order, not bit order.
// I.e. a[0] is high-order digit S10, a[10] is low-order digit S0.
//this.a = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]; // Register A
//this.b = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]; // Register B
//this.c = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]; // Register C
//this.af = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]; // Flags A
//this.bf = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]; // Flags B
// The states are called D1 to D10.  The last bit is never set
// d is sort of a combination of the 10-bit digit scan register and the 11-bit D-scan register.
// In reality, all 11 bits of the D-scan register are used, but at varying S cycles.
// The digit scan register is clocked at S9 phase 3. Thus 11 shifts of D-scan take the same time
// as 10 shifts of digit scan.
//this.d = [0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]; // D scan register, d[0] low for D1
// Currently active D value 1-11. d[dActive-1] == 0
//this.dActive = 1;
//this.cc = 0;
//this.ccMeaning = '';
//this.keyPressed = null;
// 'KO' or 'KP' if a keyboard input line is active, i.e. dActive and keyPressed match in the key matrix
//this.keyStrobe = 0;
//this.address = 0;
//this.display = 1; // Flag for display on
//this.mask = null;
//this.showDisplayScan = 0;
//this.idle = 0; // 1 if in idle loop
//this.speed = 'auto'; // Speed for updates
//this.fastStep = 1; // 1 makes SYNC, etc take 1 cycle
//this.power = 1; // On


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

boolean opsWithK(byte opcode) {
  return (LISTOPSWITHK & (1UL << opcode));
}

unsigned int getInstruction(unsigned int PC) {

  return pgm_read_word_near(objectCode + PC);
}

byte getMaskNum() {

  return getInstruction(SinclairData.address) & 0x0f;
}

void getMask() {

  unsigned int instruction = getInstruction(SinclairData.address);
  byte classBits = instruction >> 9;
  byte opcode = (instruction >> 4) & 0x1f;

  if (classBits == 3 || (classBits == 2 && opcode > 18 && opcode != 21 && opcode != 22))
  {
    byte maskno = getMaskNum();

    for (byte i = 0; i <= 10; i++)
    {
      char maskdigit = pgm_read_byte_near(masks[maskno] + i);

      if (maskdigit == ' ')
      {
        SinclairData.mask[i] = maskdigit;
      }
      else if (classBits == 3 && opsWithK(opcode))
      {
        // Register instruction
        SinclairData.mask[i] = maskdigit;
      }
      else
      {
        SinclairData.mask[i] = '*';
      }
    }
  }
  else
  {
    SinclairData.mask[0] = 0;
  }
}


void step() {

  Serial.println(F("step()"));

  unsigned int instruction = getInstruction(SinclairData.address);
  byte classBits = instruction >> 9;
  byte opcode = (instruction >> 4) & 0x1f;
  unsigned int nextAddress = SinclairData.address + 1;

  if (classBits == 3)
  {
    // Register instruction

    byte maskBits = instruction & 0xf;
    switch (opcode)
    {
      case 0: // AABA: A+B -> A
        this.add(this.model.a, this.model.b, this.model.a);
        break;
      case 1: // AAKA: A+K -> A
        this.add(this.model.a, this.getMask(), this.model.a);
        break;
      case 2: // AAKC: A+K -> C
        this.add(this.model.a, this.getMask(), this.model.c);
        break;
      case 3:
        if (sinclair)
        { // ACBB C+B -> B
          this.add(this.model.c, this.model.b, this.model.b);
        }
        else
        { // ABOA: B -> A
          this.copy(this.model.b, this.model.a);
        }
        break;
      case 4: // ABOC: B -> C
        this.copy(this.model.b, this.model.c);
        break;
      case 5: // ACKA: C+K -> A
        this.add(this.model.c, this.getMask(), this.model.a);
        break;
      case 6: // AKCB: C+K -> B
        this.add(this.model.c, this.getMask(), this.model.b);
        break;
      case 7: // SABA: A-B -> A
        this.sub(this.model.a, this.model.b, this.model.a);
        break;
      case 8: // SABC: A-B -> C
        this.sub(this.model.a, this.model.b, this.model.c);
        break;
      case 9: // SAKA: A-K -> A
        this.sub(this.model.a, this.getMask(), this.model.a);
        break;
      case 10: // SCBC: C-B -> C
        this.sub(this.model.c, this.model.b, this.model.c);
        break;
      case 11: // SCKC: C-K -> C
        this.sub(this.model.c, this.getMask(), this.model.c);
        break;
      case 12: // CAB: compare A-B
        this.compare(this.model.a, this.model.b);
        break;
      case 13: // CAK: compare A-K
        this.compare(this.model.a, this.getMask());
        break;
      case 14: // CCB: compare C-B
        this.compare(this.model.c, this.model.b);
        break;
      case 15: // CCK: compare C-K
        this.compare(this.model.c, this.getMask());
        break;
      case 16: // AKA: K -> A
        this.copy(this.getMask(), this.model.a);
        break;
      case 17: // AKB: K -> B
        this.copy(this.getMask(), this.model.b);
        break;
      case 18: // AKC: K -> C
        this.copy(this.getMask(), this.model.c);
        break;
      case 19: // EXAB: exchange A and B
        this.exchange(this.model.a, this.model.b);
        break;
      case 20: // SLLA: shift A left
        this.sll(this.model.a);
        break;
      case 21: // SLLB: shift B left
        this.sll(this.model.b);
        break;
      case 22: // SLLC: shift C left
        this.sll(this.model.c);
        break;
      case 23: // SRLA: shift A right
        this.srl(this.model.a);
        break;
      case 24: // SRLB: shift B right
        this.srl(this.model.b);
        break;
      case 25: // SRLC: shift C right
        this.srl(this.model.c);
        break;
      case 26: // AKCN: A+K -> A until key down on N or D11 [sic]
        // Patent says sets condition if key down, but real behavior
        // is to set condition if addition overflows (i.e. no key down)
        this.add(this.model.a, this.getMask(), this.model.a);
        if (this.model.keyStrobe == 'KN')
        {
          // Advance to next instruction
        }
        else if (this.model.dActive != 10)
        {
          // Hold at current instruction and continue scan
          nextAddress = this.model.address;
        }
        else
        {
          // For state d10, fall through
        }
        break;
      case 27:
        if (sinclair)
        { // SCBA C-B -> A
          this.sub(this.model.c, this.model.b, this.model.a);
        }
        else
        { // AAKAH A+K -> A hex
          this.add(this.model.a, this.getMask(), this.model.a, 1 /* hex */ );
          this.model.cc = 0;
          this.model.ccMeaning = '';
        }
        break;
      case 28:
        if (sinclair)
        { // SCKB C-K -> B
          this.sub(this.model.c, this.getMask(), this.model.b);
        }
        else
        { // SAKAH A-K -> A hex
          this.sub(this.model.a, this.getMask(), this.model.a, 1 /* hex */ );
          this.model.cc = 0;
          this.model.ccMeaning = '';
        }
        break;
      case 29: // ACKC: C+K -> C
        this.add(this.model.c, this.getMask(), this.model.c);
        break;
      case 30:
        if (sinclair)
        { // AABC A+B -> C
          this.add(this.model.a, this.model.b, this.model.c);
          break;
        }
      case 31:
        if (sinclair)
        { // ACBC C+B -> C
          this.add(this.model.c, this.model.b, this.model.c);
          break;
        }
      default:
        alert('Bad instruction ' + instruction);
        break;
    }
  }
  else if ((instruction >> 8) == 5)
  {
    // Flag instruction
    var maskBits = instruction & 0xf;
    switch (opcode)
    {
      case 16: // NOP
        break;
      case 17: // WAITDK: wait for display key
        this.model.display = 0;
        if (this.model.keyPressed == 'DK')
        {
          // Jump
          nextAddress = instruction & 0x1ff;
        }
        else
        {
          // Hold address until DK pressed
          nextAddress = this.model.address;
        }
        break;
      case 18: // WAITNO: wait for key or address register overflow
        if (this.model.keyStrobe)
        {
          // Jump
          nextAddress = instruction & 0x1ff;
        }
        else
        {
          // Hold address until key pressed or address overflow (TODO)
          nextAddress = this.model.address;
        }
        break;
      case 19: // SFB: set flag B
        this.writeFlag(this.model.bf, 1);
        break;
      case 20: // SFA: set flag A
        this.writeFlag(this.model.af, 1);
        break;
      case 21: // SYNC (SYNCH): hold address until end of D10
        if (this.model.dActive != 10)
        {
          nextAddress = this.model.address;
        }
        this.model.cc = 0;
        this.model.ccMeaning = '';
        break;
      case 22: // SCAN (SCANNO): wait for key
        this.model.display = 1; // Reset display power off latch
        if (this.model.keyStrobe)
        {
          this.model.cc = 1;
          this.model.ccMeaning = 'key';
        }
        else
        {
          this.model.cc = 0;
          this.model.ccMeaning = 'no key';
          if (this.model.dActive != 10)
          {
            // Hold address until end of D10
            nextAddress = this.model.address;
          }
        }
        break;
      case 23: // ZFB: zero flag B
        this.writeFlag(this.model.bf, 0);
        break;
      case 24: // ZFA: zero flag A
        this.writeFlag(this.model.af, 0);
        break;
      case 25: // TFB: test flag B
        this.testFlag(this.model.bf);
        break;
      case 26: // TFA: test flag A
        this.testFlag(this.model.af);
        break;
      case 27: // FFB: flip flag B
        this.writeFlag(this.model.bf, -1 /* flip */ );
        break;
      case 28: // FFA: flip flag A
        this.writeFlag(this.model.af, -1 /* flip */ );
        break;
      case 29: // CF: compare flags
        this.compareFlags(this.model.af, this.model.bf);
        break;
      case 30: // NOP
        break;
      case 31: // EXF: exchange flags
        this.exchange(this.model.af, this.model.bf);
        break;
      default:
        alert('Bad instruction ' + instruction);
        break;
    }
  }
  else if (classBits == 0)
  {
    // jump if reset: BIU, BIZ, BIGE, BINC, BIE, BET
    if (this.model.cc == 0)
    {
      nextAddress = instruction & 0x1ff;
    }
    this.model.cc = 0; // Clear after jump
    this.model.ccMeaning = '';
  }
  else if (classBits == 1)
  {
    // jump if set: BID, BIO, BILT, BIC, BINE
    if (this.model.cc == 1)
    {
      nextAddress = instruction & 0x1ff;
    }
    this.model.cc = 0; // Clear after jump
    this.model.ccMeaning = '';
  }
  else if ((instruction >> 7) == 8)
  {
    // Jump if key down on KO (BKO)
    if (this.model.keyStrobe == 'KO')
    {
      nextAddress = instruction & 0x1ff;
    }
    this.model.cc = 0; // Clear after jump
    this.model.ccMeaning = '';
  }
  else if ((instruction >> 7) == 9)
  {
    // Jump if key down on KP (BKP)
    if (this.model.keyStrobe == 'KP')
    {
      nextAddress = instruction & 0x1ff;
    }
    this.model.cc = 0; // Clear after jump
    this.model.ccMeaning = '';
  }
  else
  {
    alert('Bad instruction code ' + instruction);
  }
  this.model.address = nextAddress;
  // Put the mask for the next instruction in the model for display
  this.model.mask = this.getMask();
  // Update D state
  this.updateD();
}


add (src1, src2, dst, hex)
{
  var carry = 0;
  var maskVec = this.getMask();
  for (var i = 10; i >= 0; i--)
  {
    if (maskVec[i] == = ' ')
    {
      // masked out
      continue;
    }
    else
    {
      var result = src1[i] + src2[i] + carry;
      if (!hex && result >= 10)
      {
        result -= 10;
        carry = 1;
      }
      else if (hex && result >= 16)
      {
        result -= 16;
        carry = 1;
      }
      else
      {
        carry = 0;
      }
      dst[i] = result;
    }
  }
  if (carry)
  {
    this.model.cc = carry;
    this.model.ccMeaning = carry ? 'overflow' : 'no overflow';
  }
};




void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  /*
    test_setBit();
    test_getBit();
    test_ListAllOpsWithK();
    test_ListProgram();
    test_DisplaySinclairData();
  */

  step();

}

void loop() {
  // put your main code here, to run repeatedly:

}
