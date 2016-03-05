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

#include <Adafruit_GPS.h>

void getLocation(void) {

	// if a sentence is received, we can check the checksum, parse it...
	if (GPS.newNMEAreceived()) {
		if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
			return;  // we can fail to parse a sentence in which case we should just wait for another
	}

	// if millis() or timer wraps around, we'll just reset it
	if (timer > millis())  timer = millis();

	// approximately every 2 seconds or so, print out the current stats
	if (millis() - timer > 2000) { 
		timer = millis(); // reset the timer

		if(GPS.fix)
			updateLocation();
		else
			noFix();

		}

	return;
}

void updateLocation(void) {

	String lineOne;
	String lineTwo;
	char locator[7];

	lineOne = String(GPS.latitudeDegrees,4) + " " + String(GPS.longitudeDegrees,4);

	calculateMaidenhead(&locator[0], GPS.latitudeDegrees, GPS.longitudeDegrees);
	lineTwo = String(locator) + "  ";

	if(GPS.hour < 10)
		lineTwo += ("0" + String(GPS.hour, DEC) + ":");
	else
		lineTwo += (String(GPS.hour, DEC) + ":");

	if(GPS.minute < 10)
		lineTwo += ("0" + String(GPS.minute, DEC) + ":");
	else
		lineTwo += (String(GPS.minute, DEC) + ":");


	if(GPS.seconds < 10)
		lineTwo += ("0" + String(GPS.seconds, DEC));
	else
		lineTwo += (String(GPS.seconds, DEC));

	lcd.setCursor(0,0);
	lcd.print(lineOne);
	lcd.setCursor(0,1);
	lcd.print(lineTwo);

	// Serial.print("\nTime: ");
	// Serial.print(GPS.hour, DEC); Serial.print(':');
	// Serial.print(GPS.minute, DEC); Serial.print(':');
	// Serial.print(GPS.seconds, DEC); Serial.print('.');
	// Serial.println(GPS.milliseconds);
	// Serial.print("Date: ");
	// Serial.print(GPS.day, DEC); Serial.print('/');
	// Serial.print(GPS.month, DEC); Serial.print("/20");
	// Serial.println(GPS.year, DEC);
	// Serial.print("Fix: "); Serial.print((int)GPS.fix);
	// Serial.print(" quality: "); Serial.println((int)GPS.fixquality); 
	// Serial.print("Location: ");
	// Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
	// Serial.print(", "); 
	// Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
	// Serial.print("Location (in degrees, works with Google Maps): ");
	// Serial.print(GPS.latitudeDegrees, 4);
	// Serial.print(", "); 
	// Serial.println(GPS.longitudeDegrees, 4);
	// Serial.print(locator);
	// Serial.print("Speed (knots): "); Serial.println(GPS.speed);
	// Serial.print("Angle: "); Serial.println(GPS.angle);
	// Serial.print("Altitude: "); Serial.println(GPS.altitude);
	// Serial.print("Satellites: "); Serial.println((int)GPS.satellites);

	return;
}

void noFix(void) {
	lcd.setCursor(0,0);
	lcd.print("xx.xxxx xxx.xxxx");
	lcd.setCursor(0,1);
	lcd.print("    (NO FIX)    ");
	return;
}

void calculateMaidenhead(char *locator, double lat, double lon) {

  int o1, o2, o3;
  int a1, a2, a3;
  double remainder;

  // longitude
  remainder = lon + 180.0;
  o1 = (int)(remainder / 20.0);
  remainder = remainder - (double)o1 * 20.0;
  o2 = (int)(remainder / 2.0);
  remainder = remainder - 2.0 * (double)o2;
  o3 = (int)(12.0 * remainder);

  // latitude
  remainder = lat + 90.0;
  a1 = (int)(remainder / 10.0);
  remainder = remainder - (double)a1 * 10.0;
  a2 = (int)(remainder);
  remainder = remainder - (double)a2;
  a3 = (int)(24.0 * remainder);

  /* create locator string */
  locator[0] = (char)o1 + 'A';
  locator[1] = (char)a1 + 'A';
  locator[2] = (char)o2 + '0';
  locator[3] = (char)a2 + '0';
  locator[4] = (char)o3 + 'a';
  locator[5] = (char)a3 + 'a';
  locator[6] = (char)0;

  return;
}

// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
#ifdef UDR0
	if (GPSECHO)
		if (c) UDR0 = c;  
    // writing direct to UDR0 is much much faster than Serial.print 
    // but only one character can be written at a time. 
#endif
}

void useInterrupt(boolean v) {
	if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
	OCR0A = 0xAF;
	TIMSK0 |= _BV(OCIE0A);
	usingInterrupt = true;
	} else {
		// do not call the interrupt function COMPA anymore
		TIMSK0 &= ~_BV(OCIE0A);
		usingInterrupt = false;
	}
}