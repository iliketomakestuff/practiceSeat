/*  
* This script was created by Bob Clagett for I Like To Make Stuff
* For more projects, check out iliketomakestuff.com
* 
* This code is for the project at https://iliketomakestuff.com/progress-tracking-practice-seat/
* 
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
*  Communicates: practiceSeatv1.ino
*/ 
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>

#define PIN        D4 
#define NUMPIXELS 48 

byte ledPin = 2;
char ssid[] = "######";               // SSID of your home WiFi
char pass[] = "######";               // password of your home WiFi

int pixelPercentage = 0;
int lastPixelPercentage = 0;
int lastPercentageReading=0;
unsigned long askTimer = 0;
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

IPAddress server(192,168,1, 199);       // the fix IP address of the server
WiFiClient client;

void setup() {
  Serial.begin(115200);               // only for debug
  WiFi.begin(ssid, pass);             // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("*");
    delay(500);
  }

  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif
  // END of Trinket-specific code.

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
     pixels.clear();
}

void displayPercentage(int perc){
  if(perc==0){
    pixels.clear(); // Set all pixel colors to 'off'
  }
  if(perc != lastPercentageReading){
    lastPixelPercentage = pixelPercentage;
    lastPercentageReading = perc;
    pixelPercentage = NUMPIXELS/2*perc/100;
    int brt = 255;
     pixels.clear(); // Set all pixel colors to 'off'
  
    // The first NeoPixel in a strand is #0, second is 1, all the way up
    // to the count of pixels minus one.
    for(int i=0; i<pixelPercentage; i++) { // For each pixel...
       if(i % 2==0){
          if(i<lastPixelPercentage){ //these should have already been lit up
            pixels.setPixelColor(i, pixels.Color(brt, 0, 0));
            pixels.setPixelColor(i+NUMPIXELS/2, pixels.Color(brt, 0, 0));
          } else {
          //these are new, so fade them in
            pixels.show();
             for(int k=0;k<=brt;k+=10){
                pixels.setPixelColor(i, pixels.Color(k, 0, 0));
                pixels.setPixelColor(i+NUMPIXELS/2, pixels.Color(k, 0, 0)); 
                delay(100);
                pixels.show();
             }
          }
       }
        // Send the updated pixel colors to the hardware.
    }
  } 
   pixels.show();
}

void displayComplete(){
  for(int w=0; w<3; w++) {
    
  
    for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
         if(i % 2==0){
            pixels.setPixelColor(i, pixels.Color(0, 0, 0));
            pixels.setPixelColor(i+NUMPIXELS/2, pixels.Color(0, 0, 0));
         }
        pixels.show();   // Send the updated pixel colors to the hardware.
      } 
      delay(500);
      for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
         if(i % 2==0){
            pixels.setPixelColor(i, pixels.Color(0, 255, 0));
            pixels.setPixelColor(i+NUMPIXELS/2, pixels.Color(0, 255, 0));
         }
        pixels.show();   // Send the updated pixel colors to the hardware.
      } 
      delay(500);
  } 
}

void loop () {
  client.connect(server, 80);   // Connection to the server
  digitalWrite(ledPin, LOW);    // to show the communication only (inverted logic)
  Serial.println(".");
  client.println("Hello server! Are you sleeping?\r");  // sends the message to the server
  String answer = client.readStringUntil('\r');   // receives the answer from the sever
  Serial.println(answer+"%");
  int per = answer.toInt();
  if(per == 100){
    displayComplete();
  } else {
    displayPercentage(per);
  }
  //handle LED percentage display
  
  client.flush();
  digitalWrite(ledPin, HIGH);
  delay(1000);                  // client will trigger the communication after two seconds
}
