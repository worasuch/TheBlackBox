//////////////////////////////////////////////////////////////////
// project:   the black box                                     //
// code v.:   7.0                                               //  
// board  :   arduino mega 2560                                 //
// date   :   14/1/2018                                         //
// code by:   computer pro for robotics class                   //
////////////////////////////////////////////////////////////////// 
#include <SPI.h>

///////////////////////// Servo   ////////////////////////////////
//////////////////////////////////////////////////////////////////
#include <Servo.h>
Servo myservo1;
Servo myservo2;
Servo myservo3;
int pos;
//////////////////////////////////////////////////////////////////

///////////////////////// Finger   ///////////////////////////////
//////////////////////////////////////////////////////////////////
// --#include "FPS_GT511C3.h"
// --#include "SoftwareSerial.h"
// set up software serial pins for Arduino's w/ Atmega328P's
// FPS (TX) is connected to pin 4 (Arduino's Software RX)
// FPS (RX) is connected through a converter to pin 5 (Arduino's Software TX)
// --FPS_GT511C3 fps(50, 49); // (Arduino SS_RX = pin 4, Arduino SS_TX = pin 5)
//////////////////////////////////////////////////////////////////

///////////////////////// RTC   //////////////////////////////////
//////////////////////////////////////////////////////////////////
#include <Wire.h>
#include <RtcDS1307.h>
RtcDS1307<TwoWire> Rtc(Wire);
#define countof(a) (sizeof(a) / sizeof(a[0]))
char datestring[20];
//////////////////////////////////////////////////////////////////

///////////////////////  Variable ////////////////////////////////
//////////////////////////////////////////////////////////////////
byte tagID[12];
boolean tagread = false;
int manCount;           //the numbers of the man in system
int oldManCount;        //pervious numbers of the man in system
int ledStatus = 1;      //(Active Low)
int adminAcc = 0;       //admin

String studentName;
//////////////////////////////////////////////////////////////////

///////////////////////// ID Cart  ///////////////////////////////
//////////////////////////////////////////////////////////////////
//ID card
const char card_001[10] = {'5','5','0','0','4','C','A','6','8','4'}; //Tag ID: 55004CA684, Name: FIBO_OO1 Mo
const char card_002[10] = {0x00, 0x09, 0x00, 0x00, 0x08, 0x09, 0x09, 0x0B, 0x0A, 0x0C}; //Tag ID: **********, Name: FIBO_OO2 Chok
const char card_003[10] = {'5','5','0','0','4','C','B','2','6','4'}; //Tag ID: 55004CB264, Name: FIBO_OO3 Benz
const char card_004[10] = {'5','5','0','0','4','D','1','8','7','A'}; //Tag ID: 55004D187A, Name: FIBO_OO4 Hmee
const char card_005[10] = {'5','5','0','0','4','C','E','1','2','E'}; //Tag ID: 55004CE12E, Name: FIBO_OO5 Earth
const char card_006[10] = {'0','0','0','0','0','0','0','0','F','F'}; //Tag ID: 55004CE12E, Name: FIBO_OO6 
const char card_007[10] = {'4','E','0','0','6','C','8','C','3','A'}; //Tag ID: 4E006C8C3A, Name: FIBO_007 Son
const char card_008[10] = {'5','5','0','0','4','C','C','C','B','B'}; //Tag ID: 55004CCCBB, Name: FIBO_008 Hap
const char card_009[10] = {'4','9','0','0','7','8','8','D','3','A'}; //Tag ID: 4900788D3A, Name: FIBO_011 Ton 
const char card_010[10] = {'4','E','0','0','6','C','9','8','8','5'}; //Tag ID: 4E006C9885, Name: FIBO_010 Arm
const char card_011[10] = {0x00, 0x0D, 0x00, 0x00, 0x04, 0x02, 0x09, 0x05, 0x0C, 0x09}; //Tag ID: **********, Name: FIBO_011 Jake
const char card_013[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //Tag ID: 0000000000, Name: NoOne

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
//////////////////////////////////////////////////////////////////

///////////////////////// Keypad & LCD  //////////////////////////
//////////////////////////////////////////////////////////////////
#include <U8glib.h>
#include <Keypad.h>
U8GLIB_ST7920_128X64_1X u8g(38, 39, 40, 41, 42, 43, 44, 45, 31, 33, 35, 37);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
//U8GLIB_ST7920_128X64_1X u8g(52, 51, 53);  // SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
uint32_t  timeStart;
uint8_t   timeHours;
uint8_t   timeMinutes;
uint8_t   timeSeconds;

uint32_t  updatePressedTimer;
uint32_t  displayStandbyTimer;
uint32_t  displayMainTimer;
uint32_t  displayModeTimer;
uint32_t  displayMode1Timer;
uint32_t  displayMode2Timer;
uint32_t  counterTimer;
uint32_t  lookForcardsTimer;
uint32_t  verifyCardsTimer;
uint32_t  displaySuccessfulTimer;
uint32_t  displayFailTimer;

int activeStandby = 1;
int activeMain = 0;
int activeMode = 0;
int activeMode1 = 0;
int activeMode2 = 0;
int activeSuccessful = 0;
int activeFail = 0;

int id;
int case1 = 0;
int case2 = 0;
int onOffMode = 0;
int addUserMode = 0;
int rmUserMode = 0;

char pressed;

char keys[4][4] = 
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPin[4] = {22, 24, 26, 28};
byte colPin[4] = {30, 32, 34, 36};

Keypad keypad = Keypad(makeKeymap(keys), rowPin, colPin, 4, 4);
//////////////////////////////////////////////////////////////////

///////////////////////// SDcard   ///////////////////////////////
//////////////////////////////////////////////////////////////////
#include <SD.h>

File myFile;
const int chipSelect = 53;
String getString[100];
int data = 0;
boolean man = false;
char sendid[15];
char studentNameS[50];

void SDCard_Setup();
int find_RFID();
void read_tagID();
void ID_check();
void find_status();
void status_update(int statusline);
//////////////////////////////////////////////////////////////////

void setup() 
{ 
  Serial.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
///////////////////////// LCD setup //////////////////////////////
//////////////////////////////////////////////////////////////////
  ////////// assign default color value /////
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255, 255, 255);
  }
  ///////////////////////////////////////////
  updatePressedTimer = micros();
  displayStandbyTimer = micros();
  displayMainTimer = micros();
  displayModeTimer = micros();
  displayMode1Timer = micros();
  displayMode2Timer = micros();
  counterTimer = micros();
  lookForcardsTimer = micros();
  verifyCardsTimer = micros();
  displaySuccessfulTimer = micros();
  displayFailTimer = micros();

///////////////////////// Servo setup ////////////////////////////
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  
  myservo1.attach(8);
  myservo2.attach(9);
  myservo3.attach(10);

  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
///////////////////////// RTC setup //////////////////////////////
RTC_Setup();

///////////////////////// Finger /////////////////////////////////
// --fps.Open();         //send serial command to initialize fps
// --fps.SetLED(true);   //turn on LED so fps can see fingerprint

///////////////////////// SDcard /////////////////////////////////
  SDCard_Setup();
}

void loop() 
{
  RTC_Loop();
  updatePressed();
  counter();
  displayStandby();
  displayMain();
  displayMode();
  displayMode1();
  displayMode2();
  displaySuccessful();
  displayFail();

/////////////////  RFID  /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
  if (Serial2.available() >= 13 )                        //Make sure all the frame is received
  {
    // --fps.CaptureFinger(false);
    // --id = fps.Identify1_N();
    //delay(1000);
    Serial.println("Serial2 Available");
    
    if (Serial2.read() == 0x02 || (id < 200) )                                //Check for the start byte = 0x02
    {
      tagread = true;                                                         //New tag is read
      for (int index = 0; index < sizeof(tagID); index++)
      {
        //byte val = Serial2.read();
        char val = Serial2.read();
        //convert from ASCII value to value range from 0 to 9 and A to F
/*        if ( val >= '0' && val <= '9')
          val = val - '0';
        else if ( val >= 'A' && val <= 'F')
          val = 10 + val - 'A';

        tagID[index] = val;*/
        
        tagID[index]=val;
      }
    }
    else
    {
      tagread = false;                //Discard and skip
      Serial.println("Finger not found");
    }
  }
  if (tagread == true)                //New tag is read
  {
    Serial.print("Verified ID:");
    Serial.println(id);
    oldManCount = manCount;
    //////// Send data to cloud ////////
    ////////////////////////////////////
    Serial.println("+++tagread+++");
    
    //////// Pass or Not? //////////////
    if(compare_tag())   
    {
      Serial.println("+++successfully+++"); 
      activeStandby = 0; activeMain = 0; activeMode = 0; activeMode1 = 0; activeMode2 = 0; activeSuccessful = 1; activeFail = 0;
      send_tag();
      counterTimer = micros();
      
      //////// Admin or Not? //////////////
      if(adminAcc == 1)
      {
        Serial.println("+++Admin Acc+++");
        activeStandby = 0; activeMain = 1; activeMode = 0; activeMode1 = 0; activeMode2 = 0; activeSuccessful = 0; activeFail = 0;
        //adminAcc = 0;
      }
      else  //successful for user
      {
        Serial.print("manCount");
        Serial.println(manCount);
        
        //////// Air No.1  //////////////
        if (oldManCount == 1 && manCount == 2 )                 
        {
          digitalWrite(11, LOW);
          Servoout1();
          digitalWrite(11, HIGH);
        }
        if (oldManCount == 2 && manCount == 1 )                 
        {
          digitalWrite(11, LOW);
          Servoout1();
          digitalWrite(11, HIGH);
        }
        
        //////// Air No.2  //////////////
        if (oldManCount == 2 && manCount == 3 )                 
        {
          digitalWrite(12, LOW);
          Servoout2();
          digitalWrite(12, HIGH);
        }
        if (oldManCount == 3 && manCount == 2 )                 
        {
          digitalWrite(12, LOW);
          Servoout2();
          digitalWrite(12, HIGH);
        }

        //////// Air No.3  //////////////
        if (oldManCount == 3 && manCount == 4 )                 
        {
          digitalWrite(13, LOW);
          Servoout3();
          digitalWrite(13, HIGH);
        }
        if (oldManCount == 4 && manCount == 3 )                 
        {
          digitalWrite(13, LOW);
          Servoout3();
          digitalWrite(13, HIGH);
        }
            
      }
    }
    else
    {
      Serial.println("+++Not Authorize+++"); 
      activeStandby = 0; activeMain = 0; activeMode = 0; activeMode1 = 0; activeMode2 = 0; activeSuccessful = 0; activeFail = 1;
      counterTimer = micros();
    }
    clear_tag();
    
  tagread = false;  
  }
  
/////////////////  Keypad  ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

///////////////////// 1. OFF AIR/////////////////////////
  if ( pressed == '1' && adminAcc == 1)
  {        
    activeMain = 0; activeMode = 1; activeMode1 = 0; activeMode2 = 0;
    //Serial.println("activeMode = 1");
    //Serial.println("activeMode");
    onOffMode++;
    delay(100);
    //Serial.print("mode");
    //Serial.println(mode);
    if (pressed == '1' && activeMode == 1 && onOffMode == 2 && adminAcc == 1)
    {
      activeMain = 0; activeMode = 0; activeMode1 = 1; activeMode2 = 0; 
      onOffMode = 0;
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(13, LOW);
      Servoout1();
      Servoout2();
      Servoout3();
      digitalWrite(11, HIGH);
      digitalWrite(12, HIGH);
      digitalWrite(13, HIGH);
      counterTimer = micros();
      /////////////////////ON AIR/////////////////////////
      /////////////////////ON AIR/////////////////////////
    }
  }
  if (pressed == '2' && activeMode == 1 && onOffMode == 1 && adminAcc == 1)
  {
    activeMain = 0; activeMode = 0; activeMode1 = 0; activeMode2 = 1;
    onOffMode = 0;
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
    Servoout1();
    Servoout2();
    Servoout3();
    digitalWrite(11, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(13, HIGH);
    counterTimer = micros();
    /////////////////////OFF AIR/////////////////////////
    /////////////////////OFF AIR/////////////////////////  
  }

///////////////////// 2. Add User ID /////////////////////////
  if (pressed == '2' && activeMode == 0 && onOffMode == 0 && adminAcc == 1)
  {
    Serial.println("+++Add User ID+++");
    delay(100);
    //activeMain = 0; activeMode = 0; activeMode1 = 0; activeMode2 = 1;
    //counterTimer = micros();
    ///////////////////// Add User ID ////////////////////////
  }

///////////////////// 3. Remove User ID /////////////////////////
  if (pressed == '3' && activeMode == 0 && onOffMode == 0 && adminAcc == 1)
  {
    Serial.println("+++Remove User ID+++");
    delay(100);
    //activeMain = 0; activeMode = 0; activeMode1 = 0; activeMode2 = 1;
    //counterTimer = micros();
    ///////////////////// Add User ID ////////////////////////
  }
  
///////////////////////// End Loop /////////////////////////////
}

///////////////////////// display standby ////////////////////////
//////////////////////////////////////////////////////////////////
int displayStandby()
{
  if((micros() - displayStandbyTimer) >= 100000 && activeStandby == 1 &&activeMain == 0 && activeMode == 0 && activeMode1 == 0 && activeMode2 == 0) //0.1s
  {
    Serial.print("displayStandby\n");
    u8g.firstPage();
   do { u8g.setColorIndex(1);                                 
        u8g.drawBox(0, 0, 128, 11);                           
        u8g.setFont(u8g_font_6x10);                           
        u8g.setColorIndex(0);
        u8g.drawStr(1, 9,"The Black Box Project");
        u8g.setFont(u8g_font_6x10);
        u8g.setColorIndex(1); 
        u8g.drawStr(20, 25,"Welcome to FIBO");       
        //u8g.drawStr(25, 45,"Time: ");
        u8g.setPrintPos(9, 45); u8g.print(datestring);
        u8g.drawStr(50, 62,"No." );
        u8g.setPrintPos(70, 62); u8g.print(manCount);
       } while(u8g.nextPage());
    displayStandbyTimer = micros();
    return 1;
  }
  return 0;
}

///////////////////////// display main ///////////////////////////
//////////////////////////////////////////////////////////////////
int displayMain()
{
  if((micros() - displayMainTimer) >= 100000 && activeMain == 1 && activeMode == 0 && activeMode1 == 0 && activeMode2 == 0) //0.1s
  {
    Serial.print("displayMain\n");
    u8g.firstPage();
    do 
    {
      u8g.setFont(u8g_font_6x10);
      u8g.drawStr(50, 9,"List");
      u8g.drawStr(10, 25,"1. ON/OFF Air Cond.");
      u8g.drawStr(10, 42,"2. Add User ID.");
      u8g.drawStr(10, 62,"3. Remove User ID.");
      } while( u8g.nextPage() );
    displayMainTimer = micros();
    return 1;
  }
  return 0;
}

///////////////////////// display mode //////////////////////////
//////////////////////////////////////////////////////////////////
int displayMode()
{
  if((micros() - displayModeTimer) >= 100000 && activeMain == 0 && activeMode == 1 && activeMode1 == 0 && activeMode2 == 0) //0.1s
  {
    Serial.print("displayMode\n");
    u8g.firstPage();
    do 
    {
      u8g.setFont(u8g_font_6x10);
      u8g.drawStr(10, 9,"1. ON Air Cond.");
      u8g.drawStr(10, 25,"2. OFF Air Cond.");
      } while( u8g.nextPage() );
    displayModeTimer = micros();
    return 1;
  }
  return 0;
}

///////////////////////// display mode1 //////////////////////////
//////////////////////////////////////////////////////////////////
int displayMode1()
{
  if((micros() - displayMode1Timer) >= 100000 && activeMain == 0 && activeMode == 0 && activeMode1 == 1 && activeMode2 == 0 ) //0.1s
  {
    Serial.print("displayMode1\n");
    u8g.firstPage();
    do 
    {
      u8g.setFont(u8g_font_6x10);
      u8g.drawStr(10, 9,"ON Air Cond.");
      } while( u8g.nextPage() );
    displayMode1Timer = micros();
    return 1;
  }
  return 0;
}

///////////////////////// display mode2 //////////////////////////
//////////////////////////////////////////////////////////////////
int displayMode2()
{
  if((micros() - displayMode2Timer) >= 100000 && activeMain == 0 && activeMode == 0 && activeMode1 == 0 && activeMode2 == 1 ) //0.1s
  {
    Serial.print("displayMode2\n");
    u8g.firstPage();
    do 
    {
      u8g.setFont(u8g_font_6x10);
      u8g.drawStr(10, 9,"OFF Air Cond.");
      } while( u8g.nextPage() );
    displayMode2Timer = micros();
    return 1;
  }
  return 0;
}

///////////////////////// display successfully ///////////////////
//////////////////////////////////////////////////////////////////
int displaySuccessful()
{
  if((micros() - displaySuccessfulTimer) >= 100000 && activeStandby == 0 &&activeMain == 0 && activeMode == 0 && activeMode1 == 0 && activeMode2 == 0 
  && activeSuccessful == 1 && activeFail == 0 ) //0.1s
  {
    Serial.print("displaySuccessful\n");
    u8g.firstPage();
    do 
    {
      u8g.setFont(u8g_font_6x10);
      u8g.drawStr(25, 9,"Successfully");
      //u8g.drawStr(10, 25,"ID. ");
      u8g.setPrintPos(10, 25); u8g.print(studentName);
      } while( u8g.nextPage() );
    displaySuccessfulTimer = micros();
    return 1;
  }
  return 0;
}

///////////////////////// display fail ///////////////////////////
//////////////////////////////////////////////////////////////////
int displayFail()
{
  if((micros() - displayFailTimer) >= 100000 && activeStandby == 0 &&activeMain == 0 && activeMode == 0 && activeMode1 == 0 && activeMode2 == 0 
  && activeSuccessful == 0 && activeFail == 1 ) //0.1s
  {
    Serial.print("displayFail\n");
    u8g.firstPage();
    do 
    {
      u8g.setFont(u8g_font_6x10);
      u8g.drawStr(25, 9,"Not Authorize");
      u8g.drawStr(10, 25,"ID. ");
      } while( u8g.nextPage() );
    displayFailTimer = micros();
    return 1;
  }
  return 0;
}

///////////////////////// update pressed //////////////////////////
//////////////////////////////////////////////////////////////////
int updatePressed()
{
  if((micros() - displayMainTimer) >= 100000) //0.1s
  {
    pressed = keypad.getKey();
    updatePressedTimer = micros();
    //Serial.println("updatePressedTimer");
    //Serial.println(pressed);
  }
}

///////////////////////// counter timer //////////////////////////
//////////////////////////////////////////////////////////////////
int counter()
{
  if((micros() - counterTimer) >= 2000000) //2s
  {
    // Reset LCD after air is opened
    if(activeMain == 0 && activeMode == 0 && activeMode1 == 1 && activeMode2 == 0)
    {
      activeStandby = 1; activeMain = 0; activeMode = 0; activeMode1 = 0; activeMode2 = 0; activeSuccessful = 0; activeFail == 0;
      adminAcc = 0;
    }
    
    // Reset LCD after air is closed
    if(activeMain == 0 && activeMode == 0 && activeMode1 == 0 && activeMode2 == 1)
    {
      activeStandby = 1; activeMain = 0; activeMode = 0; activeMode1 = 0; activeMode2 = 0; activeSuccessful = 0; activeFail == 0;
      adminAcc = 0;
    }
    
    // Reset LCD after scan sucessfully
    if(activeStandby == 0 && activeMain == 0 && activeMode == 0 && activeMode1 == 0 && activeMode2 == 0 && activeSuccessful == 1 && activeFail == 0)
    {
      activeStandby = 1; activeMain = 0; activeMode = 0; activeMode1 = 0; activeMode2 = 0; activeSuccessful = 0; activeFail == 0;
    }

    // Reset LCD after scan sucessfully
    if(activeStandby == 0 && activeMain == 0 && activeMode == 0 && activeMode1 == 0 && activeMode2 == 0 && activeSuccessful == 0 && activeFail == 1)
    {
      activeStandby = 1; activeMain = 0; activeMode = 0; activeMode1 = 0; activeMode2 = 0; activeSuccessful = 0; activeFail == 0;
    }
    
    counterTimer = micros();
    return 1;
  }
  return 0;
}
///////////////////////// clear tag //////////////////////////////
//////////////////////////////////////////////////////////////////
void clear_tag()                      //Subroutine to clear the tagID content
{
  Serial.println("clear tag");
  for (int index = 0; index < sizeof(tagID); index++)
  {
    tagID[index] = 0;
  }
}
///////////////////////// clear tag //////////////////////////////
//////////////////////////////////////////////////////////////////End

///////////////////////// compare tag NEW ////////////////////////
//////////////////////////////////////////////////////////////////

///////////////////////// compare tag OLD ////////////////////////
//////////////////////////////////////////////////////////////////
/*int compare_tag()
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
  if (card_001_temp == 10 || id == 1)                                // ID card FIBO_001
  {
    studentName = "Chawakorn Sri-ngernyuang";
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
  return 1;
  }
  .
  .
  .
  if (card_011_temp == 10)
  { 
    Serial.print("Name: No One\r\n");
    return 0;
  }

  if (card_001_temp != 10 && card_002_temp != 10 && card_003_temp != 10 && card_004_temp != 10 && card_005_temp != 10
      && card_006_temp != 10 && card_007_temp != 10 && card_008_temp != 10 && card_009_temp != 10 && card_010_temp != 10 && card_011_temp != 10)
  {
    Serial.print("Name: Unknown Subscriber\r\n");
    return 0;
  }
return 0;
}*/

///////////////////////// RTC print //////////////////////////////
//////////////////////////////////////////////////////////////////
void printDateTime(const RtcDateTime& dt)
{
  //char datestring[20];          //change to globle

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
             dt.Month(),
             dt.Day(),
             dt.Year(),
             dt.Hour(),
             dt.Minute(),
             dt.Second() );
  //Serial.print(datestring);
}

///////////////////////// RTC setup //////////////////////////////
//////////////////////////////////////////////////////////////////
void RTC_Setup() 
{
  Serial.print("compiled: ");
  Serial.print(__DATE__);
  Serial.println(__TIME__);

  //--------RTC SETUP ------------
  // if you are using ESP-01 then uncomment the line below to reset the pins to
  // the available pins for SDA, SCL
  //Wire.begin(0, 2); // due to limited pins, use pin 0 and 2 for SDA, SCL

  Rtc.Begin();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  printDateTime(compiled);
  Serial.println();

  if (!Rtc.IsDateTimeValid())
  {
    // Common Cuases:
    //    1) first time you ran and the device wasn't running yet
    //    2) the battery on the device is low or even missing

    Serial.println("RTC lost confidence in the DateTime!");

    // following line sets the RTC to the date & time this sketch was compiled
    // it will also reset the valid flag internally unless the Rtc device is
    // having an issue

    Rtc.SetDateTime(compiled);
  }

  if (!Rtc.GetIsRunning())
  {
    Serial.println("RTC was not actively running, starting now");
    Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled)
  {
    Serial.println("RTC is older than compile time!  (Updating DateTime)");
    Rtc.SetDateTime(compiled);
  }
  else if (now > compiled)
  {
    Serial.println("RTC is newer than compile time. (this is expected)");
  }
  else if (now == compiled)
  {
    Serial.println("RTC is the same as compile time! (not expected but all is fine)");
  }

  // never assume the Rtc was last configured by you, so
  // just clear them to your needed state
  Rtc.SetSquareWavePin(DS1307SquareWaveOut_Low);

}

///////////////////////// RTC Loop ///////////////////////////////
//////////////////////////////////////////////////////////////////
void RTC_Loop() {
  if (!Rtc.IsDateTimeValid())
  {
    // Common Cuases:
    //    1) the battery on the device is low or even missing and the power line was disconnected
    Serial.println("RTC lost confidence in the DateTime!");
  }

  RtcDateTime now = Rtc.GetDateTime();

  printDateTime(now);
  //Serial.println();
}

///////////////////////// Servo out //////////////////////////////
//////////////////////////////////////////////////////////////////
void Servoout1()
{
  for(pos = 150; pos > 90; pos -= 1) // goes from 0 degrees to 180 degrees
  {
    myservo1.write(pos); // tell servo to go to position in variable 'pos'
    delay(10); // waits 15ms for the servo to reach the position
  }
  for(pos = 90; pos < 150 ; pos +=1)// goes from 180 degrees to 0 degrees
  {
    myservo1.write(pos); // tell servo to go to position in variable 'pos'
    delay(10); // waits 15ms for the servo to reach the position
  } 
}

void Servoout2()
{
  for(pos = 150; pos > 90; pos -= 1) // goes from 0 degrees to 180 degrees
  {
    myservo2.write(pos); // tell servo to go to position in variable 'pos'
    delay(10); // waits 15ms for the servo to reach the position
  }
  for(pos = 90; pos < 150 ; pos +=1)// goes from 180 degrees to 0 degrees
  {
    myservo2.write(pos); // tell servo to go to position in variable 'pos'
    delay(10); // waits 15ms for the servo to reach the position
  } 
}

void Servoout3()
{
  for(pos = 150; pos > 90; pos -= 1) // goes from 0 degrees to 180 degrees
  {
    myservo3.write(pos); // tell servo to go to position in variable 'pos'
    delay(10); // waits 15ms for the servo to reach the position
  }
  for(pos = 90; pos < 150 ; pos +=1)// goes from 180 degrees to 0 degrees
  {
    myservo3.write(pos); // tell servo to go to position in variable 'pos'
    delay(10); // waits 15ms for the servo to reach the position
  } 
}

///////////////////////// Sand Tag ///////////////////////////////
//////////////////////////////////////////////////////////////////

void send_tag()                                                 //Subroutine to display the tagID content
{
  String dataID =  datestring;
         dataID += "|";
         dataID += studentName;
         dataID += "|";
         dataID += manCount;
         dataID += "|";
  Serial3.print(dataID);
}
//////////////////////////////////////////////////////////////////

/////////////////////////  SD+Compare ////////////////////////////
//////////////////////////////////////////////////////////////////
void SDCard_Setup() 
{
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) 
  {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done");
}

int compare_tag()                                 //check RFID match in RfData.txt
{
  myFile = SD.open("RfData.txt");
  char inputString[100];
  int indexa = 0;
  int chkCorrect = 0;
  int Rfdata = 0;
  if (myFile) 
  {
    while (myFile.available())                  //read from SD until there's nothing else in it
    {              
      char inputChar = myFile.read();
      if(inputChar == '\n')                     //if end of line
      {                
        inputString[indexa] = '\0';             //lets the rest index are none
        if (chkCorrect == 10) 
        {
          Rfdata += 1;                          //when find data MATCH!
          for (int j = 0; j < 11; j++)          //contain ID for check match in UserData
          {      
            sendid[j] = inputString[j + 11];    //send ID when read tagID
          }
          
          ID_check();
          find_status();

          return 1;
        }
        indexa = 0;                             //reset indexa
        chkCorrect = 0;                         //reset chkCorrect
      } 
      else 
      {                                         //if it has not end of line yet
        inputString[indexa] = inputChar;
        if (indexa == 10)                       //check match ID when read until first 10 char
        {             
          for (int i = 0; i < 10; i++)          //loop match check 10 index
          {        
            if (tagID[i] == inputString[i]) 
            {
              chkCorrect++;
            }
          }
        }
        indexa++;
      }
    }
    myFile.close();
    
    if (Rfdata == 0) 
    {
      Serial.println("No RFID Found!!!");
      tagread = false;
      return 0;
    }
    Rfdata = 0;                                 //reset data found
  } 
  else 
  {
    Serial.println("error opening RfData");
    tagread = false;
  }
}

void ID_check()                                 //check ID match in UserData.txt
{
  myFile = SD.open("UserData.txt");
  char inputString[100];
  int indexa = 0;
  int chkCorrect = 0;
  int line = 0;
  if (myFile) 
  {
    while (myFile.available())                  //read from UserData until there's nothing else in it
    {              
      char inputChar = myFile.read();
      if (inputChar == '\n')                    //if end of line
      {          
        inputString[indexa] = '\0';             //lets the rest index are none
        if (chkCorrect == 11) 
        {
          Serial.println(inputString);          //print ID and Name
          for (int j = 0; j < 30; j++)          //contain ID for check match in UserData
          { 
            studentNameS[j] = inputString[j+12];
          }
          studentName = studentNameS;
          //Serial.println(studentNameS);
          Serial.println(studentName);
          data += 1;
        }
        indexa = 0;                             //reset indexa
        chkCorrect = 0;                         //reset chkCorrect
      } 
      else 
      {                          
        inputString[indexa] = inputChar;
        if (indexa == 10)                       //check match ID when read until first 11 char
        {                     
          for (int i = 0; i < 11; i++)          //loop match check 11 index
          {        
            if (sendid[i] == inputString[i]) 
            {
              chkCorrect++;
            }
          }
        }
        indexa++;
      }
    }
    myFile.close();
    line = 0;                                   //reset line
    
    if (data == 0) 
    {
      Serial.println("No Data!!!");
    }
    data = 0;                                 //reset data found
  } 
  else 
  {
    Serial.println("error opening UserData");
  }
}

void find_status()                            //check ID match in test.txt
{
  //myFile = SD.open("test.txt", FILE_WRITE); //read and write
  myFile = SD.open("status.txt", FILE_WRITE);
  myFile.seek(0);
  char inputString[100];
  int indexa = 0;
  int chkCorrect = 0;
  int line = 0, sta = 0;
  if (myFile) 
  {
    while (myFile.available())                // read from UserData until there's nothing else in it
    {                                           
      char inputChar = myFile.read();
      if (inputChar == '\n')                  //if end of line
      {          
        inputString[indexa] = '\0';           //lets the rest index are none
        line += 1;                            //find line where data is
        if (chkCorrect == 11) 
        {
          status_update(line);
          sta += 1;                           //when find ID MATCH
        }
        indexa = 0;                           //reset indexa
        chkCorrect = 0;                       //reset check Correct
      } 
      else                                    //if it has not end of line yet
      {                                
        inputString[indexa] = inputChar;
        if (indexa == 10)                       //check match ID when read until first 11 char
        {             
          for (int i = 0; i < 11; i++)          //loop match check 11 index
          {       
            if (sendid[i] == inputString[i]) 
            {
              chkCorrect++;
            }
          }
        }
        indexa++;
      }
    }
    line = 0;                                   //reset line
    if (sta == 0) 
    {
      Serial.println("New user check in");
      myFile.print(sendid);
      myFile.print(" ");
      myFile.println("1");
      manCount++;
    }
    myFile.close();
    sta = 0;                                    //reset data found
  } 
  else 
  {
    Serial.println("error opening test");
  }
}

void status_update(int statusline)
{
  int pos = 0;
  char showStatus;
  pos = 12 + 15 * (statusline - 1);
  Serial.print("pos");
  Serial.println(pos);
  Serial.print("statusline");
  Serial.println(statusline);
  myFile.seek(pos);
  showStatus = myFile.read();
  /*myFile.seek(pos);
    Serial.print("data position:");
    Serial.println(myFile.position());
    Serial.print("user status:");
    Serial.println(showStatus);*/
  if (showStatus == '0') 
  {
    manCount++;
    myFile.seek(pos);
    myFile.print("1");
    Serial.print("In");
  } 
  else if (showStatus == '1') 
  {
    manCount--;
    myFile.seek(pos);
    myFile.print("0");
    Serial.print("Out");
  }
  //Serial.println(manCount);
  man = true;
}
//////////////////////////////////////////////////////////////////
