#include "arduino_secrets.h"

#include <Wire.h>
/*
  Sketch generated by the Arduino IoT Cloud Thing "Weatherstation"
  https://create.arduino.cc/cloud/things/aa9b7bf5-1d5a-49e6-b3cf-49497f6e0310

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  float temperature;
  float humidity;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/
#include "thingProperties.h"

//IC2 variables
#define BUFFER_SIZE  4
#define MAX_NUMBER_OF_SLAVES 1
#define FIRST_SLAVE_ADDRESS 1
#define READ_CYCLE_DELAY 1800000

byte buffer[BUFFER_SIZE];

bool i2cPeripheralsEnabled = false;


void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500);



  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
  */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

// arduino IoT setup code and anything else
  // init properties
  // Arduino IoT Cloud begin

  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::CONNECT, onIoTConnect);
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::DISCONNECT, onIoTDisconnect);
  // you can also subscribe to a SYNC event which will be triggered when data is synced between board and cloud
  // any other setup code
  // do NOT initiate begin for your other i2c peripherals
  // at this point the ECCx08 library might scramble the bus to wake the device up

  Serial.println("MASTER READER");
  Serial.println("*************");

  //Wire.begin();        // Activate I2C link
}

void loop() {
  Serial.println("********NEW LOOP***********");
  ArduinoCloud.update();
  if(i2cPeripheralsEnabled){
    IC2Recive();
  }
  //UpdateData();
  

}

void IC2Recive() {

  Wire.requestFrom(FIRST_SLAVE_ADDRESS, BUFFER_SIZE);    // request data from the slave
  if (Wire.available() /*== BUFFER_SIZE*/)
  { 
    Serial.println("got into loop");
    // if the available data size is same as I'm expecting
    // Reads the buffer the slave sent
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
      buffer[i] = Wire.read();  // gets the data
    }

    // Parse the buffer
    // In order to convert the incoming bytes info short, I use union
    union short_tag {
      byte b[2];
      short val;
    } short_cast;

    // Parse the temperature
    short_cast.b[0] = buffer[0];
    short_cast.b[1] = buffer[1];
    float temperatureIc = ((float)(short_cast.val)) / 10;

    // Parse the moisture
    short_cast.b[0] = buffer[2];
    short_cast.b[1] = buffer[3];
    float moistureIc =  ((float)(short_cast.val)) / 10; //short_cast.val;

    // Prints the income data
    Serial.print("Slave address ");
    Serial.print(FIRST_SLAVE_ADDRESS);
    Serial.print(": Temprature = ");
    Serial.print(temperatureIc);
    Serial.print("; Moisture = ");
    Serial.println(moistureIc);

    temperature = temperatureIc;
    humidity = moistureIc;
    
  }
  else{
    Serial.println("Failed to go into loop");
  }

  Serial.println("*************************");

  delay(READ_CYCLE_DELAY);

}

void onIoTConnect(){
  // enable your other i2c devices
  Serial.println(">>> connected to Arduino IoT Cloud");
  Serial.println("enabling other i2c devices");
  // run the setup code (begin or else) for your other i2c devices
  Wire.begin();
  i2cPeripheralsEnabled = true;
}
void onIoTDisconnect(){
  // disable your other i2c devices
  Serial.println(">>> disconnected to Arduino IoT Cloud");
  Serial.println("disabling other i2c devices");
  // if necessary call the end() method on your i2c device library or even get rid of them.
  // really it's on a peripheral basis
  i2cPeripheralsEnabled = false;
}







