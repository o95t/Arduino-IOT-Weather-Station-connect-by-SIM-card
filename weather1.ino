



#include <Adafruit_BMP085.h>
#include <DHT.h>
#include <DHT_U.h>


#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

#include <Wire.h>


/*************************************************** 
  This is an example for the BMP085 Barometric Pressure & Temp Sensor

  Designed specifically to work with the Adafruit BMP085 Breakout 
  ----> https://www.adafruit.com/products/391

  These displays use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

// Connect VCC of the BMP085 sensor to 3.3V (NOT 5.0V!)
// Connect GND to Ground
// Connect SCL to i2c clock - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 5
// Connect SDA to i2c data - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 4
// EOC is not used, it signifies an end of conversion
// XCLR is a reset pin, also not used here

Adafruit_BMP085 bmp;
String endPoint, temp, humidity, barometer,endPointUrl;  
void setup() {
Serial1.begin(115200);  //for GSM module 
Serial.begin(9600);  

Serial.println("DHT11 test!");

dht.begin();

delay(10000);
 
Serial1.println("AT");//start connection return OK
delay(6000);
printSerialData();//print response from Serial1

// Start bmp 
if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  while (1) {}
  } 
//for api request 
  endPoint = String("GET /weatherstation/updateweatherstation.php?ID=kgnwa840&PASSWORD=8953524&softwaretype=HP1000%20V2.7.3&action=updateraw&realtime=1&rtfreq=5");
  temp = String("&outtemp=");
  humidity = String("&outhumi=");
  barometer = String("&absbaro=");
  endPointUrl = String();
}

// the loop function runs over and over again forever
void loop() {
    readTempAndSendToServer();
  
  delay(3600000);
  

}

void readTempAndSendToServer(){
  // Wait a few seconds between measurements.
  delay(4000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  // float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  // float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  // float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
//  Serial.print(f);
//  Serial.print(" *F\t");
//  Serial.print("Heat index: ");
//  Serial.print(hic);
//  Serial.print(" *C ");
//  Serial.print(hif);
//  Serial.println(" *F");  

  double pressure =0;
  pressure = bmp.readPressure();
  Serial.print("Pressure: ");
  Serial.println(pressure);
  
 Serial1.println("AT");
delay(6000);
printSerialData();
Serial1.println("AT+CGATT=1");//check if conniction done OK
delay(3000);
printSerialData();
Serial1.println("AT+CSTT=\"zain\",\"\",\"\"");
delay(4000);
printSerialData();
delay(7000);
Serial1.println("AT+CIICR");//GPRS ON 
delay(7000);
printSerialData();
//Serial1.println("AT+CIFSR");
//delay(3000);
//printSerialData();
Serial1.println("AT+CIPSTART=\"TCP\",\"52.19.127.58\",30000");//connection on server thinkspeak
delay(2000);
printSerialData();
Serial1.println("AT+CIPSEND=44");//send request 
delay(2000);
printSerialData();

  endPointUrl = endPoint + temp + t + humidity + h + barometer + pressure;
  
  Serial1.println(endPointUrl);
delay(2000);
printSerialData();

}
 

void printSerialData()
{
while(Serial1.available()!=0)
Serial.write(Serial1.read());
}
