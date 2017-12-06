/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <IRremote.h>
#include <SPI.h>

int RECV_PIN = 9;
const int slaveSelectPin = 10;
const int relayPin = 5;
const int fanPin = 6;
int fanSpeed = 180;
int mute = 64;
int soft = 60;
int volume = soft;
int oldVolume = soft;
int power;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  //IR
  Serial.begin(9600);
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
  
  //DigiPot
  // set the slaveSelectPin as an output:
  pinMode(slaveSelectPin, OUTPUT);
  // initialize SPI:
  SPI.beginTransaction(SPISettings(10000000, LSBFIRST, SPI_MODE0));
  SPI.begin();
  delay(10);
  setVolume(volume);

  //Relay
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);

  //Fan
  pinMode(fanPin, OUTPUT);
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    if(results.value == 0xE0E0F00F || results.value == 0x50F7E69C || results.value == 0x2340B922 || results.value == 0x3D53B4D || results.value == 0xF7841AC1 || results.value == 0xBC9AFFF5 || results.value == 0x39B43384 || results.value == 0xF9E7211F){
      if (volume == 64) {
         Serial.print("MUTE! Setting volume to ");
         Serial.print(oldVolume,1);
         Serial.println();
          setVolume(oldVolume);
          volume = oldVolume;
          delay(500);
         }
        else {
          Serial.println("MUTE! Setting volume to mute");
          oldVolume = volume;
          setVolume(mute);
          volume = mute;
          delay(500);
          }
      if (power == 0) { 
       Serial.println("Power on!");
       digitalWrite(relayPin, LOW);
       analogWrite(fanPin, fanSpeed);
       power = 1;
      }
     }
     
    if(results.value == 0xE0E0E01F || results.value == 0x68733A46 || results.value == 0x1A9F03F3 || results.value == 0xBF13ED7B || results.value == 0x909FC81B || results.value == 0x3825071F || results.value == 0x440AABF3){
      if (volume == 64) { //Muted, so let's go back to oldVolume
        if (oldVolume > 3) { volume = oldVolume-3; }
        else { volume = 0; }
        setVolume(volume);
       }
      else { 
        if (volume > 3) { volume = volume-3; }
        else { volume = 0; }
        Serial.print("VOLUME UP! setting volume to ");
        Serial.print(volume,1);
        Serial.println();
        setVolume(volume);
      }
      if (power == 0) { 
       Serial.println("Power on!");
       digitalWrite(relayPin, LOW);
       analogWrite(fanPin, fanSpeed);
       power = 1;
      }      
    }
    
    if(results.value == 0xE0E0D02F || results.value == 0x83B19366 || results.value == 0xDBDDAA4B || results.value == 0x2CC0C73 || results.value == 0xBF9C93BD || results.value == 0x57F4F505 || results.value == 0x781C6D8){
      if (volume == 64) { //Muted, so let's go back to oldVolume
       if (oldVolume < 60) { volume = oldVolume+3; }
       else { volume = 63; }
       setVolume(volume);
      }
      else { 
       if (volume < 60) { volume = volume+3; }
       else { volume = 63; }
       Serial.print("VOLUME DOWN! setting volume to ");
       Serial.print(volume,1);
       Serial.println();
       setVolume(volume);
      }
    }      
     if(results.value == 0xF4BA2988 || results.value == 0xB17671D3 || results.value == 0xABDE17DD || results.value == 0xE0E040BF || results.value == 0x5C928A6D || results.value == 0xC1959EE1){
      if (power == 0) { 
       Serial.println("Power button! Power on");
       digitalWrite(relayPin, LOW);
       analogWrite(fanPin, fanSpeed);
       power = 1;
       delay(500);
      }
      else {
       Serial.println("Power button! Power off");
       digitalWrite(relayPin, HIGH);
       analogWrite(fanPin, 0);
       power = 0;
       delay(1000);
      }
     }
    irrecv.resume(); // Receive the next value
  }
  delay(100);
}

void setVolume(int level) {
  // take the SS pin high to select the chip:
  digitalWrite(slaveSelectPin, HIGH);
  //  send in the address and value via SPI:
  SPI.transfer(level);
  SPI.transfer(level);
  // take the SS pin high to de-select the chip:
  digitalWrite(slaveSelectPin, LOW);
}
