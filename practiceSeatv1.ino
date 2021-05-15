/*
* This script was created by Bob Clagett for I Like To Make Stuff
* For more projects, check out iliketomakestuff.com
* 
* This code is for the project at https://iliketomakestuff.com/progress-tracking-practice-seat/
* 
* All of the components are list on the url above.
* 
* We offer NO SUPPORT for this code, and do not promise any updates/improvements.
* Please regard this as "unoptimized, prototype code".
* Feel free to branch and extend this code, but we will likely NOT merge in any changes.
* 
*Connects to the home WiFi network
*  Asks some network parameters
*  Sends and receives message from the server in every 2 seconds
*  Communicates: practiceSeatDisplayv1.ino
*/
#include <SPI.h>
#include <ESP8266WiFi.h>

char ssid[] = "######";               // SSID of your home WiFi
char pass[] = "######";               // password of your home WiFi
WiFiServer server(80);                    

IPAddress ip(192, 168, 1, 199);            // IP address of the server
IPAddress gateway(192,168,1,1);           // gateway of your network
IPAddress subnet(255,255,255,0);          // subnet mask of your network

const int touchSensorPin = D6;
const int soundPin = D2;
const int ledPin = LED_BUILTIN;

int soundVal = 0;

float sessionCount;
float targetTimeInMinutes = .5;

void setup() {
  pinMode(ledPin,OUTPUT);
  pinMode(soundPin,INPUT);
  pinMode(touchSensorPin,INPUT_PULLUP);
  Serial.begin(115200);
  Serial.println("ready!");

  WiFi.config(ip, gateway, subnet);       // forces to use the fix IP
  WiFi.begin(ssid, pass);                 // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("*");
    delay(500);
  }
  server.begin();                         // starts the server
  Serial.println("Connected to wifi");
}

void loop() {

  int sitting = digitalRead(touchSensorPin);
   if (sitting == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
   
    if(sitting==LOW){
    
      soundVal = digitalRead(soundPin);
        if (soundVal == LOW)
        {
          //digitalWrite(ledPin, HIGH);
        }
        else
        {
          //digitalWrite(ledPin, LOW);
          //Serial.println("NOISE DETECTED");
          sessionCount+=.5;

          delay(500);
        }
      }
      else {
        Serial.println("NOT SEATED!");
      if(sessionCount>0){
        sessionCount--;
        delay(500);
        
      }    
    }
    float percentComplete = sessionCount*100/(targetTimeInMinutes*60);
    if(percentComplete>100){
      percentComplete=100; 
     }
     if(percentComplete>0){
      Serial.print(percentComplete);
      Serial.println("%");
     }
    
    WiFiClient client = server.available();
    if (client) {
      if (client.connected()) {
        //digitalWrite(ledPin, LOW);  // to show the communication only (inverted logic)
        //Serial.println(".");
        String request = client.readStringUntil('\r');    // receives the message from the client
        //Serial.print("From client: "); Serial.println(request);
        client.flush();
         
        client.println(percentComplete); // sends the answer to the client
        //digitalWrite(ledPin, HIGH);
      }
      client.stop();                // tarminates the connection with the client
    } 
 } 
