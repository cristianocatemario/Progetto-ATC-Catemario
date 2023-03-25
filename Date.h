/*!	\file	Date.h
 	\author G. Meini
 	\date	30/07/2011
*/

/*!	\class Date
 	\brief classe per la gestione delle date
*/
class Date {

private:

unsigned int day;	// giorno
unsigned int month;	// mese
unsigned int year;	// anno

/*!	\fn julianNumber
    \brief calcola il numero giuliano (giorni trascorsi dal 1 Gennaio 4713 a.C.) di una data
    \see en.wikipedia.org/wiki/Julian_day
    \param data data di cui calcolare il numero giuliano
    \return numero giuliano della data fornita
*/
int julianNumber(Date d)
{
 int c, y, m;
 
 c = (14-d.getMonth())/12;
 y = d.getYear()+4800-c;
 m = d.getMonth()+12*c-3;

 return d.getDay()+(153*m+2)/5+365*y+y/4-y/100+y/400-32045;
}

public:

/*!	\fn Date
    \brief costruttore
    \param d giorno
    \param m mese
    \param y anno (d.C.)
    \note se la data non e' corretta viene impostata la data 1 Gennaio 2000
*/
Date(unsigned int d=1, unsigned int m=1, unsigned int y=2000)
{
 day = 1;
 month = 1;
 year = 2000;
 if (y<1 || m<1 || m>12 || d<1)
   return;
 if (m==1 || m==3 || m==5 || m==7 || m==8 || m==10 || m==12)
   if (d>31)
	 return;
 if (m==4 || m==6 || m==9 || m==11)
   if (d>30)
	 return;
 if (m==2 && y%4==0) // bisestile
   if (d>29) return;
 if (m==2 && y%4!=0) // non bisestile
   if (d>28) return;
 year = y;
 month = m;
 day = d;
}
/*!	\fn getDay
    \return giorno (1-31)
*/
unsigned int getDay(void)
{
 return day;
}
/*!	\fn getMonth
    \return mese (1-12)
*/
unsigned int getMonth(void)
{
 return month;
}
/*!	\fn getYear
    \return anno
*/
unsigned int getYear(void)
{
 return year;
}
/*!	\fn dayDifference
    \brief calcola la differenza in giorni da una data
    \param d data rispetto alla quale calcolare la differenza
    \return giorni trascorsi fino alla data fornita (negativo se precedente)
*/
int	dayDifference(Date d)
{
 int my_jn, jn;

 my_jn = julianNumber(*this);
 jn = julianNumber(d);

 return (jn - my_jn);
}
};
