#include <Wire.h>

#define BUFFER_SIZE  4
#define MAX_NUMBER_OF_SLAVES 1
#define FIRST_SLAVE_ADDRESS 1
#define READ_CYCLE_DELAY 1000

byte buffer[BUFFER_SIZE];

void setup()
{
  Serial.begin(9600);  
  Serial.println("MASTER READER");
  Serial.println("*************");
  
  Wire.begin();        // Activate I2C link
}

void loop()
{
   
    Wire.requestFrom(FIRST_SLAVE_ADDRESS, BUFFER_SIZE);    // request data from the slave
    if(Wire.available() == BUFFER_SIZE)
      {  // if the available data size is same as I'm expecting
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
        float temperature = ((float)(short_cast.val)) / 10;

        // Parse the moisture
        short_cast.b[0] = buffer[2];
        short_cast.b[1] = buffer[3];
        float moisture =  ((float)(short_cast.val)) / 10; //short_cast.val;

        // Prints the income data
        Serial.print("Slave address ");      
        Serial.print(FIRST_SLAVE_ADDRESS);
        Serial.print(": Temprature = ");
        Serial.print(temperature);
        Serial.print("; Moisture = ");
        Serial.println(moisture);
      }
    
    Serial.println("*************************");
  
    delay(READ_CYCLE_DELAY);
  }
