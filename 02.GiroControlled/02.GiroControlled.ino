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
#include <EEPROM.h>


// All the wires needed for full functionality
#define DIR 6
#define STEP 3
#define pot A0
#define relay 12
#define interruptPin 2
#define EN 8

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 1600

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
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//Uncomment line to use enable/disable functionality
//BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP, SLEEP);
int val, RPM=300;
float perc;

struct config_t
{
    int steps, delayTime;
} configuration;

void setup() {
    lcd.begin();                      // initialize the lcd 
    lcd.begin();
    lcd.backlight();
    
    stepper.begin(RPM, MICROSTEPS);
    // if using enable/disable on ENABLE pin (active LOW) instead of SLEEP uncomment next line
    // stepper.setEnableActiveState(LOW);
    EEPROM_readAnything(0, configuration);
    pinMode(pot, INPUT);
    pinMode(relay, OUTPUT);
    pinMode(EN, OUTPUT);
    digitalWrite(EN, LOW);
    pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), getVelocity, CHANGE);
    digitalWrite(relay, LOW);
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
    
    Serial.println("Escriba P para pasos, T para tiempo, R para reiniciar");
    Serial.println(" ejemplo P150 para configurar 150 pasos y T1000 para configurar 1 segundo de tiempo");
}

void loop() {
     if (Serial.available()){
        serialData = Serial.read();
        if ( serialData == 'P'){
          configuration.steps = Serial.readString().toInt();
          EEPROM_writeAnything(0, configuration);
          Serial.println("pasos: "+ String(configuration.steps) );
        }
        else if ( serialData == 'T'){
          configuration.delayTime = Serial.readString().toInt();
          EEPROM_writeAnything(0, configuration);
          Serial.println("Tiempo (mS): "+ String(configuration.delayTime) );
        }
        else if (serialData == 'R'){
          Serial.println("Reiniciado con exito!");
          digitalWrite(relay, LOW);
          configuration.delayTime = 0; configuration.steps = 0;
          EEPROM_writeAnything(0, configuration);
        }
    }

    lcd.setCursor(0,0);
    lcd.print("P:"); lcd.print(configuration.steps); lcd.print(" Pot:"); lcd.print(perc); lcd.print("%     ");
    lcd.setCursor(0,1);
    lcd.print("Tiempo:"); lcd.print(configuration.delayTime); lcd.print("    ");
    
    if(configuration.delayTime > 0 & configuration.steps>0)
    {
       Serial.print("Girando.");
       stepper.move(configuration.steps);
       Serial.println(); // Salto de linea
       delay(configuration.delayTime); // Paso 2, el motor espera
       digitalWrite(relay, HIGH); // Paso 3, genera una salida
       Serial.println("Tarea completada con exito!");
       delay(500); // Paso 3, espera mientras sube el piston
       digitalWrite(relay, LOW); // Paso 4, apaga la señal de rele y todo vuelve a empezar
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
