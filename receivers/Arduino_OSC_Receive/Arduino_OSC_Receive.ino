/*
  Simple Arduino OSC Receiver
  Yonatan Rozin

  Enter WiFi network details below
  Choose a port #

  Run sketch, check serial monitor for Arduino's local IP address
  Use the IP address and port # to send UDP to this Arduino
*/

#include <WiFiNINA.h> //you may need <WiFi.h> instead, depending on your board

#include <OSCBundle.h> //not included in IDE by default. Install "OSC" package by Adrian Freed from library manager

#include <Servo.h>

//these are case-sensitive!!
const char WIFI_SSID[] = ""; //put WiFi network name here
const char WIFI_PASS[] = ""; //put WiFi password here

//choose a port # - make sure your sender device uses it!
const long PORT = 1234; 

Servo servo; 
WiFiUDP Udp;

int status = WL_IDLE_STATUS;

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  if (!Serial) delay(2000);
  servo.attach(4);

  Serial.println("Connecting to WiFi.");
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network ");
    Serial.println(WIFI_SSID);      
    status = WiFi.begin(WIFI_SSID, WIFI_PASS);
    delay(1000);
  }
  Serial.print("WiFi connected.\nIP address: ");
  Serial.println(WiFi.localIP());

  Udp.begin(PORT);
}

void loop(){ 
  OSCBundle bundle;
  int size = Udp.parsePacket();

  if ( size > 0) {
    while(size--) bundle.fill(Udp.read());

    if (!bundle.hasError()) {
      //route your OSC addresses here
      // "*" is a wildcard that matches ANY value at that address level
      bundle.dispatch("/*/pad", getPad);
      bundle.dispatch("/*/button", getButton);
    }
  }
}

void getPad(OSCMessage &msg){  

  float x = msg.getFloat(0); //get first argument
  float y = msg.getFloat(1); //get second argument
  Serial.print("Pad: ");
  Serial.print(x);
  Serial.print(', ');
  Serial.println(y);
  x = map(x, 0, 600, 10, 170); //scale range of values to a different range
  servo.write(x);
}

void getButton(OSCMessage &msg){  
  float val = msg.getFloat(0); 
  digitalWrite(LED_BUILTIN, val);
}
