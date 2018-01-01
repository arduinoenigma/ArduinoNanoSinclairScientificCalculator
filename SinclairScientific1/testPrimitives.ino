void test_ListAllOpsWithK() {
  for (byte i = 0; i < 31; i++)
  {
    if (opsWithK(i)) {
      Serial.print(i);
      Serial.println(F(" has K"));
    }
  }
}

void test_ListProgram() {
  unsigned int c = 0;

  do
  {
    Serial.println(getInstruction(c++));
  }
  while (c < 320);
}

void test_DisplaySinclairData() {
  Serial.print("OwK:");
  Serial.print(LISTOPSWITHK);
  Serial.println("/OwK:");

  Serial.print("D:");
  Serial.print(SinclairData.d);
  Serial.println("/D:");
}
