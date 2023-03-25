/*!	\file	GPS.H
 	\author G. Meini
	\version 0.0
 	\date	20/11/2011
*/

#include <cstring>
#include <cmath>
#include <cstdlib>
#include <cstdio>

#include "Position.h"

extern "C"
{
#include "com.h"
}

#define TIME_POS		7
#define VALIDITY_POS	14
#define LAT_POS			16
#define LAT_SIGN_POS    26
#define LONG_POS		28
#define LONG_SIGN_POS	39
#define SPEED_POS		41

#define KNOT_TO_KMH		1.852


/*!	\class GPSobserver
 	\brief classe astratta per la notifica degli eventi generati dal dispositivo GPS
*/
class GPSobserver
{
 public:

 virtual void GPSnotify(Position position, double direction, double speed) = 0;
};


/*!	\class GPS
 	\brief classe per l'incapsulazione del dispositivo GPS
	\note la classe realizza un pattern creazionale "Singleton"
*/
class GPS
{
 private:

 //! \var gps
 //! \brief puntatore statico ad un'istanza della classe per realizzare il pattern "Singleton"
 static GPS* gps;

 //! \var linea
 //! \brief stringa per la ricostruzione del messaggio NMEA generato dal dispositivo GPS
 char linea[128];
 //! \var indice
 //! \brief indice della prima posizione disponibile nella stringa di ricostruzione del messaggio NMEA
 unsigned int indice;
 //! \var osservatore
 //! \brief attributo per la registrazione dell'oggetto che richiede di ricevere gli eventi del dispositivo GPS
 GPSobserver* osservatore;
 //! \var com_id
 //! \brief attributo per la memorizzazione dell'identificatore associato alla porta seriale aperta
 HANDLE com_id;
 
 /*!
    \fn GPS
    \brief costruttore
    \param serial_port numero della porta seriale di connessione del dispositivo GPS
  */
 GPS(unsigned char serial_port)
 {
  com_id = COM_open(serial_port, READ, 4800, 'n', 8, 1, NO_HANDSHAKE);
  indice = 0;
  osservatore = NULL;
 }
 GPS(void)
 {
 }
 /*!
	\fn extractData
	\brief metodo di estrazione dei dati da una stringa NMEA di tipo $GPRMC
	\param string stringa NMEA di tipo $GPRMC
	\retval valid validità del dato
	\retval speed velocita' (Km/h)
	\retval dir direzione rispetto al Nord (gradi)
	\return posizione estratta dalla stringa NMEA
 */
 Position extractData(char nmea_string[], unsigned int* speed, unsigned int* dir, bool* valid)
 {
  char substring[16];
  unsigned int degree, hour, minute, second, day, month, year;
  int orientation;
  double tmp;
  int pos, date_pos, dir_pos;

  if (nmea_string[VALIDITY_POS] != 'A') // dati invalidi
    {
	 Position pos;
	 *valid = false;
	 return pos;
    }

  pos = SPEED_POS;
  while (nmea_string[pos] != ',')
	   pos++;
  pos++;
  dir_pos = pos;
  while (nmea_string[pos] != ',')
	   pos++;
  pos++;
  date_pos = pos;
  
  // estrazione orario
  strncpy(substring,&nmea_string[TIME_POS],2);
  substring[2] = '\0';
  hour = (unsigned int)atoi(substring);
  strncpy(substring,&nmea_string[TIME_POS+2],2);
  substring[2] = '\0';
  minute = (unsigned int)atoi(substring);
  strncpy(substring,&nmea_string[TIME_POS+4],2);
  substring[2] = '\0';
  second = (unsigned int)atoi(substring);
  Time time(hour, minute, second);
  
  // estrazione data
  strncpy(substring,&nmea_string[date_pos],2);
  substring[2] = '\0';
  day = (unsigned int)atoi(substring);
  strncpy(substring,&nmea_string[date_pos+2],2);
  substring[2] = '\0';
  month = (unsigned int)atoi(substring);
  strncpy(substring,&nmea_string[date_pos+4],2);
  substring[2] = '\0';
  year = 2000+(unsigned int)atoi(substring);
  Date date(day, month, year);
  
  // estrazione latitudine
  strncpy(substring,&nmea_string[LAT_POS],2);
  substring[2] = '\0';
  degree = (unsigned int)atoi(substring);
  strncpy(substring,&nmea_string[LAT_POS+2],7);
  substring[7] = '\0';
  tmp = atof(substring);
  minute = (unsigned int)floor(tmp);
  tmp = (tmp - floor(tmp))*60;
  second = (unsigned int)floor(tmp+0.5);
  if (nmea_string[LAT_SIGN_POS] == 'S')
    orientation = -1;
  else
	  orientation = +1;
  Latitude lat(degree, minute, second, orientation);
  
  // estrazione longitudine
  strncpy(substring,&nmea_string[LONG_POS],3);
  substring[3] = '\0';
  degree = (unsigned int)atoi(substring);
  strncpy(substring,&nmea_string[LONG_POS+3],7);
  substring[7] = '\0';
  tmp = atof(substring);
  minute = (unsigned int)floor(tmp);
  tmp = (tmp - floor(tmp))*60;
  second = (unsigned int)floor(tmp+0.5);
  if (nmea_string[LONG_SIGN_POS] == 'W')
	orientation = -1;
  else
	  orientation = +1;
  Longitude lon(degree, minute, second, orientation);
  
  Position position(date, time, lat, lon); // creazione oggetto di tipo Position
  
  // estrazione velocita'
  strncpy(substring,&nmea_string[SPEED_POS],dir_pos-SPEED_POS-1);
  substring[dir_pos-SPEED_POS-1] = '\0';
  tmp = atof(substring);
  *speed = (unsigned int)floor(tmp*KNOT_TO_KMH+.5);
  
  // estrazione direzione
  strncpy(substring,&nmea_string[dir_pos],date_pos-dir_pos-1);
  substring[date_pos-dir_pos-1] = '\0';
  tmp = atof(substring);
  *dir = (unsigned int)floor(tmp+.5);
  
  *valid = true;
  return position;
 }
 
 public:
 
 /*!
    \fn getGPS
    \brief metodo di richiesta dell'unica istanza della classe GPS  secondo il pattern "Singleton"
    \param serial_port numero della porta seriale di connessione del dispositivo GPS
	\return puntatore all'unico oggetto di classe GPS
  */
 static GPS* getGPS(unsigned char serial_port)
 {
  if (gps == NULL) // prima invocazione: creazione dell'unica istanza della classe
    gps = new GPS(serial_port);
  return gps;
 }
 /*!
    \fn subscribe
    \brief metodo di registrazione dell'oggetto che richiede la notifica degli eventi del dispositivo GPS
  */
 void subscribe(GPSobserver* oss)
 {
  osservatore = oss;
  return;
 }
 /*!
    \fn unsubscribe
    \brief metodo di de-registrazione dell'oggetto che ha precedentemento richiesto la notifica degli eventi del dispositivo GPS
  */
 void unsubscribe(void)
 {
  osservatore = NULL;
  return;
 }
 /*!
    \fn update
    \brief metodo da invocare periodocamente: effettua la ricezione dei caratteri dal dispositivo GPS e l'inolto dei dati all'osservatore
  */
 void update(void)
 {
  int n,i;
  char buffer[1024];
  unsigned int speed, dir;
  bool valid;

  if (com_id != NULL)
    {
     n = COM_read(com_id, buffer, sizeof(buffer)); // ricezione
	 if (n > 0)
       {
        //printf("%s\r\n",buffer);
        for (i=0; i<n; i++) // ricerca carattere EOT
	       {
		    if (buffer[i] == '$')
		      {
		   	   linea[0] = buffer[i];
			   indice = 1;
			  }
		   else
		      {
			   if (buffer[i] == '\n')
			     {
				  linea[indice] = '\0';
				  if (strncmp(linea,"$GPRMC",6)==0)
				    {
					 Position pos = extractData(linea, &speed, &dir, &valid); // estrazione/formattazione dati da stringa GPS
					 if (valid && osservatore != NULL)
					     {
					      osservatore->GPSnotify(pos, (double)dir, (double)speed); // inoltro informazioni a osservatore
					     }
 				    }
				  indice = 0;
				 }
			   else
				   {
				    linea[indice] = buffer[i];
					indice++;
				   }
			  } // else
		   } // for
	   } // if
	} // if
 }
};

GPS* GPS::gps = NULL;

