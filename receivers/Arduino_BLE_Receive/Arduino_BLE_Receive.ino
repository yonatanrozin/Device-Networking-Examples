/*
  Simple Arduino BLE Receiver
  Yonatan Rozin

  Create a public BLE device name (any)
  Create your own service + characteristic UUIDs
  Create <datatype>characteristics from UUIDs in setup()
  
  Check for characteristic updates in loop(), do stuff with the new values

  Run sketch, check serial monitor for Arduino's local IP address
  Use the IP address and port # to send UDP to this Arduino
*/

#include <ArduinoBLE.h> //not included with IDE - install in libary manager

#include <Servo.h>

#define servoPin 4

char BLEDeviceName[] = ""; //give your device a name to appear in public interfaces (or not)

//generate a v4 UUID here: https://www.uuidgenerator.net/version4 and enter below
char BLEServiceUUID[] = "12345678-1234-abcd-1234-12345678abcd";

//create characteristic UUIDs - one per characteristic
//for each: use the service UUID above, modify by 1 character
char BLECharacteristic1UUID[] = "22345678-1234-abcd-1234-12345678abcd";
char BLECharacteristic2UUID[] = "32345678-1234-abcd-1234-12345678abcd";


//create 1 ble service
BLEService ble(BLEServiceUUID); 

//create BLE characteristics - remove read/write permissions as needed
//recommended: use BLEWriteWithoutResponse for much faster writing!
BLEByteCharacteristic characteristic1(BLECharacteristic1UUID, BLERead | BLENotify | BLEWriteWithoutResponse); 
BLEFloatCharacteristic characteristic2(BLECharacteristic2UUID, BLERead | BLENotify | BLEWriteWithoutResponse); 
// BLEIntCharacteristic, BLEFloatCharacteristic or BLEByteCharacteristic (for booleans)

Servo servo;

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  servo.attach(servoPin);

  Serial.begin(9600);
  if (!Serial) delay(2000);
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    while (1);
  }


  BLE.setLocalName(BLEDeviceName); 

  BLE.setAdvertisedService(ble);

  ble.addCharacteristic(characteristic1);
  ble.addCharacteristic(characteristic2);
  //repeat for all characteristics

  characteristic1.writeValue(0);

  BLE.addService(ble);
  BLE.advertise();

  Serial.println("Bluetooth® device active, waiting for connections...");
  
}

void loop(){ 
  BLE.poll(); //check for ble events

  //check characteristic for new value
  if (characteristic1.written()) {
    bool val = characteristic1.value();
    digitalWrite(LED_BUILTIN, val);
  }
  
  if (characteristic2.written()) {
    float val = characteristic2.value(); 
    //assuming an input between 0 and 1
    float servoVal = val * 180;
    servo.write(servoVal);
  }

  delay (10);
}

