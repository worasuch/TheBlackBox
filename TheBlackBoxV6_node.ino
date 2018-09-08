/*
   Class project
   Project name: The Black Box

   rfid ID-20LA
   NodeMCU V.1
*/
//Fire base
#include <FirebaseArduino.h>

#define FIREBASE_HOST "theblackbox-e956e.firebaseio.com" //Database URL
#define FIREBASE_AUTH "nUK1GBcOb7bNY5yzhG191AJwNrT4TDODSn6UXQlS" // Firebase Secret
const String TimeBB = "TimeBB";
const String data = "/data";

//Netpie variable
#include <ESP8266WiFi.h>
#include <MicroGear.h>

#include <SoftwareSerial.h>
SoftwareSerial chat(D5, D6); // RX, TX

const char* ssid      = "iPhone/Happy";       //WiFi ssid name
const char* password  = "seams7373";          //password WiFi

#define APPID   "TheBlackBox"
#define KEY     "SzQ4HHND2ycUreb"
#define SECRET  "GvLCYNOnVaN3xVaJBaz5IHs3v"

#define ALIAS   "pieled"
#define FREEBORADALIAS  "testledstatus"

WiFiClient client;

char state = 0;
char stateOutdated = 0;
char buff[16];
char dataTag[] = {};

MicroGear microgear(client);

//variable
byte tagID[12];         //check the data and checksum for ID card
boolean tagread = false;
int manCount;           //the numbers of the man in system
int oldManCount;        //pervious numbers of the man in system
int ledStatus = 1;      // (Active Low)
int temp;
int angle = 0;
char tagg[12];
String studentName;
String dateTime;

//ID card
const char card_001[10] = {'5', '5', '0', '0', '4', 'C', 'A', '6', '8', '4'}; //Tag ID: 55004CA684, Name: FIBO_OO1 Mo
const char card_002[10] = {0x00, 0x09, 0x00, 0x00, 0x08, 0x09, 0x09, 0x0B, 0x0A, 0x0C}; //Tag ID: **********, Name: FIBO_OO2 Chok
const char card_003[10] = {'5', '5', '0', '0', '4', 'C', 'B', '2', '6', '4'}; //Tag ID: 55004CB264, Name: FIBO_OO3 Benz
const char card_004[10] = {'5', '5', '0', '0', '4', 'D', '1', '8', '7', 'A'}; //Tag ID: 55004D187A, Name: FIBO_OO4 Hmee
const char card_005[10] = {'5', '5', '0', '0', '4', 'C', 'E', '1', '2', 'E'}; //Tag ID: 55004CE12E, Name: FIBO_OO5 Earth
const char card_006[10] = {'0', '0', '0', '0', '0', '0', '0', '0', 'F', 'F'}; //Tag ID: 55004CE12E, Name: FIBO_OO6
const char card_007[10] = {'4', 'E', '0', '0', '6', 'C', '8', 'C', '3', 'A'}; //Tag ID: 4E006C8C3A, Name: FIBO_007 Son
const char card_008[10] = {'5', '5', '0', '0', '4', 'C', 'C', 'C', 'B', 'B'}; //Tag ID: 55004CCCBB, Name: FIBO_008 Hap
const char card_009[10] = {'4', '9', '0', '0', '7', '8', '8', 'D', '3', 'A'}; //Tag ID: 4900788D3A, Name: FIBO_011 Ton
const char card_010[10] = {'4', 'E', '0', '0', '6', 'C', '9', '8', '8', '5'}; //Tag ID: 4E006C9885, Name: FIBO_010 Arm
const char card_011[10] = {0x00, 0x0D, 0x00, 0x00, 0x04, 0x02, 0x09, 0x05, 0x0C, 0x09}; //Tag ID: **********, Name: FIBO_011 Jake
const char card_013[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //Tag ID: 0000000000, Name: NoOne


/*const byte card_001[10] = {0x00, 0x09, 0x00, 0x00, 0x08, 0x09, 0x0A, 0x0E, 0x03, 0x07}; //Tag ID: 090089AE37, Name: FIBO_OO1
  const byte card_002[10] = {0x00, 0x09, 0x00, 0x00, 0x08, 0x09, 0x09, 0x0B, 0x0A, 0x0C}; //Tag ID: 0900899BAC, Name: FIBO_OO2
  const byte card_003[10] = {0x00, 0x09, 0x00, 0x00, 0x08, 0x09, 0x0B, 0x07, 0x0D, 0x05}; //Tag ID: 090089B7D5, Name: FIBO_OO3
  const byte card_004[10] = {0x00, 0x09, 0x00, 0x00, 0x08, 0x09, 0x0D, 0x00, 0x06, 0x02}; //Tag ID: 090089D062, Name: FIBO_OO4
  const byte card_005[10] = {0x00, 0x09, 0x00, 0x00, 0x08, 0x09, 0x0A, 0x0B, 0x0F, 0x07}; //Tag ID: 090089ABF7, Name: FIBO_OO5
  const byte card_006[10] = {0x00, 0x09, 0x00, 0x00, 0x08, 0x09, 0x0C, 0x04, 0x0E, 0x0F}; //Tag ID: 090089C4EF, Name: FIBO_006
  const byte card_007[10] = {0x00, 0x0D, 0x00, 0x00, 0x04, 0x02, 0x07, 0x0C, 0x03, 0x0D}; //Tag ID: 0D00427C3D, Name: FIBO_007
  const byte card_008[10] = {0x00, 0x09, 0x00, 0x00, 0x08, 0x09, 0x0D, 0x05, 0x0E, 0x06}; //Tag ID: 090089D5E6, Name: FIBO_008
  const byte card_009[10] = {0x00, 0x09, 0x00, 0x00, 0x08, 0x09, 0x0C, 0x0F, 0x0C, 0x0C}; //Tag ID: 090089CFCC, Name: FIBO_009
  const byte card_010[10] = {0x00, 0x0D, 0x00, 0x00, 0x04, 0x02, 0x09, 0x05, 0x0C, 0x09}; //Tag ID: 0D004295C9, Name: FIBO_010
  const byte card_011[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //Tag ID: 0000000000, Name: NoOne*/

//State of ID card
char card_001_state = 0;
char card_002_state = 0;
char card_003_state = 0;
char card_004_state = 0;
char card_005_state = 0;
char card_006_state = 0;
char card_007_state = 0;
char card_008_state = 0;
char card_009_state = 0;
char card_010_state = 0;

void onConnected(char *attribute, uint8_t* msg, unsigned int msglen);
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen);
void updateIO();
void sendState();
void send_tag();



//Serial read 
// Keep track of current position in array
int counter = 0;
// Keep track of the last comma so we know where to start the substring
int lastIndex = 0;
// Define number of pieces
const int numberOfPieces = 4;
String pieces[numberOfPieces];


void setup()
{
  chat.begin(9600);
  Serial.begin(9600);   //send data to computer
  Serial.println("Starting...");

  //initiate WiFi
  if (WiFi.begin(ssid, password))
  {
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
  }


  microgear.on(MESSAGE, onMsghandler);
  microgear.on(CONNECTED, onConnected);
  //initial microgear variable
  microgear.init(KEY, SECRET, ALIAS);

  //initial NETPIE to AppID that define
  microgear.connect(APPID);         //funcetion connect NETPIE

  //initial Firebase
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);

}

void loop()
{
  microgear.loop();

  if (chat.available())                        //Make sure all the frame is received
  {
    tagread = true;
    String tag = chat.readString();
    
    for (int i = 0; i < tag.length(); i++) 
    {
        // Loop through each character and check if it's a comma
      if (tag.substring(i, i+1) == "|") 
      {
          // Grab the piece from the last index up to the current position and store it
          pieces[counter] = tag.substring(lastIndex, i);
          //Serial.println(pieces[counter]);
          // Update the last position and add 1, so it starts from the next character
          lastIndex = i + 1;
          // Increase the position in the array that we store into
          counter++;
          
      }

        // If we're at the end of the string (no more commas to stop us)
/*        if (i == tag.length() - 1) 
        {
          // Grab the last part of the string from the lastIndex to the end
          pieces[counter] = tag.substring(lastIndex, i);
          Serial.println(pieces[counter]);
        } */
    }
    Serial.println(pieces[0]);
    Serial.println(pieces[1]);
    Serial.println(pieces[2]);
    dateTime = pieces[0];
    studentName = pieces[1];
    manCount = pieces[2].toInt();   

    // Clear out string and counters to get ready for the next incoming string
    tag = "";
    counter = 0;
    lastIndex = 0;

  }

  if (tagread == true)                //New tag is read
  {
    /////////////fire base///////////////////////
    StaticJsonBuffer<400> jsonBuffer;
    JsonObject&  valueObject = jsonBuffer.createObject();
    valueObject["ID"] = "603407000xx";
    valueObject["Name"] = studentName;
    valueObject["Time"] = dateTime;
    valueObject["Status"] = manCount;
    Firebase.push(TimeBB + data , valueObject);
    
    microgear.publish("/dataTag", studentName, true);
    //send_tag();
    //compare_tag();                    //map tag ID with user name
    Serial.print("manCount: ");
    Serial.print(manCount);
    Serial.print("\r\n");
    microgear.publish("/manCount", manCount, true);

    String data = "{\"manCount\":";
    data += manCount ;
    data += "}";
    microgear.writeFeed("HappyIoTFeed", data);
    Serial.println("Write Feed manCount Success....");

    if (oldManCount == 1 && manCount == 2)                 //the numbers of man in system change from 4 to 5
    {
      ledStatus = 0;
      microgear.publish("/ledStatus/one", ledStatus, true);
      digitalWrite(LED_BUILTIN, LOW);
      Serial.print("Servo1: ON ");                         //Servo "ON"
      Serial.print("\r\n");
      tagread = false;
    }

    if (oldManCount == 2 && manCount == 1)                //the number of man in system change from 5 to 4
    {
      ledStatus = 1;
      microgear.publish("/ledStatus/one", ledStatus, true);
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.print("Servo1: OFF ");                        //Servo "OFF"
      Serial.print("\r\n");
      tagread = false;
    }

     if (oldManCount == 2 && manCount == 3)                 //the numbers of man in system change from 4 to 5
    {
      ledStatus = 0;
      microgear.publish("/ledStatus/two", ledStatus, true);
      digitalWrite(LED_BUILTIN, LOW);
      Serial.print("Servo2: ON ");                         //Servo "ON"
      Serial.print("\r\n");
      tagread = false;
    }

    if (oldManCount == 3 && manCount == 2)                //the number of man in system change from 5 to 4
    {
      ledStatus = 1;
      microgear.publish("/ledStatus/two", ledStatus, true);
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.print("Servo2: OFF ");                        //Servo "OFF"
      Serial.print("\r\n");
      tagread = false;
    }

    
     if (oldManCount == 3 && manCount == 4)                 //the numbers of man in system change from 4 to 5
    {
      ledStatus = 0;
      microgear.publish("/ledStatus/three", ledStatus, true);
      digitalWrite(LED_BUILTIN, LOW);
      Serial.print("Servo3: ON ");                         //Servo "ON"
      Serial.print("\r\n");
      tagread = false;
    }

    if (oldManCount == 4 && manCount == 3)                //the number of man in system change from 5 to 4
    {
      ledStatus = 1;
      microgear.publish("/ledStatus/three", ledStatus, true);
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.print("Servo3: OFF ");                        //Servo "OFF"
      Serial.print("\r\n");
      tagread = false;
    }
    oldManCount = manCount;
    tagread = false;
  }
  delay(10);
}


void send_tag()                                          //Subroutine to display the tagID content
{
  String dataID = "{\"dataTag\":";
  for (int index = 0; index < 10; index++)
  {
    switch (tagID[index])
    {
      case 10:
        dataID += 'A';
        break;
      case 11:
        dataID += 'B';
        break;
      case 12:
        dataID += 'C';
        break;
      case 13:
        dataID += 'D';
        break;
      case 14:
        dataID += 'E';
        break;
      case 15:
        dataID += 'F';
        break;
      default:
        dataID += tagID[index];
    }
  }
  dataID += "}";
  microgear.publish("/dataTag", dataID);
  Serial.print("dataID HEX");
  Serial.print(dataID);

  String dataIDdec = "{\"dataTagDec\":";
  for (int index = 0; index < 10; index++)
  {
    switch (tagID[index])
    {
      case 'A':
        dataIDdec += '10';
        break;
      case 'B':
        dataIDdec += '11';
        break;
      case 'C':
        dataIDdec += '12';
        break;
      case 'D':
        dataIDdec += '13';
        break;
      case 'E':
        dataIDdec += '14';
        break;
      case 'F':
        dataIDdec += '15';
        break;
      default:
        dataIDdec += tagID[index];
    }
  }
  dataIDdec += "}";
  microgear.writeFeed("HappyIoTFeed", dataIDdec);
  Serial.println("Write Feed dataTag Success....");
  Serial.print("dataID DEC");
  Serial.print(dataIDdec);
}


void print_tag()                      //Subroutine to display the tagID content
{
  Serial.print("Tag ID: ");
  for (int index = 0; index < 10; index++)
  {
    Serial.print(tagID[index], HEX);    //Display tagID
  }
  Serial.print("\r\nChecksum: ");
  Serial.print(tagID[10], HEX);         //Display checksum
  Serial.println(tagID[11]), HEX;
}

void clear_tag()                      //Subroutine to clear the tagID content
{
  for (int index = 0; index < sizeof(tagID); index++)
  {
    tagID[index] = 0;
  }
}

void compare_tag()
{
  int card_001_temp = 0;
  int card_002_temp = 0;
  int card_003_temp = 0;
  int card_004_temp = 0;
  int card_005_temp = 0;
  int card_006_temp = 0;
  int card_007_temp = 0;
  int card_008_temp = 0;
  int card_009_temp = 0;
  int card_010_temp = 0;
  int card_011_temp = 0;


  for (int index = 0; index < 10; index++)                //check 10 byte of data
  {
    if (tagg[index] == card_001[index])card_001_temp++;
    if (tagg[index] == card_002[index])card_002_temp++;
    if (tagg[index] == card_003[index])card_003_temp++;
    if (tagg[index] == card_004[index])card_004_temp++;
    if (tagg[index] == card_005[index])card_005_temp++;
    if (tagg[index] == card_006[index])card_006_temp++;
    if (tagg[index] == card_007[index])card_007_temp++;
    if (tagg[index] == card_008[index])card_008_temp++;
    if (tagg[index] == card_009[index])card_009_temp++;
    if (tagg[index] == card_010[index])card_010_temp++;
    if (tagg[index] == card_011[index])card_011_temp++;
  }

  if (card_001_temp == 10 )                                // ID card FIBO_001
  {
    studentName = "Chawakorn Sri-ngernyuang";
    microgear.publish("/dataTag", studentName, true);
    if (card_001_state == 0)
    {
      manCount++;
      Serial.print("Welcome: FIBO_OO1\r\n");
      card_001_state = 1;
    }
    else
    {
      manCount--;
      Serial.print("Bye: FIBO_OO1\r\n");
      card_001_state = 0;
    }
  }

  if (card_002_temp == 10 )                             // ID card FIBO_002
  {
    studentName = "Chokchai Pengyasa";
    microgear.publish("/dataTag", studentName, true);
    if (card_002_state == 0)
    {
      manCount++;
      Serial.print("Welcome: FIBO_OO2\r\n");
      card_002_state = 1;
    }
    else
    {
      manCount--;
      Serial.print("Bye: FIBO_OO2\r\n");
      card_002_state = 0;
    }
  }

  if (card_003_temp == 10 )                             // ID card FIBO_003
  {
    studentName = "Kampanat Chailoet";
    microgear.publish("/dataTag", studentName, true);
    if (card_003_state == 0)
    {
      manCount++;
      Serial.print("Welcome: FIBO_OO3\r\n");
      card_003_state = 1;
    }
    else
    {
      manCount--;
      Serial.print("Bye: FIBO_OO3\r\n");
      card_003_state = 0;
    }
  }

  if (card_004_temp == 10 )                             // ID card FIBO_004
  {
    studentName = "Kraingkrai Kanjana-ekkarin";
    microgear.publish("/dataTag", studentName, true);
    if (card_004_state == 0)
    {
      manCount++;
      Serial.print("Welcome: FIBO_OO4\r\n");
      card_004_state = 1;
    }
    else
    {
      manCount--;
      Serial.print("Bye: FIBO_OO4\r\n");
      card_004_state = 0;
    }
  }

  if (card_005_temp == 10 )                             // ID card FIBO_005
  {
    studentName = "Chawankorn Tiemprasert";
    microgear.publish("/dataTag", studentName, true);
    if (card_005_state == 0)
    {
      manCount++;
      Serial.print("Welcome: FIBO_OO5\r\n");
      card_005_state = 1;
    }
    else
    {
      manCount--;
      Serial.print("Bye: FIBO_OO5\r\n");
      card_005_state = 0;
    }
  }

  if (card_006_temp == 10 )                             // ID card FIBO_006
  {
    studentName = "Ratchanon Phaetpricha";
    microgear.publish("/dataTag", studentName, true);
    if (card_006_state == 0)
    {
      manCount++;
      Serial.print("Welcome: FIBO_OO6\r\n");
      card_006_state = 1;
    }
    else
    {
      manCount--;
      Serial.print("Bye: FIBO_OO6\r\n");
      card_006_state = 0;
    }
  }

  if (card_007_temp == 10 )                             // ID card FIBO_007
  {
    studentName = "Sanan Jantanalach";
    microgear.publish("/dataTag", studentName, true);
    if (card_007_state == 0)
    {
      manCount++;
      Serial.print("Welcome: FIBO_OO7\r\n");
      card_007_state = 1;
    }
    else
    {
      manCount--;
      Serial.print("Bye: FIBO_OO7\r\n");
      card_007_state = 0;
    }
  }

  if (card_008_temp == 10 )                             // ID card FIBO_008
  {
    studentName = "Worasuchad Haomachai";
    microgear.publish("/dataTag", studentName, true);
    if (card_008_state == 0)
    {
      manCount++;
      Serial.print("Welcome: FIBO_OO8\r\n");
      card_008_state = 1;
    }
    else
    {
      manCount--;
      Serial.print("Bye: FIBO_OO8\r\n");
      card_008_state = 0;
    }
  }

  if (card_009_temp == 10 )                             // ID card FIBO_009
  {
    studentName = "Kittisak Jamseeda";
    microgear.publish("/dataTag", studentName, true);
    if (card_009_state == 0)
    {
      manCount++;
      Serial.print("Welcome: FIBO_OO9\r\n");
      card_009_state = 1;
    }
    else
    {
      manCount--;
      Serial.print("Bye: FIBO_OO9\r\n");
      card_009_state = 0;
    }
  }

  if (card_010_temp == 10 )                              // ID card FIBO_010
  {
    studentName = "Podsawat Warakuldumrongdej";
    microgear.publish("/dataTag", studentName, true);
    if (card_010_state == 0)
    {
      manCount++;
      Serial.print("Welcome: FIBO_O10\r\n");
      card_010_state = 1;
    }
    else
    {
      manCount--;
      Serial.print("Bye: FIBO_O10\r\n");
      card_010_state = 0;
    }
  }

  if (card_011_temp == 10)
  { 
    Serial.print("Name: No One\r\n");
  }

  if (card_001_temp != 10 && card_002_temp != 10 && card_003_temp != 10 && card_004_temp != 10 && card_005_temp != 10
      && card_006_temp != 10 && card_007_temp != 10 && card_008_temp != 10 && card_009_temp != 10 && card_010_temp != 10 && card_011_temp != 10)
  {
    Serial.print("Name: Unknow Subscriber\r\n");
    studentName = "Unknow Subscriber";
    microgear.publish("/dataTag", studentName, true);
  }
}

void sendState()
{
  if (state == 0)
    microgear.publish("/pieled/state", "0");
  else
    microgear.publish("/pieled/state", "1");
  Serial.println("send state..");
  stateOutdated = 0;
}

void updateIO()
{
  if (state >= 1)
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
  else
  {
    state = 0;
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

//function is called when the messages incoming
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) 
{
  char m = *(char *)msg;
  
  Serial.print("Incoming message --> ");
  msg[msglen] = '\0';
  Serial.println((char *)msg);

  //message is 1 switchon LED if 2 switchoff LED
/*  if (*(char *)msg == '1')
  {
    digitalWrite(LED_BUILTIN, LOW);     // LED switchon
  }
  else
  {
    digitalWrite(LED_BUILTIN, HIGH);    // LED switchoff
  } */
    if(m == '0' || m == '1')
    {
      state = m =='0'?0:1;
      updateIO();
    }
    if(m == '0' || m == '1' || m == '?')
      stateOutdated = 1;
}

//function is called when microgear connect to NETPIE successful
void onConnected(char *attribute, uint8_t* msg, unsigned int msglen)
{
  Serial.println("Connected to NEtPIE....");
  //we may be modify name or do something on this opportunity e.g. subscribe

  microgear.setAlias(ALIAS);
  stateOutdated = 1;
}
