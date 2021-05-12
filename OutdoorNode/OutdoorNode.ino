#include <Wire.h>

#include <DHT.h>
#include <DHT_U.h>

//=====================
// This is the hard-coded address. Change it from one device to another
#define SLAVE_ADDRESS 1
//=====================

// I2C Variables
#define BUFFER_SIZE 2
#define READ_CYCLE_DELAY 1000
short data[BUFFER_SIZE];

//DHT Variables
#define DHTPIN 9 
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Temprature Variables
float m_temperature;

// Moisture Variables
float m_moisture;

// General Variables
int m_timestamp;

void setup()
{
  Serial.begin(9600);  
  Serial.println("SLAVE SENDER");
  Serial.print("Node address: ");
  Serial.println(SLAVE_ADDRESS);
  Serial.print("Buffer size: ");
  Serial.println(BUFFER_SIZE * sizeof(short));
  Serial.println("***********************");

  m_timestamp = millis();
  Wire.begin(SLAVE_ADDRESS);  // Activate I2C network
  Wire.onRequest(requestEvent); // Set the request event handler

  Serial.println(F("DHTxx test!"));

  dht.begin();
  
  m_moisture = 0;
  m_temperature = 0;
}

void loop()
{ 
  if(millis() - m_timestamp < READ_CYCLE_DELAY) return;

  // Reads the temperature and humidity
  readTemp();
  
}

void readTemp(){
// Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Celsius (isFahreheit = false)

 m_temperature = t;
 m_moisture  = h; 

}

void requestEvent()
{
  data[0] = m_temperature * 10; // In order to use short, I multiple by 10
  data[1] = m_moisture * 10;
  Wire.write((byte*)data, BUFFER_SIZE * sizeof(float));  
}
