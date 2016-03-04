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

 #include "Arduino.h"

void LCD_Setup(LiquidCrystal lcd) {
    lcd.setCursor(0,0);
    lcd.print("FWD xx.x SWR x.x");
    lcd.setCursor(0,1);
    lcd.print("                ");

    return;
}

void updatePower(float value) {
  lcd.setCursor(POWER_VAL,0);
  if(value<10.0) {
    lcd.print(" ");
    lcd.print(value,1);
  } else {
    lcd.print(value,1);
  }
  
  return;
}

void updateSWR(float value) {
  lcd.setCursor(SWR_VAL,0);
  if(value == 0)
    lcd.print("---");
  else if (value >= 10)
    lcd.print(" HI");
  else
    lcd.print(value,1);  

  return;
}

void updateBar(int type, int value, int subbar) {
  if(type==FILL) {
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    for(int i=0; i<value-1;i++)
      lcd.write(1);
  
    lcd.write(subbar); 
  } else {
    lcd.setCursor(value-1,1);

    if(subbar == 1)
      subbar=2;

//    lcd.write(subbar);
    lcd.write(0xff);
  }
}

void getBarValue(float cv, int *newBars, int *newDbar) {
  float m;
  float block;

  m = 100 * cv / (float)BAR_FULLSCALE;
  block = m * 0.16;

  if((block - ((int)block)) < .5)
    *newDbar = BLK1;
  else
    *newDbar = BLK2;

//  Serial.print("sensor = ");
//  Serial.print(m);
//  Serial.print("  block = ");
//  Serial.print(block);
//  Serial.print("  bars = ");
//  Serial.print(bars); 
//  Serial.print("  dbar = ");
//  Serial.println(dbar); 

  *newBars = (int)block;
}

void lcdOFF(void) {
  digitalWrite(backlight, LOW);
  return;
}

