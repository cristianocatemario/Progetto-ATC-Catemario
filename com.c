
#include <stdio.h>
#include "com.h"


DCB saveDcbFlags; // variabile per salvataggio impostazioni correnti


HANDLE COM_open(unsigned char port, unsigned char mode, unsigned short speed, char parity, unsigned char bits, unsigned char stop, unsigned char flow)
{
 HANDLE com;
 COMMTIMEOUTS timeouts;
 DCB dcb;
 char com_port[] = "COM?";
 
 if (port < 1 || port > 9)
   return NULL;

 com_port[3] = (port+'0'); // trasformazione cifra numerica in carattere
 
 // apertura file speciale
 switch (mode)
	{
	 case READ:			com=CreateFile(com_port,GENERIC_READ,0,0,OPEN_EXISTING,0,0);
						break;
	 case WRITE:		com=CreateFile(com_port,GENERIC_WRITE,0,0,OPEN_EXISTING,0,0);
						break;
	 case READ_WRITE:	com=CreateFile(com_port,(GENERIC_READ|GENERIC_WRITE),0,0,OPEN_EXISTING,0,0);
						break;
	 default:			return NULL;
	}

if (com == INVALID_HANDLE_VALUE)
  return NULL;

// acquisizione e salvataggio impostazioni correnti per successivo ripristino 
if (!GetCommState(com, &dcb))
  {
   CloseHandle(com);
   return NULL;
  }
saveDcbFlags = dcb;

// impostazione dimensione buffer di ricezione e trasmissione
if (!SetupComm(com, BUFFER_DIM, BUFFER_DIM))
  {
   CloseHandle(com);
   return NULL;
  }

// impostazioni modalita’ non bloccante in lettura
timeouts.ReadIntervalTimeout = 0;
timeouts.ReadTotalTimeoutMultiplier = 0;
timeouts.ReadTotalTimeoutConstant = 1;
timeouts.WriteTotalTimeoutConstant = 0;
timeouts.WriteTotalTimeoutMultiplier = 0;
if (!SetCommTimeouts(com, &timeouts))
  {
   CloseHandle(com);
   return NULL;
  }

// impostazione baudrate
switch (speed)
      {
       case 4800:	dcb.BaudRate = CBR_4800;
					break;
       case 9600:	dcb.BaudRate = CBR_9600;
					break;
	   case 19200:	dcb.BaudRate = CBR_19200;
					break;
	   case 38400:	dcb.BaudRate = CBR_38400;
					break;
	   case 57600:	dcb.BaudRate = CBR_57600;
					break;
	   default:		CloseHandle(com);
					return NULL;
      }

// impostazione numero bit carattere
switch (bits)
      {
       case 7:	dcb.ByteSize = 7;
				break;
	   case 8:	dcb.ByteSize = 8;
				break;
	   default:	CloseHandle(com);
				return NULL;
      }

// impostazione modalità "raw"
dcb.DCBlength = sizeof(DCB);
dcb.EofChar = 0;
dcb.ErrorChar = 0;
dcb.EvtChar = 0;
dcb.fAbortOnError = 0;
dcb.fBinary = 1;
dcb.fDsrSensitivity = 0;

// impostazione CTS/RTS handshake
switch (flow)
      {
	   case NO_HANDSHAKE:	dcb.fOutxCtsFlow = 0;
							dcb.fRtsControl = RTS_CONTROL_DISABLE;
							break;
	   case HW_HANDSHAKE:	dcb.fOutxCtsFlow = 1;
							dcb.fRtsControl = RTS_CONTROL_ENABLE;
							break;
	   default:				CloseHandle(com);
							return NULL;
      }
dcb.fDtrControl = DTR_CONTROL_DISABLE;
dcb.fInX = 0;
dcb.fNull = 0;
dcb.fOutX = 0;
dcb.fOutxDsrFlow = 0;
dcb.fTXContinueOnXoff = 0;
dcb.XoffChar = 0;
dcb.XonChar = 0;
dcb.XonLim = 1;

// impostazione parita’
switch (parity)
      {
       case 'n':
       case 'N':	dcb.fParity = 0;
					dcb.fErrorChar = 0;
					dcb.Parity = NOPARITY;
					break;
       case 'o':
       case 'O':	dcb.fParity = 1;
					dcb.fErrorChar = 0;
					dcb.Parity = ODDPARITY;
					break;
	   case 'e':
	   case 'E':	dcb. fParity = 1;
					dcb.fErrorChar = 0;
					dcb.Parity = EVENPARITY;
					break;
	   default:		CloseHandle(com);
					return NULL;
      }
       
// impostazione numero stop bit
switch (stop)
      {
	   case 1:		dcb.StopBits = ONESTOPBIT;
					break;
	   case 2:		dcb.fErrorChar = TWOSTOPBITS;
					break;
	   default:		CloseHandle(com);
					return NULL;
      }

// configurazione porta seriale con impostazioni effettuate
if (!SetCommState(com, &dcb))
  {
   CloseHandle(com);
   return NULL;
  }

// svuotamento buffer di ricezione e trasmissione 
if (!PurgeComm(com, (PURGE_TXCLEAR|PURGE_RXCLEAR)))
  {
   CloseHandle(com);
   return NULL;
  }

 return com;
}


int COM_write(HANDLE com, char buf[], int n)
{
 unsigned long i;

 if (!WriteFile(com, buf, n, &i,  0))
   return -1;
 return (int)(i);
}

int COM_read(HANDLE com, char *buf, int n)
{
 unsigned long i;

 if (!ReadFile(com, buf, n, &i, 0))
   return -1;

 return (int)(i);
}

void COM_close(HANDLE com)
{
 SetCommState(com, &saveDcbFlags);
 CloseHandle(com);
 return;
} 
