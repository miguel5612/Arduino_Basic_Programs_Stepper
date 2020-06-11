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
#define DIR 55
#define STEP 54
#define pot A3
#define button 13

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
char serialData;

// 2-wire basic config, microstepping is hardwired on the driver
BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);

//Uncomment line to use enable/disable functionality
//BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP, SLEEP);
int val, RPM=300;
float perc;
int steps = 0;
bool direction = false;

void setup() {
    stepper.begin(RPM, MICROSTEPS);
    // if using enable/disable on ENABLE pin (active LOW) instead of SLEEP uncomment next line
    // stepper.setEnableActiveState(LOW);
    pinMode(pot, INPUT);
    pinMode(button, INPUT);
    Serial.begin(9600);
    Serial.println("Escriba P para pasos, ejemplo P123 para configurar 123 pasos");
    steps = EEPROM.read(0);
}

void loop() {
     if (Serial.available()){
        serialData = Serial.read();
        if ( serialData == 'P'){
          steps = Serial.readString().toInt();
          EEPROM.write(0, steps);
          Serial.println("pasos: "+ String(steps) );
        }
    }
    if(digitalRead(button) & steps>0)
    {
       Serial.print("Girando.");
       int pasos = 0;
       while(pasos <= steps)
       {
          if(direction) stepper.move(1); //Paso 1, el motor se mueve
          else stepper.move(-1);
          getVelocity(); // Chequeo si la velocidad ha cambiado
          Serial.print(".");
          pasos++;
       }
       direction = !direction;
       Serial.println("Tarea completada con exito!");
    }
    getVelocity();
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
}
