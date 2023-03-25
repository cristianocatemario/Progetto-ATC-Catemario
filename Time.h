/*!	\file	Time.h
 	\author G. Meini
 	\date	30/07/2011
*/

/*!	\class Time
 	\brief classe per la gestione degli orari
*/
class Time {

private:

unsigned int hour;		// ore
unsigned int minute;	// minuti
unsigned int second;	// secondi

public:

/*!	\fn Time
    \brief costruttore
    \param h ora
    \param m minuti
    \param s secondi
    \note se l'orario non e' corretta viene impostata l'orario 00:00:00
*/
Time(unsigned int h=0, unsigned int m=0, unsigned int s=0)
{
 hour = 0;
 minute = 0;
 second = 0;
 if (h>23)
   return;
 if (m>59)
   return;
 if (s>59)
   return;
 hour = h;
 minute = m;
 second = s;
}
/*!	\fn getHour
    \return ora (0-23)
*/
unsigned int getHour(void)
{
 return hour;
}
/*!	\fn getMinute
    \return minuti (0-59)
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
/*! \fn hourDifference
	\brief calcola la differenza in ore da un orario
	\param t orario rispetto al quale calcolare la differenza
	\return ore trascorse dall'orario fornito (negativo se precedente)
*/
double hourDifference(Time t)
{
 double my_decimal_hour, decimal_hour;

 my_decimal_hour = (double)hour + (double)minute/60. + (double)second/3600.;
 decimal_hour = (double)t.hour + (double)t.minute/60. + (double)t.second/3600.;

 return (decimal_hour - my_decimal_hour);
}
};
