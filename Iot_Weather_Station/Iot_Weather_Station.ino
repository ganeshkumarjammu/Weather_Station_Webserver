#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Adafruit_SSD1306.h>

// --------- Constants and Pins ---------
#define DHTPIN 12
#define DHTTYPE DHT11
#define MQ2PIN 32
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// --------- Sensor Objects ---------
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;
TwoWire customWire = TwoWire(0);
//customWire.begin(4, 5);  // SDA = GPIO4, SCL = GPIO5 (as example)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &customWire, OLED_RESET);


// --------- Setup ---------
void setup() {
  Serial.begin(9600);
  dht.begin();
  customWire.begin(4, 5); 
  if (!bmp.begin()) {
    Serial.println("BMP180 not found. Check wiring!");
    while (1);
  }

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.display();
}

// --------- Loop ---------
void loop() {
  // Read DHT11
  //float dhtTemp = dht.readTemperature();
  float dhtHum = dht.readHumidity();

  // Read MQ2
  int gasValue = analogRead(MQ2PIN);

  // Read BMP180
  float bmpTemp = bmp.readTemperature();
  int32_t pressure = bmp.readPressure();
  float altitude = bmp.readAltitude();
  float realAltitude = bmp.readAltitude(102000);  // Adjust sea level pressure here if needed

  // --------- Serial Output ---------
  Serial.println("=== SENSOR DATA ===");
  //Serial.print("DHT11 Temp: "); Serial.print(dhtTemp); Serial.println(" *C");
  Serial.print("Humidity: "); Serial.print(dhtHum); Serial.println(" %");
  Serial.print("Gas Level: "); Serial.println(gasValue);

  Serial.print("BMP180 Temp: "); Serial.print(bmpTemp); Serial.println(" *C");
  Serial.print("Pressure: "); Serial.print(pressure); Serial.println(" Pa");
  Serial.print("Altitude: "); Serial.print(altitude); Serial.println(" m");
  Serial.print("Real Altitude: "); Serial.print(realAltitude); Serial.println(" m");
  Serial.println();

  // --------- OLED Display ---------
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("T1:");
  //display.print(dhtTemp); display.print("C ");
  display.print("H:"); display.print(dhtHum); display.println("%");

  display.print("Gas:");
  display.println(gasValue);

  display.print("T2:");
  display.print(bmpTemp); display.println("C");

  display.print("P:");
  display.print(pressure / 100.0); display.println(" hPa");

  display.print("Alt:");
  display.print(altitude); display.println("m");

  display.display();

  delay(1000);
}
