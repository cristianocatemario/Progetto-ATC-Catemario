/*!	\file	Position.h
 	\author G. Meini
	\version 0.0
 	\date	30/07/2011
*/
#include "Date.h"
#include "Time.h"
#include "Latitude.h"
#include "Longitude.h"
extern "C"
{
#include "GPS2UTM.H"
}
/*!	\class Position
 	\brief classe per la rappresentazione di una posizione geografica associata ad un istante temporale
*/
class Position {

private:

//! \var date
//! \brief data
Date date;				
//! \var time
//! \brief orario
Time time;		
//! \var latitude
//! \brief latitudine
Latitude latitude;
//! \var longitude
//! \brief longitudine
Longitude longitude;	

public:

/*!	\fn Position
    \brief costruttore predefinito
*/
Position(void)
{
 Date d(1,1,2000);
 date = d;
 Time t(0,0,0);
 time = t;
 Latitude lat(0,0,0,NORD);
 latitude = lat;
 Longitude lon(0,0,0,EST);
 longitude = lon;
}
/*!	\fn Position
    \brief costruttore
    \param d data
    \param t orario
    \param lat latitudine
	\param lon longitudine
*/
Position(Date d, Time t, Latitude lat, Longitude lon)
{
 date = d;
 time = t;
 latitude = lat;
 longitude = lon;
}
/*!	\fn getDate
    \return data
*/
Date getDate(void)
{
 return date;
}
/*!	\fn getTime
    \return orario
*/
Time getTime(void)
{
 return time;
}
/*!	\fn getLatitude
    \return latitudine
*/
Latitude getLatitude(void)
{
 return latitude;
}
/*!	\fn getLongitude
    \return longitudine
*/
Longitude getLongitude(void)
{
 return longitude;
}
/*!	\fn getNorth
    \return coordinata UTM Nord (metri)
*/
double getNorth(void)
{
 double north, east;

 GPS2UTM(latitude.getLatitude(), longitude.getLongitude(), &east, &north);
 return north;
}
/*!	\fn getEast
    \return coordinata UTM Est (metri)
*/
double getEast(void)
{
 double north, east;

 GPS2UTM(latitude.getLatitude(), longitude.getLongitude(), &east, &north);
 return east;
}
/*!	\fn distance
	\brief calcola la distanza spaziale in metri da una posizione
	\param  p posizione rispetto alla quale calcolare la distanza
    \return distanza (metri)
*/
double distance(Position p)
{
 double dist = sqrt(pow(getNorth()-p.getNorth(),2)+pow(getEast()-p.getEast(),2));
 return dist;
}
/*!	\fn elapsedTime
	\brief calcola la differenza temporale in secondi da una posizione
	\param p posizione rispetto alla quale calcolare la differenza
    \return differenza (secondi)
*/
int elapsedTime(Position p)
{
 double hours;
 int seconds;

 hours = double(date.dayDifference(p.date)*24) + time.hourDifference(p.time);
 seconds = int(hours*3600.);
 return seconds;
}
};
