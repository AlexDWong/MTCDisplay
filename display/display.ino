/*
    Adapted from https://gist.github.com/nrdobie/8193350
*/

#include <SPI.h>
#include <stdio.h>

#define LOAD_PIN 7    //Load/CS pin

byte h, m, s, f;      //hours, minutes, seconds, frame
byte buf_input[64];   //input buffer
byte buf_temp[8];      //timecode buffer

byte command, data, index;

byte update_rate = 8;
byte counter = 0;

void display_write(uint8_t address, uint8_t value) {
  digitalWrite(LOAD_PIN, LOW); //Set CS to low
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(LOAD_PIN, HIGH); //set CS to high
}

void display_timecode() {
  display_write(0x01, (f%10));
  display_write(0x02, (f/10));
  display_write(0x03, (s%10));
  display_write(0x04, (s/10));
  display_write(0x05, (m%10));
  display_write(0x06, (m/10));
  display_write(0x07, (h%10));
  display_write(0x08, (h/10));
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
  display_write(0x0A, 0x0F);  //max available intensity
  display_write(0x0B, 0x07);  //display all digits
  display_write(0x0C, 0x01);  //turn on chip

  for (int i = 0x01; i <= 0x08; ++i) {
    display_write(i, 0x00);
  }  
}

void serialEvent() {
  if (Serial.available() > 1) {
    command = Serial.read();
    if (command != 0xF1) {
      command = Serial.read();
    }
    data = Serial.read();
    index = data >> 4;
    data &= 0x0F;   //clear packet ID
    buf_temp[index] = data;
    
  }

  //counter++;
  //if (counter >= update_rate) {
    h = (buf_temp[7] & 0x01)*16 + buf_temp[6];
    m = buf_temp[5]*16 + buf_temp[4];
    s = buf_temp[3]*16 + buf_temp[2];
    f = buf_temp[1]*16 + buf_temp[0];
    display_timecode();
    counter = 0;
    
  //}
  
}

void loop() {

  
}
  
