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
// SoftwareSerial is needed instead of the hardware Serial since D0/D1 are used to select digits 0 and 1
//

SoftwareSerial mySerial(0, 1); // RX, TX

void outputlong(unsigned long v)
{
  // set the data rate for the SoftwareSerial port
  mySerial.begin(57600);

  mySerial.print("---"); //this will be corrupted

  mySerial.print("v:");
  mySerial.println(v);
}

void outputlong(unsigned long v1, unsigned long v2)
{
  // set the data rate for the SoftwareSerial port
  mySerial.begin(57600);

  mySerial.println("---"); //this will be corrupted

  mySerial.print("v1:");
  mySerial.println(v1);

  mySerial.print("v2:");
  mySerial.println(v2);
}
