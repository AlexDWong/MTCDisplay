/*
    Adapted from https://gist.github.com/nrdobie/8193350
*/

#include <SPI.h>

#define LOAD_PIN 7    //Load/CS pin

void display_write(uint8_t address, uint8_t value) {
  digitalWrite(LOAD_PIN, LOW); //Set CS to low
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(LOAD_PIN, HIGH); //set CS to high
}

void setup() {
  pinMode(LOAD_PIN, OUTPUT);  //Load/CS pin output
  SPI.setBitOrder(MSBFIRST);  //MSB first
  SPI.begin();

  //test by turning on, then off
  display_write(0x0F, 0x01);
  delay(1000);
  display_write(0x0F, 0x00);

  display_write(0x09, 0xFF);  //enable onboard bit decode (Mode B)
  display_write(0x0A, 0x0F);  //max available intensity
  display_write(0x0B, 0x07);  //display all digits
  display_write(0x0C, 0x01);  //turn on chip

  display_write(0x02, 0x00);
  display_write(0x03, 0x00);
  display_write(0x04, 0x00);
  display_write(0x05, 0x00);
  display_write(0x06, 0x00);
  display_write(0x07, 0x00);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  for (uint8_t i = 0; i < 0x10; ++i) {
    for (uint8_t j = 0x01; j <= 0x08; ++j) {
      display_write(j, i);
    }
    
    delay(1000);
  }
}
