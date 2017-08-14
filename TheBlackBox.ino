/*
 * Mini class project
 * Project name: The Black Box
 * 
 * rfid ID-20LA
 * arduino mega2560
 */

byte tagID[12];         //check the data and checksum for ID card
boolean tagread=false;
int manCount;           //the numbers of the man in system
int oldManCount;        //pervious numbers of the man in system


//ID card
const byte card_001[10] = {0x00, 0x09, 0x00, 0x00, 0x08, 0x09, 0x0A, 0x0E, 0x03, 0x07}; //Tag ID: 090089AE37, Name: FIBO_OO1
const byte card_002[10] = {0x00, 0x09, 0x00, 0x00, 0x08, 0x09, 0x09, 0x0B, 0x0A, 0x0C}; //Tag ID: 0900899BAC, Name: FIBO_OO2
const byte card_003[10] = {0x00, 0x09, 0x00, 0x00, 0x08, 0x09, 0x0B, 0x07, 0x0D, 0x05}; //Tag ID: 090089B7D5, Name: FIBO_OO3
const byte card_004[10] = {0x00, 0x09, 0x00, 0x00, 0x08, 0x09, 0x0D, 0x00, 0x06, 0x02}; //Tag ID: 090089D062, Name: FIBO_OO4
const byte card_005[10] = {0x00, 0x09, 0x00, 0x00, 0x08, 0x09, 0x0A, 0x0B, 0x0F, 0x07}; //Tag ID: 090089ABF7, Name: FIBO_OO5
const byte card_006[10] = {0x00, 0x09, 0x00, 0x00, 0x08, 0x09, 0x0C, 0x04, 0x0E, 0x0F}; //Tag ID: 090089C4EF, Name: FIBO_006
const byte card_007[10] = {0x00, 0x0D, 0x00, 0x00, 0x04, 0x02, 0x07, 0x0C, 0x03, 0x0D}; //Tag ID: 0D00427C3D, Name: FIBO_007
const byte card_008[10] = {0x00, 0x09, 0x00, 0x00, 0x08, 0x09, 0x0D, 0x05, 0x0E, 0x06}; //Tag ID: 090089D5E6, Name: FIBO_008
const byte card_009[10] = {0x00, 0x09, 0x00, 0x00, 0x08, 0x09, 0x0C, 0x0F, 0x0C, 0x0C}; //Tag ID: 090089CFCC, Name: FIBO_009
const byte card_010[10] = {0x00, 0x0D, 0x00, 0x00, 0x04, 0x02, 0x09, 0x05, 0x0C, 0x09}; //Tag ID: 0D004295C9, Name: FIBO_010
const byte card_011[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //Tag ID: 0000000000, Name: NoOne

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
  
void setup()
{
  Serial.begin(9600);   //send data to computer
  Serial2.begin(9600);  //receive data from rfid reader
  Serial3.begin(9600);  //send data to ESP8266
}

void loop()
{

  if(Serial2.available()>=13)                           //Make sure all the frame is received
  {
    if(Serial2.read()==0x02)                            //Check for the start byte = 0x02
    {
      tagread=true;                                     //New tag is read
      for(int index=0;index<sizeof(tagID);index++)
      {
        byte val=Serial2.read();
        
        //convert from ASCII value to value range from 0 to 9 and A to F
        if( val >= '0' && val <= '9')
        val = val - '0';
        else if( val >= 'A' && val <= 'F')
        val = 10 + val - 'A';
        
        tagID[index]=val;
      }
    }
    else
    {
      tagread=false;                  //Discard and skip
    }
  }

  if(tagread==true)                   //New tag is read
  {
    oldManCount = manCount;
    compare_tag();                    //map tag ID with user name
    Serial.print("manCount: ");
    Serial.print(manCount);
    Serial.print("\r\n");
    clear_tag();                      //Clear the tag ID and ready for next tag
    
      
    if(oldManCount == 4 && manCount == 5)                  //the numbers of man in system change from 4 to 5 
    {
      //oldManCount = manCount;
      //compare_tag();                  //map tag ID with user name
      Serial.print("Servo: ON ");                         //Servo "ON"
      //Serial.print("manCount: ");
      //Serial.print(manCount);
      Serial.print("\r\n");
      //print_tag();                    //Display the tag ID
      //clear_tag();                    //Clear the tag ID and ready for next tag
      tagread=false;
    }
    
    if(oldManCount == 5 && manCount == 4)                 //the number of man in system change from 5 to 4
    {
      //oldManCount = manCount;
      //compare_tag();                  //map tag ID with user name
      Serial.print("Servo: OFF ");                        //Servo "OFF"
      //Serial.print("manCount: ");               
      //Serial.print(manCount);
      Serial.print("\r\n");
      //print_tag();                    //Display the tag ID
      //clear_tag();                    //Clear the tag ID and ready for next tag
      tagread=false; 
    }
    tagread=false;
  }
  delay(10);
}

void print_tag()                      //Subroutine to display the tagID content
{
  Serial.print("Tag ID: ");        
  for(int index=0;index<10;index++)
  {
    Serial.print(tagID[index],HEX);     //Display tagID
  }
  Serial.print("\r\nChecksum: ");
  Serial.print(tagID[10], HEX);            //Display checksum
  Serial.println(tagID[11]), HEX;
}

void clear_tag()                      //Subroutine to clear the tagID content
{  
  for(int index=0;index<sizeof(tagID);index++)
  {
    tagID[index]=0;
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

  
  for(int index = 0; index < 10; index++)                 //check 10 byte of data 
  {
    if(tagID[index] == card_001[index])card_001_temp++;            
    if(tagID[index] == card_002[index])card_002_temp++;           
    if(tagID[index] == card_003[index])card_003_temp++;             
    if(tagID[index] == card_004[index])card_004_temp++;             
    if(tagID[index] == card_005[index])card_005_temp++;           
    if(tagID[index] == card_006[index])card_006_temp++;           
    if(tagID[index] == card_007[index])card_007_temp++;             
    if(tagID[index] == card_008[index])card_008_temp++;             
    if(tagID[index] == card_009[index])card_009_temp++;           
    if(tagID[index] == card_010[index])card_010_temp++;           
    if(tagID[index] == card_011[index])card_011_temp++;           
  }
    if(card_001_temp == 10)                              // ID chad FIBO_001
    {
      Serial3.print('A');                                //Send 'A' to ESP8266
      if(card_001_state == 0)
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
    
    if(card_002_temp == 10)                              // ID chad FIBO_002
    {
      Serial3.print('B');                                //Send 'B' to ESP8266
      if(card_002_state == 0)
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
    
    if(card_003_temp == 10)                              // ID chad FIBO_003
    {
      Serial3.print('C');                                //Send 'C' to ESP8266
      if(card_003_state == 0)
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

    if(card_004_temp == 10)                              // ID chad FIBO_004
    {
      Serial3.print('D');                                //Send 'D' to ESP8266
      if(card_004_state == 0)
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
    
    if(card_005_temp == 10)                              // ID chad FIBO_005
    {
      Serial3.print('E');                                //Send 'E' to ESP8266
      if(card_005_state == 0)
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

    if(card_006_temp == 10)                              // ID chad FIBO_006
    {
      Serial3.print('L');                                //Send 'L' to ESP8266 (**send 'F' does not work)
      if(card_006_state == 0)
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
    
    if(card_007_temp == 10)                              // ID chad FIBO_007
    {
      Serial3.print('G');                                //Send 'G' to ESP8266
      if(card_007_state == 0)
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

    if(card_008_temp == 10)                              // ID chad FIBO_008
    {
      Serial3.print('H');                                //Send 'H' to ESP8266
      if(card_008_state == 0)
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
    
    if(card_009_temp == 10)                              // ID chad FIBO_009
    {
      Serial3.print('I');                                //Send 'I' to ESP8266
      if(card_009_state == 0)
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

    if(card_010_temp == 10)                               // ID chad FIBO_010
    {
      Serial3.print('J');                                //Send 'J' to ESP8266
      if(card_010_state == 0)
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
    
    if(card_011_temp == 10)Serial.print("Name: No One\r\n"); 

    if(card_001_temp != 10 && card_002_temp != 10 && card_003_temp != 10 && card_004_temp != 10 && card_005_temp != 10
    && card_006_temp != 10 && card_007_temp != 10 && card_008_temp != 10 && card_009_temp != 10 && card_010_temp != 10 && card_011_temp != 10)
    {
      Serial.print("Name: Unknown Subscriber\r\n");
      Serial3.print('K');                                //Send 'K' to ESP8266
    }
}

