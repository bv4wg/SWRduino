/**********************************************************************
 * SWRduino - SWR meter with Maidenhead/GPS location
 * Copyright (C) 2016  Howard Graves (hgraves@hiwaay.net)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************/

// include the library code:
#include <LiquidCrystal.h>
//#include <OneWire.h>
#include "SWRduino.h"
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
//#define __SIM


int currentPeakPower;
int TESTMODE = 0;

const int backlight =  10;
const int switchpin =   6;


int i=0;
int reversePowerPin = A6;
int forwardPowerPin = A5;
int reversePower = 0;
int forwardPower = 0;
float calForwardValue;
float calReverseValue;
float calForwardPeak;
float swr;
uint32_t  dimTimer;
uint32_t  loopTimer;
uint32_t  persistanceTimer;
int modesw;
boolean noDim = false;

char mode;
char firstFix = 0;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int dbar;
int bars;

SoftwareSerial mySerial(8, 7);
Adafruit_GPS GPS(&mySerial);

#define GPSECHO  false
boolean usingInterrupt = false;


void setup() {
	Serial.begin(115200);

	pinMode(backlight, OUTPUT);
	pinMode(switchpin, INPUT);

	// Power up with button down for test mode
	modesw = digitalRead(switchpin);
	if(modesw) TESTMODE = 1;
 
	while(modesw) {
		modesw = digitalRead(switchpin);
	}

	lcd.createChar(0, bar1);  
	lcd.createChar(1, bar2);  
	lcd.createChar(2, bar3); 
	lcd.begin(16, 2);

	showIntro();
	lcdON();

	modesw = digitalRead(switchpin);
	if(modesw) TESTMODE = 1;

	while(modesw) {
		modesw = digitalRead(switchpin);
	}

	currentPeakPower = 0;
	mode = SWRMODE;


	GPS.begin(9600);
	GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
	//GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
	GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
	//GPS.sendCommand(PGCMD_ANTENNA);

	useInterrupt(true);

	delay(3000);

	LCD_Setup(lcd);

	dimTimer = millis();
	loopTimer = millis();
	persistanceTimer = millis();
}

uint32_t timer = millis();
void loop() {

	delay(10);
	
	if(dimTimer > millis()) dimTimer = millis();
  	if(loopTimer > millis()) loopTimer = millis();
  	if(persistanceTimer > millis()) persistanceTimer = millis();

	modesw = digitalRead(switchpin);

	if(modesw) {
		dimTimer = millis();
		switch(mode) {
		case SWRMODE:
			mode = LOCATION;
			digitalWrite(backlight, HIGH);
			break;
		case SWRMODE_DIM:
			mode = SWRMODE;
			digitalWrite(backlight, HIGH);
			LCD_Setup(lcd);
			break;
		case LOCATION:
			mode = SWRMODE;
			digitalWrite(backlight, HIGH);   
			LCD_Setup(lcd);            
			break;
		case LOCATION_DIM:
			mode = LOCATION;
			digitalWrite(backlight, HIGH); 
			break;
		default:
			mode = SWRMODE;
			Serial.write("default");
			break;
		}
		delay(100);			// make sure we don't change modes more than once
  	}


	if(millis() - dimTimer > (DIM_TIMER * 1000)) {
		mode += 10;
		dimTimer = millis();
		lcdOFF();
	}


	if(firstFix==0) {
		if(GPS.fix){
			firstFix = 1;
			lcdON();
		}
	}

	if(millis() - loopTimer > 500) {
		switch(mode) {
			case SWRMODE:
			case SWRMODE_DIM:
				getSWR();
				break;
			case LOCATION:
			case LOCATION_DIM:
				getLocation();
		  		break;
			default:
				break;
		}
		loopTimer = millis();
	}
}
