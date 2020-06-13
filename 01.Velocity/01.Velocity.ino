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
#define DIR 55
#define STEP 54
#define pot A3

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200

// Since microstepping is set externally, make sure this matches the selected mode
// If it doesn't, the motor will move at a different RPM than chosen
// 1=full step, 2=half step etc.
#define MICROSTEPS 2
#define minRPM 10
#define maxRPM 250

float m = (maxRPM - minRPM)/(100.0-0.0);
int b = minRPM;

// 2-wire basic config, microstepping is hardwired on the driver
BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//Uncomment line to use enable/disable functionality
//BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP, SLEEP);
int val, RPM=300;
float perc;

void setup() {
    lcd.init();                      // initialize the lcd 
    lcd.init();
    lcd.backlight();

    stepper.begin(RPM, MICROSTEPS);
    // if using enable/disable on ENABLE pin (active LOW) instead of SLEEP uncomment next line
    // stepper.setEnableActiveState(LOW);
    pinMode(pot, INPUT);
    Serial.begin(9600);

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
    
  
    // energize coils - the motor will hold position
    // stepper.enable();
  
    /*
     * Moving motor one full revolution using the degree notation
     */
    val = analogRead(pot);
    perc = (val* 100.0)/1024.0;
    //RPM = (perc * maxRPM) / 100.0;
    RPM = perc*m + b;
    
    lcd.setCursor(0,0);
    lcd.print("Pot:"); lcd.print(perc); lcd.print(" %");
    lcd.setCursor(0,1);
    lcd.print("RPM:"); lcd.print(RPM);
    
    Serial.print("Potenciometro: "); Serial.print(perc); Serial.println(" %");
    Serial.print("RPM: "); Serial.println(RPM);
    stepper.setRPM(RPM);
    stepper.rotate((RPM/120.0)*MOTOR_STEPS);
    /*
     * Moving motor to original position using steps
     */
    //stepper.move(-MOTOR_STEPS*MICROSTEPS);

    // pause and allow the motor to be moved by hand
    // stepper.disable();

    //delay(5000);
}
