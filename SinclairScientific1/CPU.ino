// Arduino Port of
//
// TI calculator simulator
// Ken Shirriff, http://righto.com/ti
// Based on patent US3934233
//
// The goal of this project is to run the following simulator: http://righto.com/sinclair
// on an arduino nano powered custom pcb resembling the original Sinclair Scientific Calculator
// @arduinoenigma 2018

boolean opsWithK(byte opcode) {

  return (LISTOPSWITHK & (1UL << opcode));
}

unsigned int getInstruction(unsigned int PC) {

  return pgm_read_word_near(objectCode + PC);
}

byte getMaskNum() {

  return getInstruction(SinclairData.address) & 0x0f;
}

char *getMask() {

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
        SinclairData.mask[i] = maskdigit - '0';;
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

  return SinclairData.mask;
}

void add(signed char src1[], signed char src2[], signed char dst[], bool hex = false)
{
  byte carry = 0;
  getMask();
  for (signed char i = 10; i >= 0; i--)
  {
    if (SinclairData.mask[i] == ' ')
    {
      // masked out
      // continue;
    }
    else
    {
      signed char result = src1[i] + src2[i] + carry;
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
    SinclairData.cc = carry;
    //SinclairData.ccMeaning = carry ? 'overflow' : 'no overflow';
  }
}

void sub(signed char src1[], signed char src2[], signed char dst[], bool hex = false)
{
  byte borrow = 0;
  getMask();
  for (signed char i = 10; i >= 0; i--)
  {
    if (SinclairData.mask[i] == ' ')
    {
      // masked out
      // continue;
    }
    else
    {
      signed char result = src1[i] - src2[i] - borrow;
      if (result < 0)
      {
        result += hex ? 16 : 10;
        borrow = 1;
      }
      else
      {
        borrow = 0;
      }
      dst[i] = result;
    }
  }
  if (borrow)
  {
    SinclairData.cc = borrow;
    //SinclairData.ccMeaning = borrow ? 'borrow' : 'no borrow';
  }
}

void compare(signed char src1[], signed char src2[])
{
  signed char tmp[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  sub(src1, src2, tmp);
  // Compare sets condition if not borrow
  //SinclairData.ccMeaning = SinclairData.cc ? "less than" : "not less than";
}

void copy(signed char src[], signed char dst[])
{
  getMask();
  for (signed char i = 10; i >= 0; i--)
  {
    if (SinclairData.mask[i] == ' ')
    {
      // masked out
      // continue;
    }
    else
    {
      dst[i] = src[i];
    }
  }
}

void sll(signed char src[])
{
  getMask();
  signed char digit = 0;
  for (signed char i = 10; i >= 0; i--)
  {
    if (SinclairData.mask[i] == ' ')
    {
      // masked out
      // continue;
    }
    else
    {
      signed char newdigit = src[i];
      src[i] = digit;
      digit = newdigit;
    }
  }
}

void srl(signed char src[])
{
  getMask();
  signed char digit = 0;
  for (signed char i = 0; i <= 10; i++)
  {
    if (SinclairData.mask[i] == ' ')
    {
      // masked out
      // continue;
    }
    else
    {
      signed char newdigit = src[i];
      src[i] = digit;
      digit = newdigit;
    }
  }
}

void writeFlag(signed char dest[], signed char val)
{
  getMask();
  for (signed char i = 10; i >= 0; i--)
  {
    if (SinclairData.mask[i] == ' ')
    {
      // masked out
      // continue;
    }
    else
    {
      // Flip dst if val == -1, otherwise set to val
      dest[i] = (val < 0) ? (1 - dest[i]) : val;
    }
  }
}

void compareFlags(signed char src1[], signed char src2[])
{
  signed char cc = 0;
  getMask();
  for (signed char i = 10; i >= 0; i--)
  {
    if (SinclairData.mask[i] == ' ')
    {
      // masked out
      // continue;
    }
    else
    {
      if (src1[i] != src2[i])
      {
        cc = 1;
      }
    }
  }
  if (cc)
  {
    SinclairData.cc = 1;
    //SinclairData.ccMeaning = 'flags not equal';
  }
}

void exchange(signed char src1[], signed char src2[])
{
  getMask();
  for (signed char i = 10; i >= 0; i--)
  {
    if (SinclairData.mask[i] == ' ')
    {
      // masked out
      // continue;
    }
    else
    {
      signed char t = src1[i];
      src1[i] = src2[i];
      src2[i] = t;
    }
  }
}

void testFlag(signed char src[])
{
  signed char cc = 0;
  getMask();
  for (signed char i = 10; i >= 0; i--)
  {
    if (SinclairData.mask[i] == ' ')
    {
      // masked out
      // continue;
    }
    else
    {
      if (src[i])
      {
        cc = 1;
      }
    }
  }
  /* Only update cc if bit set */
  if (cc)
  {
    SinclairData.cc = cc;
    //SinclairData.ccMeaning = 'flag set';
  }
}

void updateD()
{
  for (signed char i = 10; i >= 0; i--)
  {
    SinclairData.d[i] = 1;
  }

  SinclairData.dActive += 1;
  if (SinclairData.dActive > 10)
  {
    SinclairData.dActive = 1;
  }
  SinclairData.d[SinclairData.dActive - 1] = 0;
}

void step()
{
  //Serial.print(F("addr:"));
  //Serial.print(SinclairData.address);

  unsigned int instruction = getInstruction(SinclairData.address);
  byte classBits = instruction >> 9;
  byte opcode = (instruction >> 4) & 0x1f;
  unsigned int nextAddress = SinclairData.address + 1;

  //Serial.print(F(" data:"));
  //Serial.print(opcode);
  //Serial.print(F(" classbits:"));
  //Serial.print(classBits);

  if (classBits == 3)
  {
    // Register instruction
    byte maskBits = instruction & 0xf;
    switch (opcode)
    {
      case 0: // AABA: A+B -> A
        displayInstruction(1);
        add(SinclairData.a, SinclairData.b, SinclairData.a);
        break;
      case 1: // AAKA: A+K -> A
        displayInstruction(2);
        add(SinclairData.a, getMask(), SinclairData.a);
        break;
      case 2: // AAKC: A+K -> C
        displayInstruction(3);
        add(SinclairData.a, getMask(), SinclairData.c);
        break;
      case 3:
        if (SinclairData.sinclair)
        { // ACBB C+B -> B
          displayInstruction(4);
          add(SinclairData.c, SinclairData.b, SinclairData.b);
        }
        else
        { // ABOA: B -> A
          displayInstruction(5);
          copy(SinclairData.b, SinclairData.a);
        }
        break;
      case 4: // ABOC: B -> C
        displayInstruction(6);
        copy(SinclairData.b, SinclairData.c);
        break;
      case 5: // ACKA: C+K -> A
        displayInstruction(7);
        add(SinclairData.c, getMask(), SinclairData.a);
        break;
      case 6: // AKCB: C+K -> B
        displayInstruction(8);
        add(SinclairData.c, getMask(), SinclairData.b);
        break;
      case 7: // SABA: A-B -> A
        displayInstruction(9);
        sub(SinclairData.a, SinclairData.b, SinclairData.a);
        break;
      case 8: // SABC: A-B -> C
        displayInstruction(10);
        sub(SinclairData.a, SinclairData.b, SinclairData.c);
        break;
      case 9: // SAKA: A-K -> A
        displayInstruction(11);
        sub(SinclairData.a, getMask(), SinclairData.a);
        break;
      case 10: // SCBC: C-B -> C
        displayInstruction(12);
        sub(SinclairData.c, SinclairData.b, SinclairData.c);
        break;
      case 11: // SCKC: C-K -> C
        displayInstruction(13);
        sub(SinclairData.c, getMask(), SinclairData.c);
        break;
      case 12: // CAB: compare A-B
        displayInstruction(14);
        compare(SinclairData.a, SinclairData.b);
        break;
      case 13: // CAK: compare A-K
        displayInstruction(15);
        compare(SinclairData.a, getMask());
        break;
      case 14: // CCB: compare C-B
        displayInstruction(16);
        compare(SinclairData.c, SinclairData.b);
        break;
      case 15: // CCK: compare C-K
        displayInstruction(17);
        compare(SinclairData.c, getMask());
        break;
      case 16: // AKA: K -> A
        displayInstruction(18);
        copy(getMask(), SinclairData.a);
        break;
      case 17: // AKB: K -> B
        displayInstruction(19);
        copy(getMask(), SinclairData.b);
        break;
      case 18: // AKC: K -> C
        displayInstruction(20);
        copy(getMask(), SinclairData.c);
        break;
      case 19: // EXAB: exchange A and B
        displayInstruction(21);
        exchange(SinclairData.a, SinclairData.b);
        break;
      case 20: // SLLA: shift A left
        displayInstruction(22);
        sll(SinclairData.a);
        break;
      case 21: // SLLB: shift B left
        displayInstruction(23);
        sll(SinclairData.b);
        break;
      case 22: // SLLC: shift C left
        displayInstruction(24);
        sll(SinclairData.c);
        break;
      case 23: // SRLA: shift A right
        displayInstruction(25);
        srl(SinclairData.a);
        break;
      case 24: // SRLB: shift B right
        displayInstruction(26);
        srl(SinclairData.b);
        break;
      case 25: // SRLC: shift C right
        displayInstruction(66);
        srl(SinclairData.c);
        break;
      case 26: // AKCN: A+K -> A until key down on N or D11 [sic]
        // Patent says sets condition if key down, but real behavior
        // is to set condition if addition overflows (i.e. no key down)
        SinclairData.display = 0;
        add(SinclairData.a, getMask(), SinclairData.a);
        if (SinclairData.keyStrobe == KN)
        {
          displayInstruction(27);
          // Advance to next instruction
        }
        else if (SinclairData.dActive != 10)
        {
          displayInstruction(28);
          // Hold at current instruction and continue scan
          nextAddress = SinclairData.address;
        }
        else
        {
          displayInstruction(29);
          // For state d10, fall through
        }
        break;
      case 27:
        if (SinclairData.sinclair)
        { // SCBA C-B -> A
          displayInstruction(30);
          sub(SinclairData.c, SinclairData.b, SinclairData.a);
        }
        else
        { // AAKAH A+K -> A hex
          displayInstruction(31);
          add(SinclairData.a, getMask(), SinclairData.a, 1 /* hex */ );
          SinclairData.cc = 0;
          //SinclairData.ccMeaning = '';
        }
        break;
      case 28:
        if (SinclairData.sinclair)
        { // SCKB C-K -> B
          displayInstruction(32);
          sub(SinclairData.c, getMask(), SinclairData.b);
        }
        else
        { // SAKAH A-K -> A hex
          displayInstruction(33);
          sub(SinclairData.a, getMask(), SinclairData.a, 1 /* hex */ );
          SinclairData.cc = 0;
          //SinclairData.ccMeaning = '';
        }
        break;
      case 29: // ACKC: C+K -> C
        displayInstruction(34);
        add(SinclairData.c, getMask(), SinclairData.c);
        break;
      case 30:
        if (SinclairData.sinclair)
        { // AABC A+B -> C
          displayInstruction(35);
          add(SinclairData.a, SinclairData.b, SinclairData.c);
          break;
        }
      case 31:
        if (SinclairData.sinclair)
        { // ACBC C+B -> C
          displayInstruction(36);
          add(SinclairData.c, SinclairData.b, SinclairData.c);
          break;
        }
      default:
        //bad instruction
        //alert('Bad instruction ' + instruction);
        displayInstruction(37);
        break;
    }
  }
  else if ((instruction >> 8) == 5)
  {
    // Flag instruction
    byte maskBits = instruction & 0xf;
    switch (opcode)
    {
      case 16: // NOP
        displayInstruction(38);
        break;
      case 17: // WAITDK: wait for display key
        SinclairData.display = 0;
        if (SinclairData.keyPressed == DK)
        {
          // Jump
          displayInstruction(39);
          nextAddress = instruction & 0x1ff;
        }
        else
        {
          // Hold address until DK pressed
          displayInstruction(40);
          nextAddress = SinclairData.address;
        }
        break;
      case 18: // WAITNO: wait for key or address register overflow
        if (SinclairData.keyStrobe)
        {
          // Jump
          displayInstruction(41);
          nextAddress = instruction & 0x1ff;
        }
        else
        {
          // Hold address until key pressed or address overflow (TODO)
          displayInstruction(42);
          nextAddress = SinclairData.address;
        }
        break;
      case 19: // SFB: set flag B
        displayInstruction(43);
        writeFlag(SinclairData.bf, 1);
        break;
      case 20: // SFA: set flag A
        displayInstruction(44);
        writeFlag(SinclairData.af, 1);
        break;
      case 21: // SYNC (SYNCH): hold address until end of D10
        displayInstruction(45);
        if (SinclairData.dActive != 10)
        {
          nextAddress = SinclairData.address;
        }
        SinclairData.cc = 0;
        //SinclairData.ccMeaning = '';
        break;
      case 22: // SCAN (SCANNO): wait for key
        SinclairData.display = 1; // Reset display power off latch
        if (SinclairData.keyStrobe)
        {
          displayInstruction(46);
          SinclairData.cc = 1;
          //SinclairData.ccMeaning = 'key';
        }
        else
        {
          displayInstruction(47);
          SinclairData.cc = 0;
          //SinclairData.ccMeaning = 'no key';
          if (SinclairData.dActive != 10)
          {
            // Hold address until end of D10
            nextAddress = SinclairData.address;
          }
        }
        break;
      case 23: // ZFB: zero flag B
        displayInstruction(48);
        writeFlag(SinclairData.bf, 0);
        break;
      case 24: // ZFA: zero flag A
        displayInstruction(49);
        writeFlag(SinclairData.af, 0);
        break;
      case 25: // TFB: test flag B
        displayInstruction(50);
        testFlag(SinclairData.bf);
        break;
      case 26: // TFA: test flag A
        displayInstruction(51);
        testFlag(SinclairData.af);
        break;
      case 27: // FFB: flip flag B
        displayInstruction(52);
        writeFlag(SinclairData.bf, -1 /* flip */ );
        break;
      case 28: // FFA: flip flag A
        displayInstruction(67);
        writeFlag(SinclairData.af, -1 /* flip */ );
        break;
      case 29: // CF: compare flags
        displayInstruction(53);
        compareFlags(SinclairData.af, SinclairData.bf);
        break;
      case 30: // NOP
        displayInstruction(54);
        break;
      case 31: // EXF: exchange flags
        displayInstruction(55);
        exchange(SinclairData.af, SinclairData.bf);
        break;
      default:
        //bad instruction
        //alert('Bad instruction ' + instruction);
        displayInstruction(56);
        break;
    }
  }
  else if (classBits == 0)
  {
    // jump if reset: BIU, BIZ, BIGE, BINC, BIE, BET
    displayInstruction(57);
    if (SinclairData.cc == 0)
    {
      displayInstruction(58);
      nextAddress = instruction & 0x1ff;
    }
    SinclairData.cc = 0; // Clear after jump
    //SinclairData.ccMeaning = '';
  }
  else if (classBits == 1)
  {
    // jump if set: BID, BIO, BILT, BIC, BINE
    displayInstruction(59);
    if (SinclairData.cc == 1)
    {
      displayInstruction(60);
      nextAddress = instruction & 0x1ff;
    }
    SinclairData.cc = 0; // Clear after jump
    //SinclairData.ccMeaning = '';
  }
  else if ((instruction >> 7) == 8)
  {
    // Jump if key down on KO (BKO)
    displayInstruction(61);
    if (SinclairData.keyStrobe == KO)
    {
      SinclairData.display = 0;
      displayInstruction(62);
      nextAddress = instruction & 0x1ff;
    }
    SinclairData.cc = 0; // Clear after jump
    //SinclairData.ccMeaning = '';
  }
  else if ((instruction >> 7) == 9)
  {
    // Jump if key down on KP (BKP)
    displayInstruction(63);
    if (SinclairData.keyStrobe == KP)
    {
      SinclairData.display = 0;
      displayInstruction(64);
      nextAddress = instruction & 0x1ff;
    }
    SinclairData.cc = 0; // Clear after jump
    //SinclairData.ccMeaning = '';
  }
  else
  {
    displayInstruction(65);
    //bad instruction
    //alert('Bad instruction code ' + instruction);
  }
  SinclairData.address = nextAddress;
  // Put the mask for the next instruction in the model for display
  //SinclairData.mask = getMask();
  getMask();
  // Update D state
  updateD();

  displayInstruction(68); // if printing is enabled, do a println after executing a line of code
}
