#include<Arduino.h>
#include<Wire.h>
#include<SoftwareSerial.h>
#include<SabertoothSimplified.h>
#include <Cytron_PS2_Shield.h>
#include<avr/wdt.h>
#include<utility/twi.h>

bool switchedOn = false;
int readPs2Var = 0;

// Big main motor driver
SoftwareSerial SabretoothSerial(NOT_A_PIN, 9); // RX on no pin (unused), TX on pin 9 (to S1).

SabertoothSimplified ST1(SabretoothSerial); // Use SoftwareSerial as the serial port.

Cytron_PS2_Shield ps2;

void setup() {
  delay(500);
  
  wdt_enable(WDTO_2S);

  Wire.begin();
  
  ps2.AttachPS2Serial(&Serial2);

  SabretoothSerial.begin(9600); // Set the same as the baud pins on the sabretooth.

}

void loop()
{
  wdt_reset(); // this checks if the board crashed and resets

  doMyWheels();

  delay(100);
}

void doMyWheels() {
  
  if (ps2.readButton(PS2_LEFT_2) == 0) {
    
    int readPs2Var=-(ps2.readButton(PS2_JOYSTICK_LEFT_Y_AXIS)-128)/2;
    sendLWheelMotorValue(readPs2Var);

    readPs2Var=-(ps2.readButton(PS2_JOYSTICK_RIGHT_Y_AXIS)-128)/2;
    sendRWheelMotorValue(readPs2Var);
    
  } else {
    sendLWheelMotorValue(0);
    sendRWheelMotorValue(0);
  }
}

int rWheelMotorValue = 0;
void sendRWheelMotorValue(int newValue) {
  if (newValue != rWheelMotorValue)
  {
    rWheelMotorValue = newValue;
    ST1.motor(2, rWheelMotorValue);
  }
}

int lWheelMotorValue = 0;
void sendLWheelMotorValue(int newValue) {
  if (newValue != lWheelMotorValue)
  {
    lWheelMotorValue = newValue;
    ST1.motor(1, lWheelMotorValue);
  }
}



/*
bool startButtonPressed = false;
void checkForOnOffChange() {
  if (!startButtonPressed && (ps2.readButton(PS2_START) == 0)) {
    startButtonPressed = true;
    robotSwitchedOn = !robotSwitchedOn;
    if (!robotSwitchedOn) {
      switchOffArcadeButtons();
      switchOffDisplay();
      sendRArmMotorValue(0);
      sendLArmMotorValue(0);
    } else {
      showNumber();
    }
  } else if (ps2.readButton(PS2_START) == 1) {
    startButtonPressed = false;
  }
}*/
