void displayInstruction(byte dummy)
{

}

void displayArray(signed char src1[]) {
  for (byte i = 0; i < 11; i++)
  {
    Serial.print((int)src1[i]);
    Serial.print(F(" "));
  }
  Serial.println(F(""));
}

void displayInstruction1(byte instructionid)
{
  Serial.print(F(" ("));
  Serial.print(instructionid);
  Serial.print(F(":"));
  switch (instructionid)
  {
    case 1:
      Serial.print(F("AABA: A+B -> A"));
      break;

    case 2:
      Serial.print(F("AABA: A+B -> A"));
      break;

    case 3:
      Serial.print(F("AAKC: A+K -> C"));
      break;

    case 4:
      Serial.print(F("ACBB C+B -> B"));
      break;

    case 5:
      Serial.print(F("ABOA: B -> A"));
      break;

    case 6:
      Serial.print(F("ABOC: B -> C"));
      break;

    case 7:
      Serial.print(F("ACKA: C+K -> A"));
      break;

    case 8:
      Serial.print(F("AKCB: C+K -> B"));
      break;

    case 9:
      Serial.print(F("SABA: A-B -> A"));
      break;

    case 10:
      Serial.print(F("SABC: A-B -> C"));
      break;

    case 11:
      Serial.print(F("SAKA: A-K -> A"));
      break;

    case 12:
      Serial.print(F("SCBC: C-B -> C"));
      break;

    case 13:
      Serial.print(F("SCKC: C-K -> C"));
      break;

    case 14:
      Serial.print(F("CAB: compare A-B"));
      break;

    case 15:
      Serial.print(F("CAK: compare A-K"));
      break;

    case 16:
      Serial.print(F("CCB: compare C-B"));
      break;

    case 17:
      Serial.print(F("CCK: compare C-K"));
      break;

    case 18:
      Serial.print(F("AKA: K -> A"));
      break;

    case 19:
      Serial.print(F("AKB: K -> B"));
      break;

    case 20:
      Serial.print(F("AKC: K -> C"));
      break;

    case 21:
      Serial.print(F("EXAB: exchange A and B"));
      break;

    case 22:
      Serial.print(F("SLLA: shift A left"));
      break;

    case 23:
      Serial.print(F("SLLB: shift B left"));
      break;

    case 24:
      Serial.print(F("SLLC: shift C left"));
      break;

    case 25:
      Serial.print(F("SRLA: shift A right"));
      break;

    case 26:
      Serial.print(F("SRLB: shift B right"));
      break;

    case 27:
      Serial.print(F("AKCN: A+K -> A until key down on N or D11 [sic] (ADVANCE)"));
      break;

    case 28:
      Serial.print(F("AKCN: A+K -> A until key down on N or D11 [sic] (HOLD)"));
      break;

    case 29:
      Serial.print(F("AKCN: A+K -> A until key down on N or D11 [sic] (D10 FALLTHROUGH)"));
      break;

    case 30:
      Serial.print(F("SCBA C-B -> A"));
      break;

    case 31:
      Serial.print(F("AAKAH A+K -> A hex"));
      break;

    case 32:
      Serial.print(F("SCKB C-K -> B"));
      break;

    case 33:
      Serial.print(F("SAKAH A-K -> A hex"));
      break;

    case 34:
      Serial.print(F("ACKC: C+K -> C"));
      break;

    case 35:
      Serial.print(F("AABC A+B -> C"));
      break;

    case 36:
      Serial.print(F("ACBC C+B -> C"));
      break;

    case 37:
      Serial.print(F("Bad instruction"));
      break;

    case 38:
      Serial.print(F("NOP"));
      break;

    case 39:
      Serial.print(F("WAITDK: wait for display key DK (JUMP)"));
      break;

    case 40:
      Serial.print(F("WAITDK: wait for display key DK (HOLD)"));
      break;

    case 41:
      Serial.print(F("WAITNO: wait for key or address register overflow (JUMP)"));
      break;

    case 42:
      Serial.print(F("WAITNO: wait for key or address register overflow (HOLDING)"));
      break;

    case 43:
      Serial.print(F("SFB: set flag B"));
      break;

    case 44:
      Serial.print(F("SFA: set flag A"));
      break;

    case 45:
      Serial.print(F("SYNC (SYNCH): hold address until end of D10"));
      break;

    case 46:
      Serial.print(F("SCAN (SCANNO): wait for key (KEY)"));
      break;

    case 47:
      Serial.print(F("SCAN (SCANNO): wait for key (NO KEY)"));
      break;

    case 48:
      Serial.print(F("ZFB: zero flag B"));
      break;

    case 49:
      Serial.print(F("ZFA: zero flag A"));
      break;

    case 50:
      Serial.print(F("TFB: test flag B"));
      break;

    case 51:
      Serial.print(F("TFA: test flag A"));
      break;

    case 52:
      Serial.print(F("FFB: flip flag B"));
      break;

    case 53:
      Serial.print(F("CF: compare flags"));
      break;

    case 54:
      Serial.print(F("NOP"));
      break;

    case 55:
      Serial.print(F("EXF: exchange flags"));
      break;

    case 56:
      Serial.print(F("Bad instruction"));
      break;

    case 57:
      Serial.print(F("Jump if reset: BIU, BIZ, BIGE, BINC, BIE, BET "));
      break;

    case 58:
      Serial.print(F("JUMP IF RESET TAKEN"));
      break;

    case 59:
      Serial.print(F("Jump if set: BID, BIO, BILT, BIC, BINE"));
      break;

    case 60:
      Serial.print(F("JUMP IF SET TAKEN"));
      break;

    case 61:
      Serial.print(F("Jump if key down on KO (BKO)"));
      break;

    case 62:
      Serial.print(F("JUMP IF KEY DOWN ON KO TAKEN"));
      break;

    case 63:
      Serial.print(F("Jump if key down on KP (BKP)"));
      break;

    case 64:
      Serial.print(F("JUMP IF KEY DOWN ON KP TAKEN"));
      break;

    case 65:
      Serial.print(F("Bad instruction code"));
      break;

    case 66:
      Serial.print(F("SRLC: shift C right"));
      break;

    case 67:
      Serial.print(F("FFA: flip flag A"));
      break;

    default:
      Serial.print(F("UNKNOWN INSTRUCTION ID"));
      break;
  }
  Serial.print(F(")"));
}
