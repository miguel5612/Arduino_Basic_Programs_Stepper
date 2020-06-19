#include <Arduino.h>
#include "BasicStepperDriver.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define DIR 6 // Y - Arduino uno cnc Shield
#define STEP 3
#define EN 8
#define pot A0

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 3200

BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const byte ledPin = 13;
const byte interruptPin = 2;
volatile byte state = LOW;

void setup() {
  lcd.begin();                      // initialize the lcd 
    lcd.begin();
    lcd.backlight();

  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), ala, CHANGE);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(ledPin, state);
  delay(900000);
}

void ala() {
  analogRead(A0);
  Serial.println("Lol");
  state = !state;
}
