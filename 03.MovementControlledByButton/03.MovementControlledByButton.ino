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
#include <EEPROM.h>

// All the wires needed for full functionality
#define DIR 10
#define STEP 9
#define pot A0
#define button A2
#define interruptPin 2
#define EN 8

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 1600

// Since microstepping is set externally, make sure this matches the selected mode
// If it doesn't, the motor will move at a different RPM than chosen
// 1=full step, 2=half step etc.
#define MICROSTEPS 2
#define minRPM 10
#define maxRPM 40

float m = (maxRPM - minRPM)/(100.0-0.0);
int b = minRPM;
char serialData;

// 2-wire basic config, microstepping is hardwired on the driver
BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);

//Uncomment line to use enable/disable functionality
//BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP, SLEEP);
int val, RPM=300;
float perc;

struct config_t
{
    int steps;
    bool dir;
} configuration;


void setup() {
    getVelocity();
    stepper.begin(RPM, MICROSTEPS);
    // if using enable/disable on ENABLE pin (active LOW) instead of SLEEP uncomment next line
    // stepper.setEnableActiveState(LOW);
    pinMode(pot, INPUT);
    pinMode(button, INPUT);
    pinMode(interruptPin, INPUT_PULLUP);
    pinMode(EN, OUTPUT);
    digitalWrite(EN, LOW);
    attachInterrupt(digitalPinToInterrupt(interruptPin), getVelocity, CHANGE);
    EEPROM_readAnything(0, configuration);
    Serial.begin(9600);
    Serial.print("Escriba P para pasos, ejemplo P"); Serial.print(configuration.steps); Serial.print(" para configurar "); Serial.print(configuration.steps);Serial.println(" pasos");
}

void loop() {
     if (Serial.available()){
        serialData = Serial.read();
        if ( serialData == 'P'){
          configuration.steps = Serial.readString().toInt();
          EEPROM_writeAnything(0, configuration);
          Serial.println("pasos: "+ String(configuration.steps) );
        }
    }
    if(digitalRead(button) & configuration.steps>0)
    {
       Serial.print("Girando.");
       int pasos = 0;
      if(configuration.dir > 0)
      {
        stepper.move(configuration.steps);
        configuration.dir = 0;
      }
      else 
      {
        stepper.move(-configuration.steps);
        configuration.dir = 1;
      }
       EEPROM_writeAnything(0, configuration);
       Serial.print("Tarea completada con exito, proxima direccion ->"); Serial.println(configuration.dir);
       Serial.print("Con velocidad: "); Serial.print(RPM); Serial.println(" RPM");
    }
}

void getVelocity()
{
  
    val = analogRead(pot);
    perc = (val* 100.0)/1024.0;
    //RPM = (perc * maxRPM) / 100.0;
    RPM = perc*m + b;
    //Serial.print("Potenciometro: "); Serial.print(perc); Serial.println(" %");
    //Serial.print("RPM: "); Serial.println(RPM);
    stepper.setRPM(RPM);
    stepper.stop();
}
template <class T> int EEPROM_writeAnything(int ee, const T& value)
{
    const byte* p = (const byte*)(const void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          EEPROM.write(ee++, *p++);
    return i;
}

template <class T> int EEPROM_readAnything(int ee, T& value)
{
    byte* p = (byte*)(void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          *p++ = EEPROM.read(ee++);
    return i;
}
