
#include <stdio.h>
#include <string.h>

#include "com.h"

static HANDLE handle = NULL;

static int lat_deg = 43;
static int lat_min = 29;
static int lat_sec = 57;
static int lon_deg = 10;
static int lon_min = 19;
static int lon_sec = 26;
static int day = 6;
static int month = 9;
static int year = 2011;
static int hour = 9;
static int minute = 0;
static int second = 0;


HANDLE COM_open(unsigned char port, unsigned char mode, unsigned short speed, char parity, unsigned char bits, unsigned char stop, unsigned char flow)
{
 if (port < 1 || port > 9)
   return NULL;

 handle = (HANDLE)(99);
 return handle;
}

int COM_write(HANDLE com, char buf[], int n)
{
 if (com != handle)
   return -1;
 return 0;
}

int COM_read(HANDLE com, char *buf, int n)
{
 char NMEA_string[128], NMEA_crc[8];
 unsigned char crc = 0x00;
 int i;

 if (com != handle)
   return -1;
 
 sprintf(NMEA_string, "$GPRMC,%02i%02i%02i,A,%02i%02.4f,N,%03i%02.4f,E,0.00,000.00,%02i%02i%02i*",
	     hour,minute,second,
		 lat_deg,((double)lat_min+(double)lat_sec/60.0),
		 lon_deg,((double)lon_min+(double)lon_sec/60.0),
		 day,month,(year-2000));
 
 for (i=1; NMEA_string[i]!='*'; i++)
    crc ^= NMEA_string[i];
 sprintf(NMEA_crc,"%02X\r\n",crc);
 strcpy(buf, NMEA_string);
 strcat(buf, NMEA_crc);

 return strlen(buf);
}

void COM_close(HANDLE com)
{
 handle = NULL;
 return;
} 
