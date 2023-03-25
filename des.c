
#include "DES.H"

BYTE iv[8] = {0, 0, 0 , 0, 0, 0, 0, 0};

BYTE PC1[56] = {57, 49, 41, 33, 25, 17,  9,\
                 1, 58, 50, 42, 34, 26, 18,\
                10,  2, 59, 51, 43, 35, 27,\
                19, 11,  3, 60, 52, 44, 36,\
                63, 55, 47, 39, 31, 23, 15,\
                 7, 62, 54, 46, 38, 30, 22,\
                14,  6, 61, 53, 45, 37, 29,\
                21, 13,  5, 28, 20, 12,  4};

BYTE PC2[48] = {14, 17, 11, 24,  1,  5,\
                 3, 28, 15,  6, 21, 10,\
                23, 19, 12,  4, 26,  8,\
                16,  7, 27, 20, 13,  2,\
                41, 52, 31, 37, 47, 55,\
                30, 40, 51, 45, 33, 48,\
                44, 49, 39, 56, 34, 53,\
                46, 42, 50, 36, 29, 32};                          

BYTE IP[64] = {58, 50, 42, 34, 26, 18, 10,  2,\
               60, 52, 44, 36, 28, 20, 12,  4,\
               62, 54, 46, 38, 30, 22, 14,  6,\
               64, 56, 48, 40, 32, 24, 16,  8,\
               57, 49, 41, 33, 25, 17,  9,  1,\
               59, 51, 43, 35, 27, 19, 11,  3,\
               61, 53, 45, 37, 29, 21, 13,  5,\
               63, 55, 47, 39, 31, 23, 15,  7};
 
BYTE E[48] = {32,  1,  2,  3,  4,  5,\
               4,  5,  6,  7,  8,  9,\
               8,  9, 10, 11, 12, 13,\
              12, 13, 14, 15, 16, 17,\
              16, 17, 18, 19, 20, 21,\
              20, 21, 22, 23, 24, 25,\
              24, 25, 26, 27, 28, 29,\
              28, 29, 30, 31, 32,  1};

BYTE S1[64] = {14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,\
                0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,\
                4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,\
               15, 12, 8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13};
      
BYTE S2[64] = {15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,\
                3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,\
                0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,\
               13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9};

BYTE S3[64] = {10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,\
               13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,\
               13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,\
                1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12};

BYTE S4[64] = { 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,\
               13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,\
               10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,\
                3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14};

BYTE S5[64] = { 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,\
               14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,\
                4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,\
               11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3};

BYTE S6[64] = {12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,\
               10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,\
                9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,\
                4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13};
              
BYTE S7[64] = { 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,\
               13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,\
                1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,\
                6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12};

BYTE S8[64] = {13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,\
                1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,\
                7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,\
                2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11};

BYTE P[32] = {16,  7, 20, 21,\
              29, 12, 28, 17,\
               1, 15, 23, 26,\
               5, 18, 31, 10,\
               2,  8, 24, 14,\
              32, 27,  3,  9,\
              19, 13, 30,  6,\
              22, 11,  4, 25}; 

BYTE FP[64] = {40,  8, 48, 16, 56, 24, 64, 32,\
               39,  7, 47, 15, 55, 23, 63, 31,\
               38,  6, 46, 14, 54, 22, 62, 30,\
               37,  5, 45, 13, 53, 21, 61, 29,\
               36,  4, 44, 12, 52, 20, 60, 28,\
               35,  3, 43, 11, 51, 19, 59, 27,\
               34,  2, 42, 10, 50, 18, 58, 26,\
               33,  1, 41,  9, 49, 17, 57, 25};

                     
static void ExpandByte(BYTE byte, BYTE bit[8])
{
 bit[7] = (BYTE)((byte & 0x01) ? 1 : 0);
 bit[6] = (BYTE)((byte & 0x02) ? 1 : 0);
 bit[5] = (BYTE)((byte & 0x04) ? 1 : 0);
 bit[4] = (BYTE)((byte & 0x08) ? 1 : 0);
 bit[3] = (BYTE)((byte & 0x10) ? 1 : 0);
 bit[2] = (BYTE)((byte & 0x20) ? 1 : 0);
 bit[1] = (BYTE)((byte & 0x40) ? 1 : 0);
 bit[0] = (BYTE)((byte & 0x80) ? 1 : 0);
 return;
}

static BYTE CompressBit(BYTE bit[8])
{
 BYTE byte = 0x00;
 if (bit[0]) byte |= 0x80;
 if (bit[1]) byte |= 0x40;
 if (bit[2]) byte |= 0x20;
 if (bit[3]) byte |= 0x10;
 if (bit[4]) byte |= 0x08;
 if (bit[5]) byte |= 0x04;
 if (bit[6]) byte |= 0x02;
 if (bit[7]) byte |= 0x01;
 return byte;
}

                
BYTE DES_key[16][6]; // chiavi derivate




void ComputeKey(BYTE Key[8])
{
 BYTE i,k;
 BYTE t0,t1;
 BYTE temp_0[64];
 BYTE temp_1[64];
 
 for (i=0; i<8; i++)
    ExpandByte(Key[i],&(temp_0[i*8])); // chiave originale

 for (i=0; i<56; i++) 
    temp_1[i] = temp_0[PC1[i]-1]; // permutazione selettiva della chiave
 
 // memcpy(temp_0, temp_1, 56);
 for (i=0; i<56; i++)
    temp_0[i] = temp_1[i];
        
 for (k=0; k<16; k++) // ciclo di generazione delle 16 chiavi derivate
    {
     switch (k)
           {
            case 0:
            case 1:
            case 8:
            case 15: // 1 left shift
                     // C
                     t0 = temp_0[0];
                     for (i=1; i<27; i++)
                        temp_0[i-1] = temp_0[i];
                     temp_0[27] = t0;
                     // D
                     t0 = temp_0[28];
                     for (i=29; i<55; i++)
                        temp_0[i-1] = temp_0[i];
                     temp_0[55] = t0;   
                     break;
            default: // 2 left shifts
                     // C
                     t0 = temp_0[0];
                     t1 = temp_0[1];
                     for (i=2; i<27; i++)
                        temp_0[i-2] = temp_0[i];
                     temp_0[26] = t0;
                     temp_0[27] = t1;
                     // D
                     t0 = temp_0[28];
                     t1 = temp_0[29];
                     for (i=30; i<55; i++)
                        temp_0[i-2] = temp_0[i];
                     temp_0[54] = t0;
                     temp_0[55] = t1;  
                     break;
           }

     for (i=0; i<48; i++)
        temp_1[i] = temp_0[PC2[i]-1]; // permutazione selettiva
     for (i=0; i<6; i++)
        {
         t0 = CompressBit(&(temp_1[i*8])); // k-esima chiave derivata   
         DES_key[k][i] = t0;
        } 
    }   

 return;
}



void Block(BYTE src[8], BYTE dst[8], BYTE chain[8], BYTE fun)
{
 BYTE i,k,b,r,c;
 BYTE t;
 BYTE temp_0[64];
 BYTE temp_1[64];
 BYTE temp[64];
 BYTE key_cycle;
 
 if (fun==CODE)
   for (i=0; i<8; i++)
      src[i] ^= chain[i];
    
 for (i=0; i<8; i++)
    ExpandByte(src[i],&(temp_0[i*8]));

 for (i=0; i<64; i++) 
    temp_1[i] = temp_0[IP[i]-1]; // permutazione iniziale
 
 // memcpy(temp_0, temp_1, 64);
 for (i=0; i<64; i++)
    temp_0[i] = temp_1[i];
 
 key_cycle = 1;
 if (fun==CODE)
   k = 0;
 else // DECODE
     k = 16;
       
 while (key_cycle) // ciclo di scansione delle 16 chiavi derivate
    {
     for (i=0; i<48; i++) 
        temp_1[i] = temp_0[E[i]-1+32]; // espansione di R
    
     for (i=0; i<6; i++)
        temp[i] = CompressBit(&(temp_1[i*8])); 
     for (i=0; i<6; i++)
        {
         if (fun==CODE)
           t = (BYTE)(DES_key[k][i] ^ temp[i]);
         else // fun==DECODE
             t = (BYTE)(DES_key[k-1][i] ^ temp[i]);
             
         ExpandByte(t,&(temp_1[i*8])); // OR esclusivo con k-esima chiave derivata
        } 
    
     for (b=0; b<8; b++)
        {
         r = (BYTE)(temp_1[b*6+0]*2 + temp_1[b*6+5]*1);
         c = (BYTE)(temp_1[b*6+1]*8 + temp_1[b*6+2]*4 + temp_1[b*6+3]*2 + temp_1[b*6+4]*1);


         switch (b)
               {
                /* computazione della funzione definita dalla S-box relativa al blocco b */
                case 0:
                        temp[b*4]   =  (BYTE)((S1[r*16+c] & 0x08) ? 1 : 0);
                        temp[b*4+1] =  (BYTE)((S1[r*16+c] & 0x04) ? 1 : 0);
                        temp[b*4+2] =  (BYTE)((S1[r*16+c] & 0x02) ? 1 : 0);
                        temp[b*4+3] =  (BYTE)((S1[r*16+c] & 0x01) ? 1 : 0);
                        break;
                case 1: temp[b*4]   =  (BYTE)((S2[r*16+c] & 0x08) ? 1 : 0);
                        temp[b*4+1] =  (BYTE)((S2[r*16+c] & 0x04) ? 1 : 0);
                        temp[b*4+2] =  (BYTE)((S2[r*16+c] & 0x02) ? 1 : 0);
                        temp[b*4+3] =  (BYTE)((S2[r*16+c] & 0x01) ? 1 : 0);              
                        break;             
                case 2: temp[b*4]   =  (BYTE)((S3[r*16+c] & 0x08) ? 1 : 0);
                        temp[b*4+1] =  (BYTE)((S3[r*16+c] & 0x04) ? 1 : 0);
                        temp[b*4+2] =  (BYTE)((S3[r*16+c] & 0x02) ? 1 : 0);
                        temp[b*4+3] =  (BYTE)((S3[r*16+c] & 0x01) ? 1 : 0);              
                        break;             
                case 3: temp[b*4]   =  (BYTE)((S4[r*16+c] & 0x08) ? 1 : 0);
                        temp[b*4+1] =  (BYTE)((S4[r*16+c] & 0x04) ? 1 : 0);
                        temp[b*4+2] =  (BYTE)((S4[r*16+c] & 0x02) ? 1 : 0);
                        temp[b*4+3] =  (BYTE)((S4[r*16+c] & 0x01) ? 1 : 0);               
                        break;             
                case 4: temp[b*4]   =  (BYTE)((S5[r*16+c] & 0x08) ? 1 : 0);
                        temp[b*4+1] =  (BYTE)((S5[r*16+c] & 0x04) ? 1 : 0);
                        temp[b*4+2] =  (BYTE)((S5[r*16+c] & 0x02) ? 1 : 0);
                        temp[b*4+3] =  (BYTE)((S5[r*16+c] & 0x01) ? 1 : 0);                
                        break;             
                case 5: temp[b*4]   =  (BYTE)((S6[r*16+c] & 0x08) ? 1 : 0);
                        temp[b*4+1] =  (BYTE)((S6[r*16+c] & 0x04) ? 1 : 0);
                        temp[b*4+2] =  (BYTE)((S6[r*16+c] & 0x02) ? 1 : 0);
                        temp[b*4+3] =  (BYTE)((S6[r*16+c] & 0x01) ? 1 : 0);               
                        break;             
                case 6: temp[b*4]   =  (BYTE)((S7[r*16+c] & 0x08) ? 1 : 0);
                        temp[b*4+1] =  (BYTE)((S7[r*16+c] & 0x04) ? 1 : 0);
                        temp[b*4+2] =  (BYTE)((S7[r*16+c] & 0x02) ? 1 : 0);
                        temp[b*4+3] =  (BYTE)((S7[r*16+c] & 0x01) ? 1 : 0);               
                        break;             
                case 7: temp[b*4]   =  (BYTE)((S8[r*16+c] & 0x08) ? 1 : 0);
                        temp[b*4+1] =  (BYTE)((S8[r*16+c] & 0x04) ? 1 : 0);
                        temp[b*4+2] =  (BYTE)((S8[r*16+c] & 0x02) ? 1 : 0);
                        temp[b*4+3] =  (BYTE)((S8[r*16+c] & 0x01) ? 1 : 0);               
                        break;             
               }          

        }
     
     for (i=0; i<32; i++) 
        temp_1[i] = (BYTE)((temp[P[i]-1]) ^ temp_0[i]); // permutazione e OR esclusivo con L
 
     for (i=0; i<32; i++)
        {
         temp_0[i] = temp_0[i+32];
         temp_0[i+32] = temp_1[i];
        }
     
     if (fun==CODE)
       {
        k++;
        if (k==16)
          key_cycle = 0;
       }
     else // fun==DECODE
         {
          k--;
          if (k==0)
            key_cycle = 0;
         }         
         
    } // fine ciclo scansione chiavi derivate
           
 for (i=0; i<32; i++)
    {
     // inversione ordine (LR -> RL)
     temp_1[i+32] = temp_0[i];
     temp_1[i] = temp_0[i+32];
    }  
    
 for (i=0; i<64; i++) 
    temp_0[i] = temp_1[FP[i]-1]; // permutazione finale
        
 if (fun==CODE)
   for (i=0; i<8; i++)
      dst[i] = CompressBit(&(temp_0[i*8]));
 else // fun==DECODE    
     for (i=0; i<8; i++)
        dst[i] = (BYTE)((CompressBit(&(temp_0[i*8]))) ^ chain[i]);
        
 return;
}


void DES_encrypt(BYTE in[8], BYTE out[8], BYTE pswd[8])
{
 ComputeKey(pswd);
 Block(in,out,iv,CODE);
 return;
}

void DES_decrypt(BYTE in[8], BYTE out[8], BYTE pswd[8])
{
 ComputeKey(pswd);
 Block(in,out,iv,DECODE);
 return;
}
