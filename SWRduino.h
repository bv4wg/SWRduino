#ifndef SWRDUINO_H
#define SWRDUINO_H

typedef struct {
	uint8_t gpsHour;
	uint8_t gpsMinute;
	uint8_t gpsSeconds;
	uint16_t gpsMilliseconds;
	uint8_t gpsDay;
	uint8_t gpsMonth;
	uint8_t gpsYear;
	boolean gpsFix;
	uint8_t gpsFixQuality;
	float gpsLatitude;
	float gpsLongitude;
	char lat;
	char lon;
	char mag;
	int32_t gpsLatitudeFixed;
	int32_t gpsLongitudeFixed;
	float gpsLatitudeDegs;
	float gpsLongitudeDegs;
	float gpsSpeed;
	float gpsAngle;
	float gpsAltitude;
	uint8_t gpsSatellites;
} gpsData_t;

#endif