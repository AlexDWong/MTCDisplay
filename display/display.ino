/*
    Adapted from https://gist.github.com/nrdobie/8193350
*/

#include <SPI.h>
#include <stdio.h>

#define LOAD_PIN 7    //Load/CS pin

byte h, m, s, f;      //hours, minutes, seconds, frame
byte buf_temp[8];     //timecode buffer
byte command, data, index;

/*  Write VALUE to register ADDRESS on the MAX7219. */
void display_write(uint8_t address, uint8_t value) {
  digitalWrite(LOAD_PIN, LOW); //Toggle enable pin to load MAX7219 shift register
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(LOAD_PIN, HIGH); 
}


/*  Update Timecode display. Called once FRAMES LSB quarter-frame received to
      avoid writing to chip every single time.
*/
void display_timecode() {   
  display_write(0x02, ((f%10)|0x80)); //enable decimal point on LSB
  display_write(0x06, (f/10));        
  display_write(0x08, ((s%10)|0x80)); 
  display_write(0x04, (s/10));
  display_write(0x03, ((m%10)|0x80));
  display_write(0x07, (m/10));
  display_write(0x05, ((h%10)|0x80));
  display_write(0x01, (h/10));
}

void setup() {
  pinMode(LOAD_PIN, OUTPUT);  //Load/CS pin output
  SPI.setBitOrder(MSBFIRST);  //MSB first
  SPI.begin();
  
  Serial.begin(31250, SERIAL_8N1); //MIDI input 

  //test by turning on, then off
  display_write(0x0F, 0x01);
  delay(1000);
  display_write(0x0F, 0x00);

  display_write(0x09, 0xFF);  //enable onboard bit decode (Mode B)
  display_write(0x0A, 0x0F);  //max intensity
  display_write(0x0B, 0x07);  //display all digits
  display_write(0x0C, 0x01);  //turn on chip

  //display all zeros, and add decimal points to LSB
  for (int i = 0x01; i <= 0x08; ++i) {
    display_write(i, 0x00);
  }
  display_write(0x05, 0x80);  //
  display_write(0x03, 0x80);
  display_write(0x08, 0x80);
  display_write(0x02, 0x80);
  
}

/*  Instead of using a while() loop, reduce power usage and increase performance using a UART interrupt handler.
      When a quarter-frame TC packet is received, update the internal timecode buffer.
      When the FRAMES LSB quarter-frame is received, also update the visual display.
*/
void serialEvent() {
  if (Serial.available() > 1) {
    command = Serial.read();
    if (command != 0xF1) {    //realign quarter-frame packets
      command = Serial.read();
    }
    data = Serial.read();
    index = data >> 4;  //extract index/packet ID
    data &= 0x0F;       //clear packet ID from data
    buf_temp[index] = data;
  }

  if (index >= 0x07) {  //recalculate timecode once FRAMES LSB quarter-frame received
    h = (buf_temp[7] & 0x01)*16 + buf_temp[6];
    m = buf_temp[5]*16 + buf_temp[4];
    s = buf_temp[3]*16 + buf_temp[2];
    f = buf_temp[1]*16 + buf_temp[0];
    display_timecode();
  }
}

void loop() {
}
  
