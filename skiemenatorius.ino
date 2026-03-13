/*
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read data from a PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 * 
 * Example sketch/program showing how to read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
 * Reader on the Arduino SPI interface.
 * 
 * When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
 * then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
 * you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
 * will show the ID/UID, type and any data blocks it can read. Note: you may see "Timeout in communication" messages
 * when removing the PICC from reading distance too early.
 * 
 * If your reader supports it, this sketch/program will read all the PICCs presented (that is: multiple tag reading).
 * So if you stack two or more PICCs on top of each other and present them to the reader, it will first output all
 * details of the first and then the next PICC. Note that this may take some time as all data blocks are dumped, so
 * keep the PICCs at reading distance until complete.
 * 
 * @license Released into the public domain.
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 * More pin layouts for other boards can be found here: https://github.com/miguelbalboa/rfid#pin-layout
 */

#include <SPI.h>
#include <MFRC522.h>


#define DEBUG true

#define RST_PIN         21          // Configurable, see typical pin layout above
#define SS_PIN          5         // Configurable, see typical pin layout above
#define RST_PIN2         2          // Configurable, see typical pin layout above
#define SS_PIN2          15         // Configurable, see typical pin layout above
#define RXD2 14
#define TXD2 12

#define BTN_PIN 13


MFRC522 mfrc522_1(SS_PIN, RST_PIN);  // Create MFRC522 instance
MFRC522 mfrc522_2(SS_PIN2, RST_PIN2);  // Create MFRC522 instance



byte buff[64+2];
byte sizeOfBuff = sizeof(buff);
byte *sizeBuff = &sizeOfBuff;
byte quadPagesToRead = 4;
byte numberOfSongs[] = {0xAA,0x0C,0x00,0xB6};
byte currSong[] = {0xAA,0x0D,0x00,0xB7};
//byte play[] = {0xAA,0x07,0x02,0x00,0x05,0xB9};
byte vol100[] = {0xAA,0x13,0x01,0xFF,0xBD};
int i=0;
uint8_t nrOfSongs=0;
//uint8_t raides[255];
//uint16_t lut[]={0, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 0, 0, 0, 14, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 17, 0, 0, 0, 18, 0, 0, 0, 19, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//uint16_t RaidLut[33][33]={{0,1,2,3,4,5,6,7,0,8,9,10,11,12,0,0,13,14,15,16,17,18,19,20,21,22,23,25,0,0,26,27,28},{1,0,0,29,30,31,32,33,0,34,35,36,37,38,0,0,39,40,41,42,43,44,45,46,47,48,49,50,0,0,51,52,53},{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{3,54,0,0,55,56,57,58,0,59,60,61,62,63,0,0,64,65,66,67,68,69,70,71,72,73,74,75,0,0,76,77,78},{4,79,0,80,0,81,82,83,0,84,85,86,87,88,0,0,89,90,91,92,93,94,95,96,97,98,99,100,0,0,101,102,103},{5,104,0,105,106,0,107,108,0,109,110,111,112,113,0,0,114,115,116,117,118,119,120,121,122,123,124,125,0,0,126,127,128},{6,129,0,130,131,132,0,133,0,134,135,136,137,138,0,0,139,140,141,142,143,144,145,146,147,148,149,150,0,0,151,152,153},{7,154,0,155,156,157,158,0,0,159,160,161,162,163,0,0,164,165,166,167,168,169,170,171,172,173,174,175,0,0,176,177,178},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{8,179,0,180,181,182,183,184,0,0,185,186,187,188,0,0,189,190,191,192,193,194,195,196,197,198,199,200,0,0,201,202,203},{9,204,0,205,206,207,208,209,0,210,0,211,212,213,0,0,214,215,216,217,218,219,220,221,222,223,224,225,0,0,226,227,228},{10,229,0,230,231,232,233,234,0,235,236,0,237,238,0,0,239,240,241,242,243,244,245,246,247,248,249,250,0,0,251,252,253},{11,254,0,255,256,257,258,259,0,260,261,262,0,263,0,0,264,265,266,267,268,269,270,271,272,273,274,275,0,0,276,277,278},{12,279,0,280,281,282,283,284,0,285,286,287,288,0,0,0,289,290,291,292,293,294,295,296,297,298,299,300,0,0,301,302,303},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{13,304,0,305,306,307,308,309,0,310,311,312,313,314,0,0,0,315,316,317,318,319,320,321,322,323,324,325,0,0,326,327,328},{14,329,0,330,331,332,333,334,0,335,336,337,338,339,0,0,340,0,341,342,343,344,345,346,347,348,349,350,0,0,351,352,353},{15,354,0,355,356,357,358,359,0,360,361,362,363,364,0,0,365,366,0,367,368,369,370,371,372,373,374,375,0,0,376,377,378},{16,379,0,380,381,382,383,384,0,385,386,387,388,389,0,0,390,391,392,0,393,394,395,396,397,398,399,400,0,0,401,402,403},{17,404,0,405,406,407,408,409,0,410,411,412,413,414,0,0,415,416,417,418,0,419,420,421,422,423,424,425,0,0,426,427,428},{18,429,0,430,431,432,433,434,0,435,436,437,438,439,0,0,440,441,442,443,444,0,445,446,447,448,449,450,0,0,451,452,453},{19,454,0,455,456,457,458,459,0,460,461,462,463,464,0,0,465,466,467,468,469,470,0,471,472,473,474,475,0,0,476,477,478},{20,479,0,480,481,482,483,484,0,485,486,487,488,489,0,0,490,491,492,493,494,495,496,0,497,498,499,500,0,0,501,502,503},{21,504,0,505,506,507,508,509,0,510,511,512,513,514,0,0,515,516,517,518,519,520,521,522,0,523,524,525,0,0,526,527,528},{22,529,0,530,531,532,533,534,0,535,536,537,538,539,0,0,540,541,542,543,544,545,546,547,548,0,549,550,0,0,551,552,553},{23,554,0,555,556,557,558,559,0,560,561,562,563,564,0,0,565,566,567,568,569,570,571,572,573,574,0,575,0,0,576,577,578},{25,579,0,580,581,582,583,584,0,585,586,587,588,589,0,0,590,591,592,593,594,595,596,597,598,599,600,0,0,0,601,602,603},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{26,604,0,605,606,607,608,609,0,610,611,612,613,614,0,0,615,616,617,618,619,620,621,622,623,0,624,625,0,0,0,626,0},{27,627,0,0,0,0,0,628,0,629,0,0,0,630,0,0,631,0,632,633,634,635,636,637,0,0,0,638,0,0,639,0,640},{28,641,0,0,0,0,0,642,0,643,0,0,0,644,0,0,0,0,645,646,647,648,649,0,0,0,0,650,0,0,0,0,0}};
#include "RaidLut.h"



void play(uint16_t song)
{
  Serial.print("playing:");
  Serial.println(song);

  byte low = song & 0x00FF;
  byte high = song >> 8;
  byte sm = 0xAA+0x07+0x02+high+low&0xFF;
  Serial.print("sm:");
  Serial.println(sm);
  
  byte cmd[] = {0xAA,0x07,0x02,high,low,sm};
  Serial2.write(cmd, sizeof(cmd));
  delay(100);
}

void w8_4PlayFinish()
{
  Serial.print("wating..");
  byte buf[6];
  bool playing = true;
  byte cmd[] = {0xAA,0x01,0x00,0xAB}; //query play status cmd
  while(playing)
  {
    Serial2.write(cmd, sizeof(cmd));
    
    delay(250);
    memset(buf,0,6);
    i = 0;
    while(Serial2.available())
    {
      buff[i]= Serial2.read();
      Serial.print(buf[i],HEX);
      Serial.print(" ");
      i++;
    }
    Serial.println("");
    playing=buf[4];
  }
}

byte getValue(MFRC522 reader)
{
  // static byte letter = 0;
  // static uint8_t retries=0;
  static byte temp_buff[64+2];
  static byte buffSizeInt8;
  static byte *buffSize_p = &buffSizeInt8;
  memset(temp_buff,0,64+2);

  for(int retries=0; retries < 3; retries++)
  {
    // reader.PCD_Init();
    // delay(10);
    if( reader.PICC_IsNewCardPresent() )  // required
    {
      if(DEBUG){Serial.println("card found");}

      if( reader.PICC_ReadCardSerial() ) // seems not needed
      {
        if(DEBUG){Serial.println("card selected");}

        for(int i=0;i<(sizeof(temp_buff)/16);i++)
        {
          *buffSize_p = sizeof(temp_buff);
          reader.GetStatusCodeName(reader.MIFARE_Read(i*0x04, temp_buff+0x10*i, buffSize_p));
        }

        if(DEBUG){Serial.println((char)buff[25]);}

        return temp_buff[25];
      }
    }
    delay(10); // retry delay
    if(DEBUG){Serial.print("retries: ");Serial.println(retries);}
  }

  return 0; // return 0 if card read failed
}

void setup() {
  Serial.begin(115200);   // Initialize serial communications with the PC
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // Uart to Audio board

  // while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  mfrc522_1.PCD_Init();   // Init MFRC522
  delay(4);       // Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522_1.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  delay(4);
  mfrc522_2.PCD_Init();   // Init MFRC522
  delay(4);       // Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522_2.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  Serial2.write(vol100, sizeof(vol100));

  pinMode(BTN_PIN,INPUT_PULLUP);
 
}



char first,second;
uint16_t songNr=0;

void logic(char first, char second)
{
  const uint8_t offset = 32;
  Serial.print("letter codes: ");Serial.print(first);Serial.print(" ");Serial.println(second);
  // first-=offset;
  // second-=offset;
  if(first>0) first-=offset;
  if(second>0) second-=offset;

  if(first != 0 || second != 0)
  {
    Serial.print("letter values: ");Serial.print(first,DEC);Serial.print(" ");Serial.println(second,DEC);
    songNr= RaidLut[first][second];
    Serial.print("need to play: ");Serial.println(songNr);
    if(songNr > 0)
    {
      play(songNr);
    }
    delay(100);
  }
  else
  {
    delay(50);
  }
}

void loop() {
  if(!digitalRead(BTN_PIN))
  {

    first = getValue(mfrc522_1);
    second= getValue(mfrc522_2);
    
  //  for(int i=0; i< sizeof(buff);i+=4)
  //  {
  //    Serial.print(i/4,HEX);
  //    Serial.print(" ");
  //    Serial.print(buff[i],HEX);
  //    Serial.print(" ");
  //    Serial.print(buff[i+1],HEX);
  //    Serial.print(" ");
  //    Serial.print(buff[i+2],HEX);
  //    Serial.print(" ");
  //    Serial.print(buff[i+3],HEX);
  //    Serial.println("");
  //  }
      
    logic(first, second);
      // Serial.print("first: ");Serial.print(first);Serial.print(" second: ");Serial.println(second);
      
      // if(first != 0 && second != 0)
      // {
      // //  Serial.print("first: ");Serial.print(first);Serial.print(" second: ");Serial.println(second);
      //   Serial.print("letter codes: ");Serial.print(first-65);Serial.print(" ");Serial.println(second-65);
      //   songNr = lut[(first-65)*26+(second-65)];
      //   Serial.print("need to play: ");Serial.println(songNr);
      //   if(songNr > 20)
      //   {
      //     Serial.print("song: ");Serial.print(songNr);Serial.println(" not found");
      //     play(3);
      //   }
      //   else
      //   {
      //     Serial.print("playing: ");Serial.println(songNr);
      //     play(songNr);
      //   }
        
      //   delay(100);
        
//        Serial2.write(numberOfSongs, sizeof(numberOfSongs));delay(100);
//        memset(buff,0,66);
//        i = 0;
//        while(Serial2.available())
//        {
//          buff[i]= Serial2.read();
//          Serial.print(buff[i],HEX);
//          Serial.print(" ");
//          i++;
//        }
//        Serial.println("");
//        nrOfSongs=buff[4];
    
        
    //    play(1);
//        for(int i=1; i< nrOfSongs+1;i++)
//        {
//          play(i);
//    //      w8_4PlayFinish();
//          delay(2500);
//        }
    
//        Serial2.write(currSong, sizeof(currSong));delay(100);
//        
//        memset(buff,0,66);
//        i = 0;
//        while(Serial2.available())
//        {
//          buff[i]= Serial2.read();
//          Serial.print(buff[i],HEX);
//          Serial.print(" ");
//          i++;
//        }
//        Serial.println("");
  //     }
  }
  else
  {
    delay(50);
  }
}
