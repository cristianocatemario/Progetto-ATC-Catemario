/*!	\file	DES.H
*/
#define CODE    0
#define DECODE	1

typedef unsigned char BYTE; // 8 bit
typedef unsigned short int WORD; // 16 bit

/*!
  \brief codifica un blocco di 8 byte con algoritmo DES
  \param in blocco da codificare (8 byte)
  \param pswd password di codifica (8 byte)
  \retval out blocco codificato (8 byte)
*/
void DES_encrypt(BYTE in[8], BYTE out[8], BYTE pswd[8]);
/*!
  \brief decodifica un blocco di 8 byte codificato con algoritmo DES
  \param in blocco da decodificare (8 byte)
  \param pswd password di decodifica (8 byte)
  \retval out blocco decodificato (8 byte)
*/
void DES_decrypt(BYTE in[8], BYTE out[8], BYTE pswd[8]);
