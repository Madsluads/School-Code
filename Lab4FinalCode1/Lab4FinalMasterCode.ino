//Master
#include <Wire.h>
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 6, 5, 4, 3);

//global variables
int mode = 1;     //1 for temp, 2 for gas
byte readMSB = 0;
byte readLSB = 0;
int anRead = 0;
float temp = 0;

void setup()
{
  Serial.begin(9600);
  Wire.begin(); // This command denotes which device is the master
  delay(100);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  //pushbutton
  attachInterrupt(0, changeMode, FALLING);
}//end setup()

void loop()
{
  updateSlave();
  if (mode == 1)
  {
    readWire();
    convertTemp();

    lcd.setCursor(0, 0);
    lcd.print("Temperature (C):");
    clearLine(1);
    lcd.setCursor(0, 1);
    lcd.print("     ");
    lcd.print(temp);
  }
  if (mode == 2)
  {
    readWire();
    lcd.setCursor(0, 0);
    lcd.print("      Gas");
    checkGas();
  }

  delay(1000);
}//end loop()

void convertTemp()
{
  float volt = anRead * (5.0 / 1023.0);   //convert to voltage
  volt = volt * 1000;                     //convert to milivolts
  temp = (volt - 500) / 10;
}//end convertTemp

void checkGas()
{
  //approximate range: 487 - 868
  //programmed range: 480 - 870
  //range splits: 480 - 610, 611 - 740, 741 - 870

  if (anRead < 611)
  {
    clearLine(1);
    lcd.setCursor(0, 1);
    lcd.print("     Normal");
  }
  else if ((anRead >= 611) && (anRead <= 740))
  {
    clearLine(1);
    lcd.setCursor(0, 1);
    lcd.print("     Smoke");
  }
  else if (anRead > 740)
  {
    clearLine(1);
    lcd.setCursor(0, 1);
    lcd.print("      Fire");
  }
}

void updateSlave()
{
  Wire.beginTransmission(8);
  Wire.write(mode);
  Wire.endTransmission();
}//end updateSlave()

void readWire()
{
  Wire.requestFrom(8, 1);
  readMSB = Wire.read();

  Wire.requestFrom(8, 1);
  readLSB = Wire.read();

  anRead = (readMSB * 256) + readLSB;
}//end readWire()

void changeMode()
{
  if (mode == 1)
  {
    clearLine(0);
    mode = 2;
  }
  else if (mode == 2)
  {
    clearLine(0);
    mode = 1;
  }
}//end changeMode()

void clearLine(int line)
{
  lcd.setCursor(0, line);
  lcd.print("                 ");
}//end clearLine()
