                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          #line 1 "remote_node.ino"
#include <SPI.h>
#include <SD.h>
#include "Arduino.h"

File remote;
File prvate;

                  
unsigned int fileinfo;

void setup() 
{
  #define MOSI 11
  #define MISO 12
  #define IRQ 4
  #define CLK 13
  #define CSsd 8
  #define CSrf 7
  #define SCN 6
 
  pinMode(CSsd, OUTPUT);
  pinMode(CSrf, OUTPUT);

  Serial.begin(9600);  // 9600 memes per meme
  
  if (!SD.begin(4)) {
    Serial.println("SD failed!");
    return;
  }
  Serial.println("SD connected!");

  if (!SD.exists("remote_data.txt")) {
    Serial.println("files missing.");
  }
  remote = SD.open("remote_data.txt", FILE_READ);
  fileinfo = remote.available();
}

struct packet                  
{
  byte adress;
  unsigned int partno;
  unsigned int fileinfo;
  byte data[8];
  byte checksum;
  byte key;
};

int crcgen(byte crcdata[8], byte key)  // key must be 4 bits or less                                                                                                                
{
  unsigned long crc[2] = {0,0};
  byte chksm[2] = {0,0};
  unsigned long process=0;
  byte i;
  byte j;
  for (i = 0; i < 4; i++)                                       
  {
    crc[0] |= crcdata[i];                                
    crc[0] = crc[0] << 8;                               
    crc[1] |= crcdata[(i+4)];                                 
    crc[1] = crc[1] << 8;
  }
  for (i = 0; i < 2; i++)
  {
    process = key;
    process = process << 23;
    while(process < (1<<31)) process = process << 1;
    for (j = 0; j < 32; j++)
    {
      
      if (crc[i] < (1<<31)) crc[i] = crc[i] << 1;
      else
      {
        crc[i] = crc[i] ^ process;
        crc[i] = crc[i] << 1;
      }
      
    }
    
  }
  
  //return (chksm[0]<<4)|(constrain(chksm[1], 0 , 15));
}

/*struct packet                  
{
  byte adress;
  unsigned int partno;
  unsigned int fileinfo;
  byte data[8];
  byte checksum;
  byte key;
};*/

void loop() 
{
  packet post[8];
  unsigned int filesize = remote.available();
  for(int k = 0; k < 8; k++){
    post[k] = {0, 0, 0, 0, 0, random(16)};
    post[k].fileinfo = filesize;
    post[k].partno = (filesize - remote.available());
    for(int l = 0; l < 8; l++)
    {
      post[k].data[l]= remote.read();
    }
    post[k].checksum = crcgen(post[k].data, post[k].key);
    
  }
  //radio.send()
  
  
}


