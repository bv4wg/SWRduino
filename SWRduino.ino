
// include the library code:
#include <LiquidCrystal.h>
//#include <OneWire.h>
#include "SWRduino.h"

#define __SIM

#define PEAK_HOLD       5
#define POWER_VAL       4
#define SWR_VAL         13
#define BAR_FULLSCALE   5
#define BLK1            0
#define BLK2            1
#define FILL            1
#define NOFILL          0
#define PERSISTANCE     20
#define BACKLIGHT_TIMER 100

#define LCD_OFF         0
#define SWRMODE         1
#define SWRMODE_DIM     11
#define LOCATION        2
#define LOCATION_DIM    12

int currentPeakPower;

const int backlight =  10;
const int switchpin =   6;

byte bar1[8] = {
  0b01000,
  0b01000,
  0b01000,
  0b01000,
  0b01000,
  0b01000,
  0b01000,
  0b01000
};

byte bar2[8] = {
  0b01001,
  0b01001,
  0b01001,
  0b01001,
  0b01001,
  0b01001,
  0b01001,
  0b01001
};

byte bar3[8] = {
  0b00001,
  0b00001,
  0b00001,
  0b00001,
  0b00001,
  0b00001,
  0b00001,
  0b00001
};

  int i=0;
  int reversePowerPin = A6;
  int forwardPowerPin = A5;
  int reversePower = 0;
  int forwardPower = 0;
  float calForwardValue;
  float calReverseValue;
  float calForwardPeak;
  float swr;
  int loopCount;
  int modesw;

  char mode;
  
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int dbar;
int bars;
  
void setup() {
  Serial.begin(115200);
  pinMode(backlight, OUTPUT);
  pinMode(switchpin, INPUT);
  
  lcd.createChar(0, bar1);  
  lcd.createChar(1, bar2);  
  lcd.createChar(2, bar3); 
  lcd.begin(16, 2);
  LCD_Setup(lcd);
  digitalWrite(backlight, HIGH);

  currentPeakPower = 0;
  mode = SWRMODE;

}

void loop() {

  delay(100);
  loopCount++;

  modesw = digitalRead(switchpin);

  if(modesw) {
    loopCount = 0;
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
        break;
    }
  }

  if(loopCount > 200) {
    mode += 10;
    loopCount = 0;
    lcdOFF();
  }

  switch(mode) {
    case SWRMODE:
      getSWR();
      break;
    case LOCATION:
      getLocation();
      break;
    default:
      break;
  }
}
