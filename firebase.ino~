#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <time.h>

// Config Firebase
#define FIREBASE_HOST "XXXXX.firebaseio.com"
#define FIREBASE_AUTH "SSSSS"

// Config connect WiFi
#define WIFI_SSID "Since1989"
#define WIFI_PASSWORD "since_16"

// Config time
int timezone = 7;
char ntp_server1[20] = "time.navy.mi.th";
char ntp_server2[20] = "fw.eng.ku.ac.th";
char ntp_server3[20] = "time.uni.net.th";
int dst = 0;

//int i = 0;
String userName;
byte tagID[1];         //check the data and checksum for ID card


void setup() 
{
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  configTime(timezone * 3600, dst, ntp_server1, ntp_server2, ntp_server3);
  Serial.println("Waiting for time");
  
  while (!time(nullptr)) 
  {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println();
  Serial.println("Now: " + NowString());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}


void loop() 
{

  if (Serial.available()>0)
  {
    for(int index=0;index<sizeof(tagID);index++)
      {
        byte val=Serial.read();
        
        //convert from ASCII value to value range from 0 to 9 and A to F
        if( val >= '0' && val <= '9')
        val = val - '0';
        else if( val >= 'A' && val <= 'F')
        val = 10 + val - 'A';
        
        tagID[index]=val;
      }
      
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();

    if(Firebase.failed()) 
    {
      Serial.print("pushing /number failed:");
      Serial.println(Firebase.error());  
      return;
    }

    //Serial.print(Serial.read());
    Serial.print(tagID[0], HEX);
      
    if(tagID[0] == 0x41)                         //recieve 'A' from Arduino
    {  
      userName = "FIBO_001";
      root["name"] = userName;
      root["time"] = NowString();
      String objectName = Firebase.push("data logger", root);
      Serial.print("pushed /data to ");
      Serial.println(objectName);
      delay(3000);
    }

    if(tagID[0] == 0x42)                        //recieve 'B' from Arduino
    {  
      userName = "FIBO_002";
      root["name"] = userName;
      root["time"] = NowString();
      String objectName = Firebase.push("data logger", root);
      Serial.print("pushed /data to ");
      Serial.println(objectName);
      delay(3000);
    }

    if(tagID[0] == 0x43)                         //recieve 'C' from Arduino
    {  
      userName = "FIBO_003";
      root["name"] = userName;
      root["time"] = NowString();
      String objectName = Firebase.push("data logger", root);
      Serial.print("pushed /data to ");
      Serial.println(objectName);
      delay(3000);
    }

    if(tagID[0] == 0x44)                        //recieve 'D' from Arduino
    {  
      userName = "FIBO_004";
      root["name"] = userName;
      root["time"] = NowString();
      String objectName = Firebase.push("data logger", root);
      Serial.print("pushed /data to ");
      Serial.println(objectName);
      delay(3000);
    }

    if(tagID[0] == 0x45)                         //recieve 'E' from Arduino
    {  
      userName = "FIBO_005";
      root["name"] = userName;
      root["time"] = NowString();
      String objectName = Firebase.push("data logger", root);
      Serial.print("pushed /data to ");
      Serial.println(objectName);
      delay(3000);
    }

    if(tagID[0] == 0x4C)                        //recieve 'F' from Arduino "Not work", so change to 'L'
    {  
      userName = "FIBO_006";
      root["name"] = userName;
      root["time"] = NowString();
      String objectName = Firebase.push("data logger", root);
      Serial.print("pushed /data to ");
      Serial.println(objectName);
      delay(3000);
    }

    if(tagID[0] == 0x47)                         //recieve 'G' from Arduino
    {  
      userName = "FIBO_007";
      root["name"] = userName;
      root["time"] = NowString();
      String objectName = Firebase.push("data logger", root);
      Serial.print("pushed /data to ");
      Serial.println(objectName);
      delay(3000);
    }

    if(tagID[0] == 0x48)                        //recieve 'H' from Arduino
    {  
      userName = "FIBO_008";
      root["name"] = userName;
      root["time"] = NowString();
      String objectName = Firebase.push("data logger", root);
      Serial.print("pushed /data to ");
      Serial.println(objectName);
      delay(3000);
    }

    if(tagID[0] == 0x49)                        //recieve 'I' from Arduino
    {  
      userName = "FIBO_009";
      root["name"] = userName;
      root["time"] = NowString();
      String objectName = Firebase.push("data logger", root);
      Serial.print("pushed /data to ");
      Serial.println(objectName);
      delay(3000);
    }

    if(tagID[0] == 0x4A)                         //recieve 'J' from Arduino
    {  
      userName = "FIBO_010";
      root["name"] = userName;
      root["time"] = NowString();
      String objectName = Firebase.push("data logger", root);
      Serial.print("pushed /data to ");
      Serial.println(objectName);
      delay(3000);
    }

    if(tagID[0] == 0x4B)                        //recieve 'K' from Arduino
    {  
      userName = "Unknown Subscriber";
      root["name"] = userName;
      root["time"] = NowString();
      String objectName = Firebase.push("data logger", root);
      Serial.print("pushed /data to ");
      Serial.println(objectName);
      delay(3000);
    }
   
  }

  
}

String NowString() 
{
  time_t now = time(nullptr);
  struct tm* newtime = localtime(&now);
   
  String tmpNow = "";
  tmpNow += String(newtime->tm_year + 1900);
  tmpNow += "-";
  tmpNow += String(newtime->tm_mon + 1);
  tmpNow += "-";
  tmpNow += String(newtime->tm_mday);
  tmpNow += " ";
  tmpNow += String(newtime->tm_hour);
  tmpNow += ":";
  tmpNow += String(newtime->tm_min);
  tmpNow += ":";
  tmpNow += String(newtime->tm_sec);
  return tmpNow;
}
