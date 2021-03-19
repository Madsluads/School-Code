//Slave
#include <Wire.h>

int anRead = 0;
byte anReadMSB = 0;
byte anReadLSB = 0;
boolean MSB = true;
int mode = 0;     //1 = temp, 2 = gas
byte pins[] = {A0, A2};

void setup()
{
  Serial.begin(9600);

  Wire.begin(8); // This command denotes which device is the master
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);

  delay(100);
}//end setup()

void loop()
{
  
  delay(500);
}//end loop()

//This function is when the Master sends data to the slave
void receiveEvent(int numBytes)
{
  mode = Wire.read();
}//end receiveEvent()

//This is when the Master requests data from the Slave
void requestEvent()
{
  if (MSB)
  {
   getInput();
    
    Wire.write(anReadMSB);
    Serial.println(anReadMSB);
    MSB = !MSB;
  }
  else
  {
    Wire.write(anReadLSB);
    Serial.println(anReadLSB);
    MSB = !MSB;
  }
}//end requestEvent()

//A funciton to avoid the wrong information being sent
void getInput()
{
  anRead = analogRead(pins[(mode - 1)]);
  Serial.print("Analog input: ");
  Serial.println(anRead);
  
  anReadMSB = anRead / 256;
  anReadLSB = anRead % 256;
}//end getInput()
