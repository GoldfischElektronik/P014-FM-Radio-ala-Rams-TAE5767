
// Arduino Pro Mini
// TAE5767 FM Radio Module
// Audio Amplifier
// 16 ohm Speaker
// Li-ion Charger 
// 18650 li-ion battery
// 18650 battery holder
// USB-C connector
// 2x fancy buttons
// Antenna (a wire would do as well)


#include <Arduino.h>
#include <Wire.h>
#include <radio.h>
#include <TEA5767.h>
#include <EEPROM.h>

/// The band that will be tuned by this sketch is FM.
#define FIX_BAND RADIO_BAND_FM

/// The station that will be tuned by this sketch is 95.60 MHz.
long initStation = 9560;
long FIX_STATION = initStation;
int eePromFixStationPosition = 10;

TEA5767 radio; // Create an instance of Class

// uint8_t test1;
// byte test2;

// Button
const int buttonPin = 2; // the number of the pushbutton pin
int buttonState = 0;     // variable for reading the pushbutton stat

// void writeIntIntoEEPROM(int address, int number)
//{
//   byte byte1 = number >> 8;
//   byte byte2 = number & 0xFF;
//   EEPROM.write(address, byte1);
//   EEPROM.write(address + 1, byte2);
// }
//
// int readIntFromEEPROM(int address)
//{
//   byte byte1 = EEPROM.read(address);
//   byte byte2 = EEPROM.read(address + 1);
//   return (byte1 << 8) + byte2;
// }

void writeLongIntoEEPROM(int address, long number)
{
  EEPROM.write(address, (number >> 24) & 0xFF);
  EEPROM.write(address + 1, (number >> 16) & 0xFF);
  EEPROM.write(address + 2, (number >> 8) & 0xFF);
  EEPROM.write(address + 3, number & 0xFF);
}

long readLongFromEEPROM(int address)
{
  return ((long)EEPROM.read(address) << 24) +
         ((long)EEPROM.read(address + 1) << 16) +
         ((long)EEPROM.read(address + 2) << 8) +
         (long)EEPROM.read(address + 3);
}

void saveChanel(long frequency)
{
  Serial.print("saveChanel/frequency before: ");
  Serial.println(frequency);
  writeLongIntoEEPROM(eePromFixStationPosition, frequency);
  Serial.print("saveChanel/frequency after:  ");
  Serial.println(readLongFromEEPROM(eePromFixStationPosition));
}

void changeChanel()
{

  FIX_STATION += 10;
  if (FIX_STATION >= 10800)
  {
    FIX_STATION = 8700;
  }

  Serial.print("changeChanel/FIX_STATION: ");
  Serial.println(FIX_STATION);

  saveChanel(FIX_STATION);
  radio.setBandFrequency(FIX_BAND, FIX_STATION);

  //  Serial.println(radio.getFrequency());
  //  radio.seekUp();
  //  radio.seekDown();
}

void seekChanel()
{

  radio.seekUp();
  FIX_STATION = radio.getFrequency();

  Serial.print("changeChanel/FIX_STATION: ");
  Serial.println(FIX_STATION);

  // saveChanel(FIX_STATION);
  // radio.setBandFrequency(FIX_BAND, FIX_STATION);

  //  Serial.println(radio.getFrequency());
  //  radio.seekUp();
  //  radio.seekDown();
}

void printFrequency()
{
  char s[12];
  radio.formatFrequency(s, sizeof(s));
  Serial.print("Station:");
  Serial.println(s);

  Serial.print("Radio:");
  radio.debugRadioInfo();

  Serial.print("Audio:");
  radio.debugAudioInfo();
}

/// Setup a FM only radio configuration
/// with some debugging on the Serial port
void setup()
{

  pinMode(buttonPin, INPUT_PULLUP);

  // open the Serial port
  Serial.begin(115200);
  Serial.println("Radio...");
  delay(200);

  // Initialize the Radio
  radio.init();

  // Enable information to the Serial port
  radio.debugEnable();

  long Station = readLongFromEEPROM(eePromFixStationPosition);
  Serial.print("init Read Station: ");
  Serial.println(Station);
  if (Station < 8700 || Station > 10800)
  {
    Station = initStation;
    writeLongIntoEEPROM(eePromFixStationPosition, Station);
  }

  FIX_STATION = Station;
  Serial.print("FIX_STATION: ");
  Serial.println(FIX_STATION);

  radio.setBandFrequency(FIX_BAND, FIX_STATION);
  Serial.print("Min Freqency: ");
  Serial.println(radio.getMinFrequency());
  Serial.print("Max Freqency: ");
  Serial.println(radio.getMaxFrequency());
  radio.setVolume(2);
  radio.setMono(false);
}

// setup

// show the current chip data every 3 seconds.
void loop()
{

  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == LOW)
  {
    // Serial.println("buttonState == LOW");
    changeChanel();
    // seekChanel(); 
    delay(300);
  }
}
