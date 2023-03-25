/*!	\file	COM.H
*/
#include <windows.h>

#define READ   			0
#define WRITE   		1
#define READ_WRITE  	2
#define NO_HANDSHAKE	0
#define HW_HANDSHAKE	1
#define BUFFER_DIM		1024 // 1Kbyte

/*! 
	\brief apre una porta seriale impostandone i parametri di comunicazione
	\param port numero di porta seriale (0, 1, 2, ..., 9)
	\param mode READ (0), WRITE (1) o READ_WRITE (2)
	\param speed baudrate (9600, 19200, 38400 o 57600)
	\param parity N/n (NONE), E/e (EVEN), O/o (ODD)
	\param bits bits/char (7 o 8)
	\param stop stop-bits (1 o 2)
	\param flow NO_HANDSHAKE (0) o HW_HANDSHAKE (1)
	\return un identificatore da utilizzare nelle funzioni di trasmissione/ricezione o un valore negativo in caso di errore
*/
HANDLE COM_open(unsigned char port, unsigned char mode, unsigned short speed, char parity, unsigned char bits, unsigned char stop, unsigned char flow);
/*! 
	\brief trasmette un buffer di caratteri alla porta seriale
	\param com identificatore restituito dalla funzione COM_open
	\param buf buffer dei caratteri
	\param n numero di caratteri nel buffer
	\return numero di caratteri trasmessi o un valore negativo in caso di errore
*/
int COM_write(HANDLE com, char buf[], int n);
/*! 
	\brief riceve in un buffer i caratteri dalla porta seriale
	\param com identificatore restituito dalla funzione COM_open
	\param buf buffer dei caratteri
	\param n numero massimo di caratteri da ricevere nel buffer
	\return numero di caratteri ricevuti o un valore negativo in caso di errore
*/
int COM_read(HANDLE com, char *buf, int n);
/*! 
	\brief chiude la porta seriale
	\param com identificatore restituito dalla funzione COM_open
*/
void COM_close(HANDLE com);
