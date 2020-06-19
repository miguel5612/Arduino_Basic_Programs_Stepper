/*
 * Connect STEP, DIR as indicated
 *
 * Copyright (C)2020 Miguel Califa
 *
 * This file may be redistributed under the terms of the MIT license.
 * A copy of this license has been included with this distribution in the file LICENSE.
 */
#include <Arduino.h>
#include "BasicStepperDriver.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// All the wires needed for full functionality
#define DIR 6 // Y - Arduino uno cnc Shield
#define STEP 3
#define EN 8
#define pot A0

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 1600

// Since microstepping is set externally, make sure this matches the selected mode
// If it doesn't, the motor will move at a different RPM than chosen
// 1=full step, 2=half step etc.
#define MICROSTEPS 1
#define minRPM 10
#define maxRPM 114


float m = (maxRPM - minRPM)/(100.0-0.0);
int b = minRPM;

// 2-wire basic config, microstepping is hardwired on the driver
BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//Uncomment line to use enable/disable functionality
//BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP, SLEEP);
int val, RPM=300;
float perc;
const byte interruptPin = 2;


void setup() {
    Serial.begin(9600);
    pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), getVelocity, CHANGE);
    
    lcd.begin();                      // initialize the lcd 
    lcd.begin();
    lcd.backlight();

    //stepper.begin(RPM, MICROSTEPS);
    // if using enable/disable on ENABLE pin (active LOW) instead of SLEEP uncomment next line
    // stepper.setEnableActiveState(LOW);
    pinMode(pot, INPUT);
    pinMode(EN, OUTPUT);
    
    digitalWrite(EN, LOW);

    lcd.setCursor(3,0);
    lcd.print("Creado por:");
    lcd.setCursor(2,1);
    lcd.print(" Miguel Califa ");
    delay(1000);
    lcd.setCursor(2,1);
    lcd.print("   CNC Ciensa   ");
    delay(1000);
    lcd.clear();
}

void loop() {
    lcd.setCursor(0,0);
    lcd.print("Pot:"); lcd.print(perc); lcd.print(" %"); lcd.print("     ");
    lcd.setCursor(0,1);
    lcd.print("RPM:"); lcd.print(RPM); lcd.print("     ");
    
    getVelocity();
    stepper.move(1e5000);
}

void getVelocity()
{
    Serial.print("Potenciometro: "); Serial.print(perc); Serial.println(" %");
    Serial.print("RPM: "); Serial.println(RPM);
  
    val = analogRead(pot);
    perc = (val* 100.0)/1024.0;
    //RPM = (perc * maxRPM) / 100.0;
    RPM = perc*m + b;
    //Serial.print("Potenciometro: "); Serial.print(perc); Serial.println(" %");
    //Serial.print("RPM: "); Serial.println(RPM);
    stepper.setRPM(RPM);
    stepper.stop();
}
