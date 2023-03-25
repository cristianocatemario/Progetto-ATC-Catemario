/*!	\file	Longitude.h
 	\author G. Meini
 	\date	20/11/2011
*/

#define EST		1
#define WEST	-1

/*!	\class Longitude
 	\brief classe per la rappresentazione delle longitudini
*/
class Longitude {

private:

int orientation;		// -1 => W, +1 => E
unsigned int degree;	// gradi
unsigned int minute;	// primi
unsigned int second;	// secondi

public:

/*!	\fn Longitude
    \brief costruttore
    \param d gradi (positivi per Est, negativi per Ovest)
    \param m primi
    \param s secondi
	\param o orientamento (-1 => W, +1 => E)
    \note se la latitudine non e' corretta viene impostata a 0^0'0"
*/
Longitude(unsigned int d=0, unsigned int m=0, unsigned int s=0, int o=1)
{
 degree = 0;
 minute = 0;
 second = 0;
 orientation = 1;
 if (d<=179 && m<=59 && s<=59)
   {
	degree = d; minute = m; second = s;
   }
 else
	 {
	  if (d==180 && m==0 && s==0)
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
    \return orientamento (-1=> W, +1 => E)
*/
int getOrientation(void)
{
 return orientation;
}
/*!	\fn getDegree
    \return gradi (0 - 179)
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
/*! \fn getLongitude
	\return longitudine in formato decimale (positiva per Est, negativa per Ovest)
*/
double getLongitude(void)
{
 double longitude = (double)degree + (double)minute/60. + (double)second/3600.;
 
 if (orientation < 0)
   return (-longitude);
 else
     return longitude;
}
};
