// User defined settings
#define  DEFAULTMODE  ATMEGA  // If running in non-interactive mode, you need to set this to ATMEGA, TINY2313, or HVSP.
#define  INTERACTIVE  0       // Set this to 0 to disable interactive mode
#define  BURN_EFUSE   1       // Set this to 1 to enable burning extended fuse byte
#define  BAUD         115200  // Serial port rate at which to talk to PC

// If interactive mode is off, these fuse settings are used instead of user prompted values
#define  HFUSE        0xDD  // default for ATmega328P per datasheet p282-284 0xD9 UNO=0xDE Duemilanove328=0xDA
#define  LFUSE        0xE2  // default for ATmega328P per datasheet p282-284 0x62 UNO=0xFF
#define  EFUSE        0xFF  // default for ATmega328P per datasheet p282-284 0xFF UNO=0x05(effective FD)
// default low fuse  for ATtiny85 per datasheet p148-149 0x62 digispark=0xF1
// default high fuse for ATtiny85 per datasheet p148-149 0xDF digispark=0xD5
// default ext fuse  for ATtiny85 per datasheet p148-149 0xFF digispark=0xFE
// default low fuse  for ATtiny2313 per datasheet p159-160 0x64
// default high fuse for ATtiny2313 per datasheet p159-160 0xDF
// default ext fuse  for ATtiny2313 per datasheet p159-160 0xFF

// Pin Assignments
#define DATA0 A1
#define DATA1 1
#define DATA2 2
#define DATA3 3
#define DATA4 4
#define DATA5 5
#define DATA6 6
#define DATA7 7
#define  VCC     12
#define  RDY     13    // RDY/!BSY signal from target
#define  OE      11
#define  WR      10
#define  BS1     A2
#define  XA0     8
#define  XA1     A4
#define  RST     A0    // 12V to target reset enable (!12V_EN)
#define  XTAL1   A3
#define  PAGELM  A5
#define  BS2M    9

// Pin assignment adjustments for Tiny
#define  PAGELT  BS1 // PAGEL is set to same pin as BS1 for ATtiny2313
#define  BS2T    XA1 // BS2 is set to same pin as XA1 for ATtiny2313
// Pin assignments for HVSP mode
#define  SCI    BS1
#define  SDO    RDY
#define  SII    XA0
#define  SDI    XA1

// Serial instructions for HVSP mode
// Based on the ATtiny85 datasheet Table 20-16 pg. 163-165.
// These instructions don't contain the necessary zero padding, which is added later.

// LFUSE
#define HVSP_READ_LFUSE_DATA     B00000100  // For the commands we are interested in
#define HVSP_READ_LFUSE_INSTR1   B01001100  // only the 1st instruction contains a fixed data packet.
#define HVSP_READ_LFUSE_INSTR2   B01101000  // Instructions 2-3 have data = all zeros.
#define HVSP_READ_LFUSE_INSTR3   B01101100

#define HVSP_WRITE_LFUSE_DATA    B01000000  // For the write instructions, the data contents
#define HVSP_WRITE_LFUSE_INSTR1  B01001100  // for instruction 2 are the desired fuse bits.
#define HVSP_WRITE_LFUSE_INSTR2  B00101100  // Instructions 3-4 have data = all zeros.
#define HVSP_WRITE_LFUSE_INSTR3  B01100100
#define HVSP_WRITE_LFUSE_INSTR4  B01101100

// HFUSE
#define HVSP_READ_HFUSE_DATA     B00000100
#define HVSP_READ_HFUSE_INSTR1   B01001100
#define HVSP_READ_HFUSE_INSTR2   B01111010
#define HVSP_READ_HFUSE_INSTR3   B01111110

#define HVSP_WRITE_HFUSE_DATA    B01000000
#define HVSP_WRITE_HFUSE_INSTR1  B01001100
#define HVSP_WRITE_HFUSE_INSTR2  B00101100
#define HVSP_WRITE_HFUSE_INSTR3  B01110100
#define HVSP_WRITE_HFUSE_INSTR4  B01111100

// EFUSE
// Note: not all ATtiny's have an EFUSE
#define HVSP_READ_EFUSE_DATA     B00000100
#define HVSP_READ_EFUSE_INSTR1   B01001100
#define HVSP_READ_EFUSE_INSTR2   B01101010
#define HVSP_READ_EFUSE_INSTR3   B01101110

#define HVSP_WRITE_EFUSE_DATA    B01000000
#define HVSP_WRITE_EFUSE_INSTR1  B01001100
#define HVSP_WRITE_EFUSE_INSTR2  B00101100
#define HVSP_WRITE_EFUSE_INSTR3  B01100110
#define HVSP_WRITE_EFUSE_INSTR4  B01101110

// Internal definitions
enum modelist { ATMEGA, TINY2313, HVSP };
enum fusesel { LFUSE_SEL, HFUSE_SEL, EFUSE_SEL };

// Global variables
byte mode = DEFAULTMODE;  // programming mode
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
long val1;
byte base = 0;
byte hfuse = HFUSE;
byte lfuse = LFUSE;
byte efuse = EFUSE;

// These pin assignments change depending on which chip is being programmed,
// so they are put into variables
byte PAGEL = PAGELM;
byte BS2 = BS2M;

#define ATMega8H 0xDD
#define ATMega8L 0xE2
#define ATMega8E 0xFF

#define ATMega328PH 0xDE
#define ATMega328PL 0xFF
#define ATMega328PE 0xFD

#define ATMega328H 0xDB
#define ATMega328L 0xFF
#define ATMega328E 0xFD

#define ATtiny13H 0xFF
#define ATtiny13L 0x6A

#define ATtiny85H 0xDD
#define ATtiny85L 0xE2
#define ATtiny85E 0xFF

void setup() { // run once, when the sketch starts
  // reserve bytes for the inputString
  inputString.reserve(26);

  // Set up control lines for HV parallel programming

  // clear digital pins 0-7
  digitalWrite(DATA0, 0);
  digitalWrite(DATA1, 0);
  digitalWrite(DATA2, 0);
  digitalWrite(DATA3, 0);
  digitalWrite(DATA4, 0);
  digitalWrite(DATA5, 0);
  digitalWrite(DATA6, 0);
  digitalWrite(DATA7, 0);

  //  set digital pins 0-7 as inputs for now
  pinMode(DATA0, INPUT);
  pinMode(DATA1, INPUT);
  pinMode(DATA2, INPUT);
  pinMode(DATA3, INPUT);
  pinMode(DATA4, INPUT);
  pinMode(DATA5, INPUT);
  pinMode(DATA6, INPUT);
  pinMode(DATA7, INPUT);

  pinMode(VCC, OUTPUT);
  pinMode(RDY, INPUT);
  pinMode(OE, OUTPUT);
  pinMode(WR, OUTPUT);
  pinMode(BS1, OUTPUT);
  pinMode(XA0, OUTPUT);
  pinMode(XA1, OUTPUT);
  pinMode(PAGEL, OUTPUT);
  pinMode(RST, OUTPUT);  // enable signal for DC-DC converter that generates +12V !RESET
  pinMode(BS2, OUTPUT);
  pinMode(XTAL1, OUTPUT);

  // Initialize output pins as needed
  digitalWrite(RST, HIGH);  // Turn off 12V step-up converter (non-inverting, unlike original circuit)
  digitalWrite(VCC, LOW);

  Serial.begin(BAUD);  // Open serial port
  delay(50);
#if defined __AVR_ATmega32U4__ && INTERACTIVE == 1
  // if Leonardo or similar wait for serial monitor connection
  while (!Serial) ;
#endif
  while (Serial.readString() != "#START")
  {
    Serial.println("#ready");
    delay(50);
  }
}  // end of setup

void loop() {  // run over and over again

  byte read_hfuse, read_lfuse, read_efuse;  // fuses read from target for verify
  // variable to store what to do next, options
  // 0=display choices
  // 1=prompt for a command
  // 2=do nothing and expect further input
  // 3=read fuses from target AVR
  // 4=write fuses to target AVR
  static int doNext = 0;
#if (INTERACTIVE == 0)
  if (doNext == 0) {
    doNext = 2;  //non-interactive, just write the fuses, change to 3 to just read the fuses
  }
#endif

  if (doNext == 0) {  // 0=display choices
    doNext = 2;
  }
  // 2=do nothing and expect further input, no routine for it, just skip through the loop

  if (doNext == 3) {  // 3=read fuses from target AVR
    Serial.end();
    delay(50);
    // Initialize pins to enter programming mode

    // clear digital pins 0-7
    digitalWrite(DATA0, 0);
    digitalWrite(DATA1, 0);
    digitalWrite(DATA2, 0);
    digitalWrite(DATA3, 0);
    digitalWrite(DATA4, 0);
    digitalWrite(DATA5, 0);
    digitalWrite(DATA6, 0);
    digitalWrite(DATA7, 0);

    // set digital pins 0-7 as inputs for now
    pinMode(DATA0, INPUT);
    pinMode(DATA1, INPUT);
    pinMode(DATA2, INPUT);
    pinMode(DATA3, INPUT);
    pinMode(DATA4, INPUT);
    pinMode(DATA5, INPUT);
    pinMode(DATA6, INPUT);
    pinMode(DATA7, INPUT);

    digitalWrite(PAGEL, LOW);
    digitalWrite(XA1, LOW);
    digitalWrite(XA0, LOW);
    digitalWrite(BS1, LOW);
    digitalWrite(BS2, LOW);
    digitalWrite(WR, LOW);  // ATtiny2313 needs this to be low to enter programming mode, ATmega doesn't care
    digitalWrite(OE, LOW);

    if (mode == HVSP) {
      digitalWrite(SDI, LOW);  // set necessary pin values to enter programming mode
      digitalWrite(SII, LOW);
      pinMode(SDO, OUTPUT);    // SDO is same as RDY pin
      digitalWrite(SDO, LOW);  // needs to be low to enter programming mode
    }

    // Enter programming mode
    digitalWrite(VCC, HIGH);  // Apply VCC to start programming process
    delayMicroseconds(80);
    digitalWrite(RST, LOW);   // Apply 12V to !RESET thru transistor

    if (mode == HVSP) {
      // reset SDO after short delay, longer leads to logic contention because target sets SDO high after entering programming mode
      delayMicroseconds(1);   // datasheet says 10us, 1us is needed to avoid drive contention on SDO
      pinMode(SDO, INPUT);    // set to input to avoid logic contention
    }

    delayMicroseconds(10);  // Give lots of time for part to enter programming mode
    digitalWrite(OE, HIGH);
    digitalWrite(WR, HIGH);   // Now that we're in programming mode we can disable !WR
    delay(1);

    // Now in programming mode until RST is set HIGH again to remove 12V

    if (mode == HVSP) {
      HVSP_read(HVSP_READ_LFUSE_DATA, HVSP_READ_LFUSE_INSTR1);
      HVSP_read(0x00, HVSP_READ_LFUSE_INSTR2);
      read_lfuse = HVSP_read(0x00, HVSP_READ_LFUSE_INSTR3);

      HVSP_read(HVSP_READ_HFUSE_DATA, HVSP_READ_HFUSE_INSTR1);
      HVSP_read(0x00, HVSP_READ_HFUSE_INSTR2);
      read_hfuse = HVSP_read(0x00, HVSP_READ_HFUSE_INSTR3);

#if (BURN_EFUSE == 1)
      HVSP_read(HVSP_READ_EFUSE_DATA, HVSP_READ_EFUSE_INSTR1);
      HVSP_read(0x00, HVSP_READ_EFUSE_INSTR2);
      read_efuse = HVSP_read(0x00, HVSP_READ_EFUSE_INSTR3);
#endif

    } else {

      // Get current fuse settings stored on target device
      read_lfuse = fuse_read(LFUSE_SEL);
      read_hfuse = fuse_read(HFUSE_SEL);
#if (BURN_EFUSE == 1)
      read_efuse = fuse_read(EFUSE_SEL);
#endif
    }

    // done, disable outputs

    // clear digital pins 0-7
    digitalWrite(DATA0, 0);
    digitalWrite(DATA1, 0);
    digitalWrite(DATA2, 0);
    digitalWrite(DATA3, 0);
    digitalWrite(DATA4, 0);
    digitalWrite(DATA5, 0);
    digitalWrite(DATA6, 0);
    digitalWrite(DATA7, 0);

    // set digital pins 0-7 as inputs for now
    pinMode(DATA0, INPUT);
    pinMode(DATA1, INPUT);
    pinMode(DATA2, INPUT);
    pinMode(DATA3, INPUT);
    pinMode(DATA4, INPUT);
    pinMode(DATA5, INPUT);
    pinMode(DATA6, INPUT);
    pinMode(DATA7, INPUT);

    digitalWrite(RST, HIGH);  // exit programming mode
    delay(1);
    digitalWrite(OE, LOW);
    digitalWrite(WR, LOW);
    digitalWrite(PAGEL, LOW);
    digitalWrite(XA1, LOW);
    digitalWrite(XA0, LOW);
    digitalWrite(BS1, LOW);
    digitalWrite(BS2, LOW);
    digitalWrite(VCC, LOW);

    // Open serial port again to print fuse values
    Serial.begin(BAUD);
    delay(50);
    Serial.print("#Fuse");
    printHex(read_hfuse);
    Serial.print("&");
    printHex(read_lfuse);
    Serial.print("&");
#if (BURN_EFUSE == 1)
    printHex(read_efuse);
#endif
    Serial.println();
    doNext = 1;
  } // end of doNext3 read sequence

  if (doNext == 4) {  // 4=write fuses to target AVR

    // This business with TXC0 is required because Arduino doesn't give us a means to tell if a serial
    // transmission is complete before we move on and do other things.  If we don't wait for TXC0 to be reset,
    // I found that sometimes the 1st fuse burn would fail.  It turns out that DATA1 (which doubles as Arduino serial
    // TX) was still toggling by the time the 1st XTAL strobe latches the fuse program command.  Bad news.
#ifndef __AVR_ATmega32U4__
    UCSR0A |= _BV(TXC0);  // Reset serial transmit complete flag (need to do this manually because TX interrupts aren't used by Arduino)
#endif
#ifndef __AVR_ATmega32U4__
    while (!(UCSR0A & _BV(TXC0))); // Wait for serial transmission to complete before burning fuses!
#endif
    Serial.end();    // We're done with serial comms (for now) so disable UART
    delay(50);
    // Initialize pins to enter programming mode

    // clear digital pins 0-7
    digitalWrite(DATA0, 0);
    digitalWrite(DATA1, 0);
    digitalWrite(DATA2, 0);
    digitalWrite(DATA3, 0);
    digitalWrite(DATA4, 0);
    digitalWrite(DATA5, 0);
    digitalWrite(DATA6, 0);
    digitalWrite(DATA7, 0);

    // set digital pins 0-7 as inputs for now
    pinMode(DATA0, INPUT);
    pinMode(DATA1, INPUT);
    pinMode(DATA2, INPUT);
    pinMode(DATA3, INPUT);
    pinMode(DATA4, INPUT);
    pinMode(DATA5, INPUT);
    pinMode(DATA6, INPUT);
    pinMode(DATA7, INPUT);

    digitalWrite(PAGEL, LOW);
    digitalWrite(XA1, LOW);
    digitalWrite(XA0, LOW);
    digitalWrite(BS1, LOW);
    digitalWrite(BS2, LOW);
    digitalWrite(WR, LOW);  // ATtiny2313 needs this to be low to enter programming mode, ATmega doesn't care
    digitalWrite(OE, LOW);

    if (mode == HVSP) {
      digitalWrite(SDI, LOW);  // set necessary pin values to enter programming mode
      digitalWrite(SII, LOW);
      pinMode(SDO, OUTPUT);    // SDO is same as RDY pin
      digitalWrite(SDO, LOW);  // needs to be low to enter programming mode
    }

    // Enter programming mode
    digitalWrite(VCC, HIGH);  // Apply VCC to start programming process
    delayMicroseconds(80);
    digitalWrite(RST, LOW);   // Apply 12V to !RESET thru level shifter

    if (mode == HVSP) {
      // reset SDO after short delay, longer leads to logic contention because target sets SDO high after entering programming mode
      delayMicroseconds(1);   // datasheet says 10us, 1us is needed to avoid drive contention on SDO
      pinMode(SDO, INPUT);    // set to input to avoid logic contention
    }

    delayMicroseconds(10);  // Give lots of time for part to enter programming mode
    digitalWrite(OE, HIGH);
    digitalWrite(WR, HIGH);   // Now that we're in programming mode we can disable !WR
    delay(1);

    // Now in programming mode until RST is set HIGH again to remove 12V

    // Now burn desired fuses
    // How we do this depends on which mode we're in
    if (mode == HVSP) {
      HVSP_write(HVSP_WRITE_LFUSE_DATA, HVSP_WRITE_LFUSE_INSTR1);
      HVSP_write(lfuse, HVSP_WRITE_LFUSE_INSTR2);
      HVSP_write(0x00, HVSP_WRITE_LFUSE_INSTR3);
      HVSP_write(0x00, HVSP_WRITE_LFUSE_INSTR4);
      while (digitalRead(SDO) == LOW); // wait until burn is done

      HVSP_write(HVSP_WRITE_HFUSE_DATA, HVSP_WRITE_HFUSE_INSTR1);
      HVSP_write(hfuse, HVSP_WRITE_HFUSE_INSTR2);
      HVSP_write(0x00, HVSP_WRITE_HFUSE_INSTR3);
      HVSP_write(0x00, HVSP_WRITE_HFUSE_INSTR4);
      while (digitalRead(SDO) == LOW);

#if (BURN_EFUSE == 1)
      HVSP_write(HVSP_WRITE_EFUSE_DATA, HVSP_WRITE_EFUSE_INSTR1);
      HVSP_write(efuse, HVSP_WRITE_EFUSE_INSTR2);
      HVSP_write(0x00, HVSP_WRITE_EFUSE_INSTR3);
      HVSP_write(0x00, HVSP_WRITE_EFUSE_INSTR4);
      while (digitalRead(SDO) == LOW);
#endif
    } else {
      //delay(10);

      // First, program HFUSE
      fuse_burn(hfuse, HFUSE_SEL);

      // Now, program LFUSE
      fuse_burn(lfuse, LFUSE_SEL);

#if (BURN_EFUSE == 1)
      // Lastly, program EFUSE
      fuse_burn(efuse, EFUSE_SEL);
#endif
    }
    // clear digital pins 0-7
    digitalWrite(DATA0, 0);
    digitalWrite(DATA1, 0);
    digitalWrite(DATA2, 0);
    digitalWrite(DATA3, 0);
    digitalWrite(DATA4, 0);
    digitalWrite(DATA5, 0);
    digitalWrite(DATA6, 0);
    digitalWrite(DATA7, 0);

    // set digital pins 0-7 as inputs for now
    pinMode(DATA0, INPUT);
    pinMode(DATA1, INPUT);
    pinMode(DATA2, INPUT);
    pinMode(DATA3, INPUT);
    pinMode(DATA4, INPUT);
    pinMode(DATA5, INPUT);
    pinMode(DATA6, INPUT);
    pinMode(DATA7, INPUT);

    digitalWrite(RST, HIGH);  // exit programming mode
    delay(1);
    digitalWrite(OE, LOW);
    digitalWrite(WR, LOW);
    digitalWrite(PAGEL, LOW);
    digitalWrite(XA1, LOW);
    digitalWrite(XA0, LOW);
    digitalWrite(BS1, LOW);
    digitalWrite(BS2, LOW);
    digitalWrite(VCC, LOW);
    /*************************Read Fuses***************************/
    // Initialize pins to enter programming mode
    if (mode == HVSP) {
      digitalWrite(SDI, LOW);  // set necessary pin values to enter programming mode
      digitalWrite(SII, LOW);
      pinMode(SDO, OUTPUT);    // SDO is same as RDY pin
      digitalWrite(SDO, LOW);  // needs to be low to enter programming mode
    }

    // Enter programming mode
    digitalWrite(VCC, HIGH);  // Apply VCC to start programming process
    delayMicroseconds(80);
    digitalWrite(RST, LOW);   // Apply 12V to !RESET thru transistor

    if (mode == HVSP) {
      // reset SDO after short delay, longer leads to logic contention because target sets SDO high after entering programming mode
      delayMicroseconds(1);   // datasheet says 10us, 1us is needed to avoid drive contention on SDO
      pinMode(SDO, INPUT);    // set to input to avoid logic contention
    }

    delayMicroseconds(10);  // Give lots of time for part to enter programming mode
    digitalWrite(OE, HIGH);
    digitalWrite(WR, HIGH);   // Now that we're in programming mode we can disable !WR
    delay(1);

    // Now in programming mode until RST is set HIGH again to remove 12V

    if (mode == HVSP) {
      HVSP_read(HVSP_READ_LFUSE_DATA, HVSP_READ_LFUSE_INSTR1);
      HVSP_read(0x00, HVSP_READ_LFUSE_INSTR2);
      read_lfuse = HVSP_read(0x00, HVSP_READ_LFUSE_INSTR3);

      HVSP_read(HVSP_READ_HFUSE_DATA, HVSP_READ_HFUSE_INSTR1);
      HVSP_read(0x00, HVSP_READ_HFUSE_INSTR2);
      read_hfuse = HVSP_read(0x00, HVSP_READ_HFUSE_INSTR3);

#if (BURN_EFUSE == 1)
      HVSP_read(HVSP_READ_EFUSE_DATA, HVSP_READ_EFUSE_INSTR1);
      HVSP_read(0x00, HVSP_READ_EFUSE_INSTR2);
      read_efuse = HVSP_read(0x00, HVSP_READ_EFUSE_INSTR3);
#endif

    } else {

      // Get current fuse settings stored on target device
      read_lfuse = fuse_read(LFUSE_SEL);
      read_hfuse = fuse_read(HFUSE_SEL);
#if (BURN_EFUSE == 1)
      read_efuse = fuse_read(EFUSE_SEL);
#endif
    }

    // done, disable outputs

    // clear digital pins 0-7
    digitalWrite(DATA0, 0);
    digitalWrite(DATA1, 0);
    digitalWrite(DATA2, 0);
    digitalWrite(DATA3, 0);
    digitalWrite(DATA4, 0);
    digitalWrite(DATA5, 0);
    digitalWrite(DATA6, 0);
    digitalWrite(DATA7, 0);

    // set digital pins 0-7 as inputs for now
    pinMode(DATA0, INPUT);
    pinMode(DATA1, INPUT);
    pinMode(DATA2, INPUT);
    pinMode(DATA3, INPUT);
    pinMode(DATA4, INPUT);
    pinMode(DATA5, INPUT);
    pinMode(DATA6, INPUT);
    pinMode(DATA7, INPUT);

    digitalWrite(RST, HIGH);  // exit programming mode
    delay(1);
    digitalWrite(OE, LOW);
    digitalWrite(WR, LOW);
    digitalWrite(PAGEL, LOW);
    digitalWrite(XA1, LOW);
    digitalWrite(XA0, LOW);
    digitalWrite(BS1, LOW);
    digitalWrite(BS2, LOW);
    digitalWrite(VCC, LOW);

    // Open serial port again to print fuse values
    Serial.begin(BAUD);
    delay(50);
    Serial.print("#Burn");
    printHex(read_hfuse);
    Serial.print("&");
    printHex(read_lfuse);
    Serial.print("&");
#if (BURN_EFUSE == 1)
    printHex(read_efuse);
#endif
    Serial.println();
    doNext = 2;
  }

  // handle a string which came in via Serial and decide what to do next based on the input
  if (stringComplete) {
    evalString();
    if (inputString == "0") {
      Serial.println("Con0");
      hfuse = ATMega8H;
      lfuse = ATMega8L;
      efuse = ATMega8E;
      PAGEL = PAGELM;
      BS2 = BS2M;
      mode = ATMEGA;
      #define BURN_EFUSE 0
      doNext = 2;
      base = 16;
    }
    else if (inputString == "1") {
      Serial.println("Con1");
      hfuse = ATMega328PH;
      lfuse = ATMega328PL;
      efuse = ATMega328PE;
      PAGEL = PAGELM;
      BS2 = BS2M;
      mode = ATMEGA;
      doNext = 2;
      base = 16;
    }
    else if (inputString == "2") {
      Serial.println("Con2");
      hfuse = ATMega328H;
      lfuse = ATMega328L;
      efuse = ATMega328E;
      PAGEL = PAGELM;
      BS2 = BS2M;
      mode = ATMEGA;
      doNext = 2;
      base = 16;
    }
    else if (inputString == "3") {
      Serial.println("Con3");
      hfuse = ATtiny13H;
      lfuse = ATtiny13L;
      PAGEL = PAGELM;
      BS2 = BS2M;
      #define BURN_EFUSE 0
      mode = HVSP;
      doNext = 2;
      base = 16;
    }
    else if (inputString == "4") {
      Serial.println("Con4");
      hfuse = ATtiny85H;
      lfuse = ATtiny85L;
      efuse = ATtiny85E;
      PAGEL = PAGELM;
      BS2 = BS2M;
      mode = HVSP;
      doNext = 2;
      base = 16;
    }
    else if (inputString == "Q") {
      Serial.print(hfuse,HEX);
      Serial.print(lfuse,HEX);
      Serial.println(efuse,HEX);
    }
    else if (inputString.startsWith("H", 0)) {
      Serial.print("High");
      inputString = inputString.substring(1);
      evalString();
      if (base > 0) {
        hfuse = val1;
        doNext = 2;
      }
    }
    else if (inputString.startsWith("L", 0)) {
      Serial.print("Low");
      inputString = inputString.substring(1);
      evalString();
      if (base > 0) {
        lfuse = val1;
        doNext = 2;
      }
    }
    else if (inputString .startsWith("X", 0)) {
      Serial.print("Ext");
      inputString = inputString.substring(1);
      evalString();
      if (base > 0) {
        efuse = val1;
        doNext = 2;
      }
    }
    if (base > 0 && base < 20) {
      Serial.print("0X");
      printHex(val1);
      Serial.println();
    }
    else if (base == 99);  // print nothing
    else if (inputString == "W") {
      Serial.println("");
      doNext = 4;
    }
    else if (inputString == "R") {
      doNext = 3;
    }
    else if (inputString == "P") {
      Serial.println("Port_Ready");
      doNext = 2;
    }
    else {
      Serial.println("#Error");
      doNext = 2;
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
    base = 0;
  }

  // get new serial input and store in a String
  getSerialCharacters();

} //end of loop

void getSerialCharacters() {
  if (Serial.available()) {
    inputString = Serial.readStringUntil('\n');
    if (inputString != "")
      stringComplete = true;
  }
} // end of getSerialCharacters

void evalString() {  // interpret the numbers and convert from hex, decimal, or binary as needed
  inputString.toUpperCase();
  if (inputString.startsWith("0X", 0)  &&
      ((inputString.substring(2, 3) >= "0" && inputString.substring(2, 3) <= "9" ) ||
       (inputString.substring(2, 3) >= "A" && inputString.substring(2, 3) <= "F" ))) {
    //    Serial.println("hex number");
    base = 16;
  }
  else if ((inputString.substring(0, 1) >= "0" && inputString.substring(0, 1) <= "9" ) ||
           (inputString.substring(0, 1) >= "A" && inputString.substring(0, 1) <= "F" )) {
    //    Serial.println("hex number");
    base = 16;
  }

  int stringlength = inputString.length() + 1;
  char char_array[stringlength];
  inputString.toCharArray(char_array, stringlength);
  if (base != 0) {
    char * pEnd;
    val1 = strtol(char_array, &pEnd, base);
    if (val1 > 255) {
      base = 0;
    }
  }
} // end of evalString

void printBits(byte myByte) {
  for (byte mask = 0x80; mask; mask >>= 1) {
    if (mask  & myByte)
      Serial.print('1');
    else
      Serial.print('0');
  }
}

void printHex(byte myByte) {
  Serial.print(myByte / 16, HEX);
  Serial.print(myByte % 16, HEX);
}

void send_cmd(byte command)  // Send command to target AVR
{
  // Set controls for command mode
  digitalWrite(XA1, HIGH);
  digitalWrite(XA0, LOW);
  digitalWrite(BS1, LOW);

  if (mode != TINY2313)
    digitalWrite(BS2, LOW);  // Command load seems not to work if BS2 is high

  // set data pins to the value of command;
  digitalWrite(DATA0, bitRead(command, 0));
  digitalWrite(DATA1, bitRead(command, 1));
  digitalWrite(DATA2, bitRead(command, 2));
  digitalWrite(DATA3, bitRead(command, 3));
  digitalWrite(DATA4, bitRead(command, 4));
  digitalWrite(DATA5, bitRead(command, 5));
  digitalWrite(DATA6, bitRead(command, 6));
  digitalWrite(DATA7, bitRead(command, 7));

  // Set all DATA lines to outputs
  pinMode(DATA0, OUTPUT);
  pinMode(DATA1, OUTPUT);
  pinMode(DATA2, OUTPUT);
  pinMode(DATA3, OUTPUT);
  pinMode(DATA4, OUTPUT);
  pinMode(DATA5, OUTPUT);
  pinMode(DATA6, OUTPUT);
  pinMode(DATA7, OUTPUT);

  strobe_xtal();  // latch DATA

  digitalWrite(DATA0, 0);
  digitalWrite(DATA1, 0);
  digitalWrite(DATA2, 0);
  digitalWrite(DATA3, 0);
  digitalWrite(DATA4, 0);
  digitalWrite(DATA5, 0);
  digitalWrite(DATA6, 0);
  digitalWrite(DATA7, 0);

  // reset DATA to input to avoid bus contentions
  pinMode(DATA0, INPUT);
  pinMode(DATA1, INPUT);
  pinMode(DATA2, INPUT);
  pinMode(DATA3, INPUT);
  pinMode(DATA4, INPUT);
  pinMode(DATA5, INPUT);
  pinMode(DATA6, INPUT);
  pinMode(DATA7, INPUT);

}

void fuse_burn(byte fuse, int select)  // write high or low fuse to AVR
{

  send_cmd(B01000000);  // Send command to enable fuse programming mode

  // Enable data loading
  digitalWrite(XA1, LOW);
  digitalWrite(XA0, HIGH);
  // Specify low byte
  digitalWrite(BS1, LOW);
  if (mode != TINY2313)
    digitalWrite(BS2, LOW);
  delay(1);

  // Load fuse value into target

  // set data pins to value of fuse;
  digitalWrite(DATA0, bitRead(fuse, 0));
  digitalWrite(DATA1, bitRead(fuse, 1));
  digitalWrite(DATA2, bitRead(fuse, 2));
  digitalWrite(DATA3, bitRead(fuse, 3));
  digitalWrite(DATA4, bitRead(fuse, 4));
  digitalWrite(DATA5, bitRead(fuse, 5));
  digitalWrite(DATA6, bitRead(fuse, 6));
  digitalWrite(DATA7, bitRead(fuse, 7));

  // Set all DATA lines to outputs
  pinMode(DATA0, OUTPUT);
  pinMode(DATA1, OUTPUT);
  pinMode(DATA2, OUTPUT);
  pinMode(DATA3, OUTPUT);
  pinMode(DATA4, OUTPUT);
  pinMode(DATA5, OUTPUT);
  pinMode(DATA6, OUTPUT);
  pinMode(DATA7, OUTPUT);

  strobe_xtal();  // latch DATA

  digitalWrite(DATA0, 0);
  digitalWrite(DATA1, 0);
  digitalWrite(DATA2, 0);
  digitalWrite(DATA3, 0);
  digitalWrite(DATA4, 0);
  digitalWrite(DATA5, 0);
  digitalWrite(DATA6, 0);
  digitalWrite(DATA7, 0);

  // reset DATA to input to avoid bus contentions
  pinMode(DATA0, INPUT);
  pinMode(DATA1, INPUT);
  pinMode(DATA2, INPUT);
  pinMode(DATA3, INPUT);
  pinMode(DATA4, INPUT);
  pinMode(DATA5, INPUT);
  pinMode(DATA6, INPUT);
  pinMode(DATA7, INPUT);


  // Decide which fuse location to burn
  switch (select) {
    case HFUSE_SEL:
      digitalWrite(BS1, HIGH); // program HFUSE
      digitalWrite(BS2, LOW);
      break;
    case LFUSE_SEL:
      digitalWrite(BS1, LOW);  // program LFUSE
      digitalWrite(BS2, LOW);
      break;
    case EFUSE_SEL:
      digitalWrite(BS1, LOW);  // program EFUSE
      digitalWrite(BS2, HIGH);
      break;
  }
  delay(1);
  // Burn the fuse
  digitalWrite(WR, LOW);
  delay(1);
  digitalWrite(WR, HIGH);
  //delay(100);

  while (digitalRead(RDY) == LOW); // when RDY goes high, burn is done

  // Reset control lines to original state
  digitalWrite(BS1, LOW);
  digitalWrite(BS2, LOW);
}

byte fuse_read(int select) {
  byte fuse;

  send_cmd(B00000100);  // Send command to read fuse bits

  // Configure DATA as input so we can read back fuse values from target

  digitalWrite(DATA0, 0);
  digitalWrite(DATA1, 0);
  digitalWrite(DATA2, 0);
  digitalWrite(DATA3, 0);
  digitalWrite(DATA4, 0);
  digitalWrite(DATA5, 0);
  digitalWrite(DATA6, 0);
  digitalWrite(DATA7, 0);

  pinMode(DATA0, INPUT);
  pinMode(DATA1, INPUT);
  pinMode(DATA2, INPUT);
  pinMode(DATA3, INPUT);
  pinMode(DATA4, INPUT);
  pinMode(DATA5, INPUT);
  pinMode(DATA6, INPUT);
  pinMode(DATA7, INPUT);

  // Set control lines
  switch (select) {
    case LFUSE_SEL:
      // Read LFUSE
      digitalWrite(BS2, LOW);
      digitalWrite(BS1, LOW);
      break;
    case HFUSE_SEL:
      // Read HFUSE
      digitalWrite(BS2, HIGH);
      digitalWrite(BS1, HIGH);
      break;
    case EFUSE_SEL:
      // Read EFUSE
      digitalWrite(BS2, HIGH);
      digitalWrite(BS1, LOW);
      break;
  }

  //  Read fuse
  digitalWrite(OE, LOW);
  delay(1);

  // read data pins and set value in variable fuse
  fuse = digitalRead(DATA7) << 7 |
         digitalRead(DATA6) << 6 |
         digitalRead(DATA5) << 5 |
         digitalRead(DATA4) << 4 |
         digitalRead(DATA3) << 3 |
         digitalRead(DATA2) << 2 |
         digitalRead(DATA1) << 1 |
         digitalRead(DATA0);

  digitalWrite(OE, HIGH);  // Done reading, disable output enable line
  return fuse;
}

byte HVSP_read(byte data, byte instr) { // Read a byte using the HVSP protocol

  byte response = 0x00; // a place to hold the response from target

  digitalWrite(SCI, LOW);  // set clock low
  // 1st bit is always zero
  digitalWrite(SDI, LOW);
  digitalWrite(SII, LOW);
  sclk();

  // We capture a response on every readm even though only certain responses contain
  // valid data.  For fuses, the valid response is captured on the 3rd instruction write.
  // It is up to the program calling this function to figure out which response is valid.

  // The MSB of the response byte comes "early", that is,
  // before the 1st non-zero-padded byte of the 3rd instruction is sent to the target.
  // For more information, see the ATtiny25/45/85 datasheet, Table 20-16 (pg. 164).
  if (digitalRead(SDO) == HIGH) // target sent back a '1'?
    response |= 0x80;  // set MSB of response byte high

  // Send each bit of data and instruction byte serially, MSB first
  // I do this by shifting the byte left 1 bit at a time and checking the value of the new MSB
  for (int i = 0; i < 8; i++) { // i is bit number
    if ((data << i) & 0x80)  // shift data byte left and check if new MSB is 1 or 0
      digitalWrite(SDI, HIGH);  // bit was 1, set pin high
    else
      digitalWrite(SDI, LOW);   // bit was 0, set pin low

    if ((instr << i) & 0x80)   // same process for instruction byte
      digitalWrite(SII, HIGH);
    else
      digitalWrite(SII, LOW);
    sclk();

    if (i < 7) {  // remaining 7 bits of response are read here (one at a time)
      // note that i is one less than the bit position of response we are reading, since we read
      // the MSB above.  That's why I shift 0x40 right and not 0x80.
      if (digitalRead(SDO) == HIGH) // if we get a logic 1 from target,
        response |= (0x40 >> i);    // set corresponding bit of response to 1
    }
  }

  // Last 2 bits are always zero
  for (int i = 0; i < 2; i++) {
    digitalWrite(SDI, LOW);
    digitalWrite(SII, LOW);
    sclk();
  }

  return response;
}

void HVSP_write(byte data, byte instr) { // Write to target using the HVSP protocol

  digitalWrite(SCI, LOW);  // set clock low

  // 1st bit is always zero
  digitalWrite(SDI, LOW);
  digitalWrite(SII, LOW);
  sclk();  // latch bit

  // Send each bit of data and instruction byte serially, MSB first
  // I do this by shifting the byte left 1 bit at a time and checking the value of the new MSB
  for (int i = 0; i < 8; i++) { // i is bit number
    if ((data << i) & 0x80)  // shift data byte left and check if new MSB is 1 or 0
      digitalWrite(SDI, HIGH);  // bit was 1, set pin high
    else
      digitalWrite(SDI, LOW);   // bit was 0, set pin low

    if ((instr << i) & 0x80)  // same process for instruction byte
      digitalWrite(SII, HIGH);
    else
      digitalWrite(SII, LOW);

    sclk();  // strobe SCI (serial clock) to latch data
  }

  // Last 2 bits are always zero
  for (int i = 0; i < 2; i++) {
    digitalWrite(SDI, LOW);
    digitalWrite(SII, LOW);
    sclk();
  }
}

void sclk(void) {  // send serial clock pulse, used by HVSP commands

  // These delays are much  longer than the minimum requirements,
  // but we don't really care about speed.
  delay(1);
  digitalWrite(SCI, HIGH);
  delay(1);
  digitalWrite(SCI, LOW);
}

void strobe_xtal(void) {  // strobe xtal (usually to latch data on the bus)

  delay(1);
  digitalWrite(XTAL1, HIGH);  // pulse XTAL to send command to target
  delay(1);
  digitalWrite(XTAL1, LOW);
}
