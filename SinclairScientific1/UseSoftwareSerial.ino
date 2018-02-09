SoftwareSerial mySerial(0, 1); // RX, TX

void outputlong(unsigned long v)
{
  // set the data rate for the SoftwareSerial port
  mySerial.begin(57600);

  mySerial.print("---"); //this will be corrupted

  mySerial.print("v:");
  mySerial.println(v);
}
