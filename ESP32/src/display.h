#ifndef DISPLAY_H
#define DISPLAY_H


#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

void displaySetup();

void printDisplay();

#endif