#ifndef SWRDUINO_H
#define SWRDUINO_H

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

void useInterrupt(boolean); 
void showIntro(void);
void lcdON(void);
void LCD_Setup(LiquidCrystal lcd);
void lcdOFF(void);
void getSWR(void);
void getLocation(void);

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

char alpha[26] = {	'A','B','C','D','E','F','G',
					'H','I','J','K','L','M','N',
					'O','P','Q','R','S','T','U',
					'V','W','X','Y','Z'};

#endif