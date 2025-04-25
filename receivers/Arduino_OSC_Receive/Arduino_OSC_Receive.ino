#include <WiFiNINA.h>
#include <OSCBundle.h>
#include <Servo.h>

Servo servo; 

WiFiUDP Udp;

//these are case-sensitive!!
const char WIFI_SSID[] = ""; //put WiFi network name here
const char WIFI_PASS[] = ""; //put WiFi password here

int status = WL_IDLE_STATUS;

const long PORT = 4242;

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  if (!Serial) delay(2000);
  servo.attach(A0);

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
      bundle.dispatch("/pad_1", getPad);
      bundle.dispatch("/button_1", getButton);
    }
  }
}

void getPad(OSCMessage &msg){  
  float val = msg.getFloat(0); //put argument index in here (counting from 0)
  float mapped = map(val, 0, 500, 10, 170);
  servo.write(mapped);
}

void getButton(OSCMessage &msg){  
  float val = msg.getFloat(0); //put argument index in here (counting from 0)
  digitalWrite(LED_BUILTIN, val);
}
