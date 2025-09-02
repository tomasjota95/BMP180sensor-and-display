// BMP180_I2C.ino
//
// shows how to use the BMP180MI library with the sensor connected using I2C.
//
// connect the BMP180 to the Arduino like this:
// Arduino - BMC180
// 5V ------ VCC
// GND ----- GND
// SDA ----- SDA
// SCL ----- SCL

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BMP180I2C.h>
#include <Wire.h>

#define BMP180_I2C_ADDRESS 0x77
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//create an BMP180 object using the I2C interface
BMP180I2C bmp180(BMP180_I2C_ADDRESS);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //wait for serial connection to open (only necessary on some boards)
  while (!Serial);

  Wire.begin();

  //begin() initializes the interface, checks the sensor ID and reads the calibration parameters.  
  if (!bmp180.begin())
  {
    Serial.println("begin() failed. Check BMP180 Interface and I2C Address.");
    while (1);
  }

   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.setTextSize(1);
  display.setTextColor(WHITE);

  //reset sensor to default parameters.
  bmp180.resetToDefaults();

  //enable ultra high resolution mode for pressure measurements
  bmp180.setSamplingMode(BMP180MI::MODE_UHR);
}

void loop() {
  // put your main code here, to run repeatedly:

  delay(1000);
  display.clearDisplay();
  display.setCursor(0, 10);

  //start a temperature measurement
  if (!bmp180.measureTemperature())
  {
    Serial.println("could not start temperature measurement, is a measurement already running?");
    return;
  }

  //wait for the measurement to finish. proceed as soon as hasValue() returned true. 
  do
  {
    delay(100);
  } while (!bmp180.hasValue());

  //start a pressure measurement. pressure measurements depend on temperature measurement, you should only start a pressure 
  //measurement immediately after a temperature measurement. 
  if (!bmp180.measurePressure())
  {
    Serial.println("could not start pressure measurement, is a measurement already running?");
    return;
  }

  //wait for the measurement to finish. proceed as soon as hasValue() returned true. 
  do
  {
    delay(100);
  } while (!bmp180.hasValue());

  display.print("TEMP: "); 
  display.print(bmp180.getTemperature()); 
  display.print(" C\n\n");
  display.print("PRESION: "); 
  display.print(bmp180.getPressure());
  display.print(" Pa\n\n");
  display.display();

  Serial.println(bmp180.getTemperature());
  Serial.println(bmp180.getPressure());
}
