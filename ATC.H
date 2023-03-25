/*!	\file	ATC.H
 	\author G. Meini
	\version 0.0
 	\date	20/11/2011
*/
#include <cstdio>
#include <cstring>

#include "GPS.H"

extern "C"
{
#include "DES.H"
}

#define SEC_PER_WEEK	3600*24*7 // secondi in una settimana
#define PASSWORD_FILE	"password.bin"
#define POSITION_FILE	"position.bin"

/*!	\struct POSITION
 	\brief formato del record del file binario di salvataggio automatico delle posizioni
*/
struct POSITION
{
 unsigned int day;			// giorno
 unsigned int month;		// mese
 unsigned int year;			// anno
 unsigned int hour;			// ore
 unsigned int minute;		// minuti
 unsigned int second;		// secondi
 int lat_sign;				// -1 => S, +1 => N
 unsigned int lat_degree;	// latitudine: gradi
 unsigned int lat_minute;	// latitudine: primi
 unsigned int lat_second;	// latitudine: secondi
 int lon_sign;				// -1 => W, +1 => E
 unsigned int lon_degree;	// longitudine: gradi
 unsigned int lon_minute;	// longitudine: primi
 unsigned int lon_second;	// longitudine: secondi
};

/*!	\class ATC
 	\brief classe principale dell'applicazione Advance Trip Computer
*/
class ATC : public GPSobserver
{
 private:

 //! \var trip
 //! \brief stato dell'applicazione (viaggio in corso)
 bool trip;
 //! \var positionCounter
 //! \brief numero di posizioni acquisite
 unsigned int positionCounter;
 //! \var actualDirection
 //! \brief direzione attuale (in gradi rispetto al Nord)
 double actualDirection;
 //! \var actualSpeed
 //! \brief velocita' attuale (in Km/h)
 double actualSpeed;
 //! \var positions
 //! \brief vettore delle posizioni acquisite
 Position* positions;
 //! \var positionFile
 //! \brief riferimento al file di salvataggio delle posizioni acquisite
 FILE* positionFile;

 /*!
     \fn checkPassword
     \brief verifica la password fornita con la versione cifrata nel file
	 \param password password utente
 */
 bool checkPassword(char password[])
 {
  BYTE clear[8], coded[8], pswd[8];
  int passwordLen = strlen(password);
  FILE* passwordFile; 
  
  // preparazione password (aggiustamento per lunghezza minore/maggiore 8 caratteri)
  if (passwordLen<=8)
    {
	 for (int i=0; i<passwordLen; i++)
	    clear[i] = password[i];
	 for (int i=passwordLen; i<8; i++)
        clear[i] = ' ';
    }
  else
      {
	   for (int i=0; i<8; i++)
	      clear[i] = password[i];
      }
  // codifica password
  DES_encrypt(clear, coded, clear);
  // recupero file codificata da file
  passwordFile = fopen(PASSWORD_FILE,"rb");
  fread(pswd, 8, 1, passwordFile);
  fclose(passwordFile);
  // confronto password
  for (int i=0; i<8; i++)
     if (coded[i] != pswd[i])
	   return false;
  return true;
 }
 /*!
     \fn getTotalDistance
     \brief calcola la distanza totale percorsa
	 \return distanza totale (Km)
	 \note algoritmo potenzialmente inefficiente
 */
 double getTotalDistance(void)
 {
  double distance = 0;

  for (unsigned int i=1; i<positionCounter; i++)
     distance += positions[i].distance(positions[i-1]);

  return distance/1000.0;
 }
 /*!
     \fn getElapsedTime
     \brief calcola il tempo trascorso dalla partenza
	 \return tempo (ore)
 */
 double getElapsedTime(void)
 {
  int time;

  if (positionCounter > 1)
    time = positions[0].elapsedTime(positions[positionCounter-1]);
  else
	  time = 0;
  
  return (double)time/3600.0;
 }
 /*!
     \fn loadFromFile
     \brief carica le posizioni da file
	 \return numero di posizioni caricate, -1 in caso di errore
 */
 int loadFromFile(const char filename[])
 {
  POSITION pos;

  positionFile = fopen(filename, "rb");
  if (positionFile != NULL)
    {
	 positionCounter = 0;
	 while (!feof(positionFile))
	      {
		   if (fread(&pos,sizeof(POSITION),1,positionFile) == 1)
		     {
			  Date date(pos.day, pos.month, pos.year);
			  Time time(pos.hour, pos.minute, pos.second);
			  Latitude lat(pos.lat_degree, pos.lat_minute, pos.lat_second, pos.lat_sign);
			  Longitude lon(pos.lon_degree, pos.lon_minute, pos.lon_second, pos.lon_sign);
			  Position position(date, time, lat, lon);
			  positions[positionCounter] = position;
			  positionCounter++;
		     };
	      }
    }
  else
	  return-1;
  fclose(positionFile);
  return positionCounter;
 }


 public:

 /*!
     \fn ATC
     \brief costruttore
	 \note se esiste il file delle posizioni inizializza l'oggetto nello stato di viaggio in corso
 */
 ATC(void)
 {
  trip = false;
  positionCounter = 0;
  actualDirection = 0.0;
  actualSpeed = 0.0;
  positions = new Position[SEC_PER_WEEK];
  if (loadFromFile(POSITION_FILE) >= 0)
    trip = true;
 }
 /*!
     \fn ATC
     \brief distruttore
 */
 ~ATC(void)
 {
  delete positions;
 }
 /*!
     \fn isTrip
     \brief stato del viaggio
	 \return vero = viaggio in corso, falso = viaggio non in corso
 */
 bool isTrip(void)
 {
  return trip;
 }
 /*!
     \fn beginJourney
     \brief inizio del viaggio e memorizzazione della password
	 \param password password utente
 */
 void beginJourney(char password[])
 {
  BYTE clear[8], coded[8];
  int passwordLen = strlen(password);
  FILE *passwordFile; 

  if (trip) // viaggio in corso
    return;

  // preparazione password (aggiustamento per lunghezza minore/maggiore 8 caratteri)
  if (passwordLen<=8)
    {
	 for (int i=0; i<passwordLen; i++)
	    clear[i] = password[i];
	 for (int i=passwordLen; i<8; i++)
        clear[i] = ' ';
    }
  else
      {
	   for (int i=0; i<8; i++)
	      clear[i] = password[i];
      }
  // codifica password
  DES_encrypt(clear, coded, clear);
  // scrittura password codificata su file
  passwordFile = fopen(PASSWORD_FILE,"wb");
  fwrite(coded, 8, 1, passwordFile);
  fclose(passwordFile);
  // rimozione file delle posizioni
  remove(POSITION_FILE);
  positionCounter = 0;
  trip = true;
 }
 /*!
     \fn endJourney
     \brief fine del viaggio
	 \param password password utente
	 \note se la password e' errata non termina il viaggio
 */
 void endJourney(char password[])
 {
  if (trip == false)
    return;
  if (checkPassword(password))
    {
	 remove(POSITION_FILE);
	 remove(PASSWORD_FILE);
	 positionCounter = 0;
	 trip = false;
    }
 }
 /*!
     \fn getJourneyInfo
     \brief richiesta informazioni sul viaggio
	 \retval totalDistance distanza totale percorsa (Km)
	 \retval elapsedTime tempo trascorso dalla partenza
	 \retval meanSpeed velocita' media del viaggio
 */
 void getJourneyInfo(double* totalDistance, double* elapsedTime, double* meanSpeed)
 {
  *totalDistance = getTotalDistance();
  *elapsedTime = getElapsedTime();
  if (*elapsedTime == 0.0)
    *meanSpeed = 0.0;
  else
	  *meanSpeed = (*totalDistance)/(*elapsedTime);
 }
 /*!
     \fn getPastPosition
     \brief richiesta informazioni su una posizione nel corso del viaggio
	 \param date data richiesta
	 \param time orario richiesto
     \param password password utente
	 \return posizione al tempo indicato
	 \note se la password è errata o il viaggio non e' in corso restituisce la posizione 0^0'0"N 0^0'0"E
 */
 Position getPastPosition(Date date, Time time, char password[])
 {
  Latitude lat;
  Longitude lon;
  Position pos(date, time, lat, lon); // posizione fittizia con data/ora della richiesta
  
  if (positionCounter == 0) // viaggio non in corso
    return pos;
  
  if (checkPassword(password))
    {
     if (positions[0].elapsedTime(pos)<0) // data/ora indicata precedente al momento della partenza
	   return positions[0]; // prima posizione registrata
	 for (unsigned int i=0; i<positionCounter; i++) // ricerca posizione immediatamente successiva alla data/ora richiesta
	    if (positions[i].elapsedTime(pos)<=0)
		  return positions[i];
	 return positions[positionCounter-1]; // ultima posizione registrata
    }
  else
      return pos;
 }
 /*!
     \fn getActualPosition
     \brief richiesta posizione attuale
	 \retval speed velocità (Km/h)
	 \retval direction direzione rispetto al Nord (gradi)
	 \return posizione attuale
	 \note viene restituita l'ultima posizione registrata, o una posizione nulla se il viaggio non è in corso
 */
 Position getActualPosition(double* speed, double* direction)
 {
  Latitude lat;
  Longitude lon;
  Date date;
  Time time;
  Position pos(date, time, lat, lon); // posizione fittizia con data/ora fittizie
  
  *speed = actualSpeed;
  *direction = actualDirection;
  if (positionCounter == 0) // viaggio non in corso
    return pos;
    
  return positions[positionCounter-1];
 }
 /*!
     \fn saveToFile
     \brief salvataggio su file testuale delle posizioni registrate
	 \param filename denominazione/percorso del file
	 \param password password utente
 */
 void saveToFile(char filename[], char password[])
 {
  FILE* positionFile;
  char string[128], tmp[8];
  unsigned char crc;

  if (trip == false)
    return;
  if (!checkPassword(password))
    return;

  positionFile = fopen(filename,"wt");
  for (unsigned int i=0; i<positionCounter; i++)
     {
	  sprintf(string,"$GPRMC,%02i%02i%02i.000,A,%02i%02.4f,%c,%03i%02.4f,%c,0.00,000.00,%02i%02i%02i*",
	          positions[i].getTime().getHour(), positions[i].getTime().getMinute(), positions[i].getTime().getSecond(),
	          positions[i].getLatitude().getDegree(),(double)positions[i].getLatitude().getMinute()+(double)positions[i].getLatitude().getSecond()/60.0,(positions[i].getLatitude().getOrientation()>0 ? 'N' : 'S'),
		      positions[i].getLongitude().getDegree(),(double)positions[i].getLongitude().getMinute()+(double)positions[i].getLongitude().getSecond()/60.0,(positions[i].getLongitude().getOrientation()>0 ? 'E' : 'W'),
		      positions[i].getDate().getDay(), positions[i].getDate().getMonth(), positions[i].getDate().getYear()-2000);
      crc = 0x00;
      for (unsigned int i=1; string[i]!='*'; i++)
         crc ^= string[i];
      sprintf(tmp,"%02X",crc);
      fprintf(positionFile,"%s%s\r\n",string,tmp);
	 }
  fclose(positionFile);
 }
 /*!
     \fn GPSnotify
     \brief funzione ereditata dalla classe GPSobserver ed invocata dalla classe GPS per l'aggiornamento della posizione
	 \param position posizione attuale
	 \param direction direzione rispetto al Nord (gradi)
	 \param speed velocita' (Km/h)
 */
 void GPSnotify(Position position, double direction, double speed)
 {
  POSITION pos;

  if (positionCounter < SEC_PER_WEEK)
    {
	 // inserimento posizione nel vettore delle posizioni e aggiornamento del contatore
	 positions[positionCounter] = position;
	 positionCounter++;
	 // scrittura posizione su file
	 pos.day = position.getDate().getDay();
	 pos.month = position.getDate().getMonth();
	 pos.year = position.getDate().getYear();
	 pos.hour = position.getTime().getHour();
	 pos.minute = position.getTime().getMinute();
	 pos.second = position.getTime().getSecond();
	 pos.lat_sign = position.getLatitude().getOrientation();
	 pos.lat_degree = position.getLatitude().getDegree();
	 pos.lat_minute = position.getLatitude().getMinute();
	 pos.lat_second = position.getLatitude().getSecond();
	 pos.lon_sign = position.getLongitude().getOrientation();
	 pos.lon_degree = position.getLongitude().getDegree();
	 pos.lon_minute = position.getLongitude().getMinute();
	 pos.lon_second = position.getLongitude().getSecond();
	 positionFile = fopen(POSITION_FILE, "ab");
     fwrite(&pos, sizeof(pos), 1, positionFile);
	 fclose(positionFile);
    }
  actualDirection = direction;
  actualSpeed = speed;
 }
};
