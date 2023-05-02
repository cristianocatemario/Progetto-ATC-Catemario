/*!	\file	main.cpp
	\author G. Meini
	\date	20/11/2011
*/
#include <cstdio>
#include "ATC.H"
/*!	\property gps
	\brief variabile puntatore alla porta seriale GPS
*/
GPS* gps;
/*!	\property active
	\brief variabile di tipo bool per la ricerca dell'attivazione della porta o meno
*/
bool active;

/*!	\class update
	\brief thread di invocazione del metodo update della classe GPS
*/
unsigned long WINAPI update(void* arg)
{
 while (active)
      {
	   (*gps).update();
	   Sleep(140);
      }
 ExitThread(0);
}

/*!	\class main
	\brief main
*/
int main(int argc, char* argv[])
{
/*!	\property GPS_port
	\brief variabile per la ricerca della porta seriale GPS
*/
 unsigned char GPS_port;
 /*! 
	\property command
	\brief comando che eseguirà la porta GPS
*/
 char command[16];
 /*!
	\property password
	\brief password per accedere alla porta GPS
*/
 char password[32];
 /*!
   \property filename
   \brief nome del file per accedere alla porta GPS
*/
 char filename[64];
 HANDLE thread;
 /*!
   \property speed
   \brief velocità con il quale viaggia il dispositivo GPS
*/
 double speed;
 /*!
   \property direzione
   \brief direzione per il quale viaggia il dispositivo GPS
*/
 double direction;
 /*!
   \property totalDistance
   \brief distanza totale percorsa dal dispositivo GPS
*/
 double totalDistance;
 /*!
   \property elapsedTime
   \brief tempo trascorso in una determinata distanza
*/
 double elapsedTime;
 /*!
   \property meanSpeed
   \brief velocità media del dispositivo GPS
*/
 double meanSpeed;
 /*!
   \property pos
   \brief variabile di tipo pos
*/
 Position pos;
 /*!
   \property day
   \brief valore che identifica il giorno inserito dall'utente nel dispositivo GPS
*/
 unsigned int day;
 /*!
   \property month
   \brief valore che identifica il mese inserito dall'utente nel dispositivo GPS
*/
 unsigned int month;
 /*!
   \property year
   \brief valore che identifica l'anno inserito dall'utente nel dispositivo GPS
 */
 unsigned int year;
 /*!
   \property hour
   \brief valore che identifica l'ora inserito dall'utente nel dispositivo GPS
 */
 unsigned int hour;
 /*!
   \property minute
   \brief valore che identifica il minuto inserito dall'utente nel dispositivo GPS
*/
 unsigned int minute;
 /*!
   \property second
   \brief valore che identifica il secondo inserito dall'utente nel dispositivo GPS
   */
 unsigned int second;

 if (argc != 2)
   {
	printf("%s GPS-port\r\n",argv[0]);
    return -1;
   }
 GPS_port = (unsigned char)atoi(argv[1]);
 
 gps = GPS::getGPS(GPS_port);
 ATC atc;
 if (atc.isTrip())
   {
    (*gps).subscribe(&atc);
    active = true;
	thread = CreateThread(NULL,1024,&update,NULL,0,NULL);
   }

 do {  //ciclo del menu di comandi
	 printf("B: Begin!\r\n");
	 printf("E: End!\r\n");
	 printf("I: Info?\r\n");
	 printf("A: Actual?\r\n");
	 printf("P: Past?\r\n");
	 printf("S: Save!\r\n");
	 printf("X: eXit!\r\n");
	 printf("J: Journey?\r\n");
	 scanf("%s",command);
     
	 switch (command[0])
	       {
			case 'B':	
			case 'b':	printf("Password? ");
						scanf("%s",password);
						atc.beginJourney(password);
						(*gps).subscribe(&atc);
						if (!active)
						  {
						   active = true;
						   thread = CreateThread(NULL,1024,&update,NULL,0,NULL);
						  }
						break;
            case 'E':	
			case 'e':	printf("Password? ");
						scanf("%s",password);
						(*gps).unsubscribe();
						atc.endJourney(password);
						if (active)
						  {
						   active = false;
						   WaitForSingleObject(thread,INFINITE);
						  }
						break;
			case 'I':	
			case 'i':	atc.getJourneyInfo(&totalDistance, &elapsedTime, &meanSpeed);
						printf("Total distance: %f\r\n",totalDistance);
						printf("Elapsed time: %f\r\n",elapsedTime);
						printf("Mean speed: %f\r\n",meanSpeed);
						break;
			case 'A':	
			case 'a':	pos = atc.getActualPosition(&speed, &direction);
				        printf("Date: %02i/%02i/%i\r\n",pos.getDate().getDay(),pos.getDate().getMonth(),pos.getDate().getYear());
						printf("Time: %02i:%02i:%02i\r\n",pos.getTime().getHour(),pos.getTime().getMinute(),pos.getTime().getSecond());
						printf("Latitude: %i^%02i'%02i\"%c\r\n",pos.getLatitude().getDegree(),pos.getLatitude().getMinute(),pos.getLatitude().getSecond(),(pos.getLatitude().getOrientation()>0 ? 'N' : 'S'));
						printf("Longitude: %i^%02i'%02i\"%c\r\n",pos.getLongitude().getDegree(),pos.getLongitude().getMinute(),pos.getLongitude().getSecond(), (pos.getLongitude().getOrientation()>0 ? 'E' : 'W'));
						printf("Speed: %fKm/h\r\n",speed);
						printf("Route: %f^\r\n",direction);
						break;
			case 'S':	
			case 's':	printf("Filename? ");
						scanf("%s",filename);
						printf("Password? ");
						scanf("%s",password);
						atc.saveToFile(filename, password);
						break;
			case 'X':	
			case 'x':	break;
			case 'J':
			case 'j':   if (atc.isTrip())
						  printf("YES\r\n");
						else
							printf("NO\r\n");
				        break;
			case 'P':	
			case 'p':	printf("Password? "); scanf("%s",password);
						printf("Day? "); scanf("%u",&day);
						printf("Month? "); scanf("%u",&month);
						printf("Year? "); scanf("%u",&year);
						Date date(day, month, year);
						printf("Hour? "); scanf("%u",&hour);
						printf("Minute? "); scanf("%u",&minute);
						printf("Second? "); scanf("%u",&second);
						Time time(hour, minute, second);
						pos = atc.getPastPosition(date, time, password);
						printf("Date: %02i/%02i/%i\r\n",pos.getDate().getDay(),pos.getDate().getMonth(),pos.getDate().getYear());
						printf("Time: %02i:%02i:%02i\r\n",pos.getTime().getHour(),pos.getTime().getMinute(),pos.getTime().getSecond());
						printf("Latitude: %i^%02i'%02i\"%c\r\n",pos.getLatitude().getDegree(),pos.getLatitude().getMinute(),pos.getLatitude().getSecond(),(pos.getLatitude().getOrientation()>0 ? 'N' : 'S'));
						printf("Longitude: %i^%02i'%02i\"%c\r\n",pos.getLongitude().getDegree(),pos.getLongitude().getMinute(),pos.getLongitude().getSecond(),(pos.getLongitude().getOrientation()>0 ? 'E' : 'W'));
						break;

	       }
    } while(command[0] != 'X' && command[0] != 'x');
 
 if (active)
   {
    active = false;
    WaitForSingleObject(thread,INFINITE);
   }
 delete gps;
 return 0;
}
