/*!	\file	Latitude.h
 	\author G. Meini
 	\date	20/11/2011
*/

#define NORD	1
#define SUD		-1

/*!	\class Latitude
 	\brief classe per la rappresentazione delle latitudini
*/
class Latitude {

private:

int orientation;		// -1 => Sud, +1 => Nord
unsigned int degree;	// gradi
unsigned int minute;	// primi
unsigned int second;	// secondi

public:

/*!	\fn Latitude
    \brief costruttore
    \param d gradi (positivi per Nord, negativi per Sud)
    \param m primi
    \param s secondi
	\param o orientamento (-1 => S, +1 => N)
    \note se la latitudine non e' corretta viene impostata a 0^0'0"
*/
Latitude(unsigned int d=0, unsigned int m=0, unsigned int s=0, int o = 1)
{
 degree = 0;
 minute = 0;
 second = 0;
 orientation = 1;
 if (d<=89 && m<=59 && s<=59)
   {
	degree = d; minute = m; second = s;
   }
 else
     {
	  if (d==90 && m==0 && s==0)
	    {
		 degree = d; minute = 0; second = 0;
	    }
	  else
		  return;
      }
 if (o>=0)
   orientation = +1;
 else
	 orientation = -1; 
}
/*!	\fn getOrientation
    \return orientamento (-1=> S, +1 => N)
*/
int getOrientation(void)
{
 return orientation;
}
/*!	\fn getDegree
    \return gradi (0 - 89)
*/
unsigned int getDegree(void)
{
 return degree;
}
/*!	\fn getMinute
    \return primi (0-59)
*/
unsigned int getMinute(void)
{
 return minute;
}
/*!	\fn getSecond
    \return secondi (0-59)
*/
unsigned int getSecond(void)
{
 return second;
}
/*! \fn getLatitude
	\return latitudine in formato decimale (positiva per Nord, negativa per Sud)
*/
double getLatitude(void)
{
 double latitude = (double)degree + (double)minute/60. + (double)second/3600.;
 if (orientation < 0)
   return (-latitude);
 else
     return latitude;
}
};
