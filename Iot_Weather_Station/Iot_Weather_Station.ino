#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define DHTPIN 12
#define DHTTYPE DHT11

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_BMP085 bmp;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  
  // Initialize OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();
  
  // Initialize BMP180
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP180 sensor, check wiring!");
    while (1);
  }
  
  // Initialize DHT11
  dht.begin();
  
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  // Read sensors
  float bmpTemp = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0F; // Convert to hPa
  float altitude = bmp.readAltitude();
  float humidity = dht.readHumidity();
  float dhtTemp = dht.readTemperature();
  
  // Check if readings are valid
  if (isnan(humidity) || isnan(dhtTemp)) {
    Serial.println("Failed to read from DHT11 sensor!");
  }
  
  // Display on OLED
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Weather Station");
  display.println("---------------");
  
  display.print("BMP Temp: ");
  display.print(bmpTemp);
  display.println(" C");
  
  display.print("DHT Temp: ");
  display.print(dhtTemp);
  display.println(" C");
  
  display.print("Humidity: ");
  display.print(humidity);
  display.println(" %");
  
  display.print("Pressure: ");
  display.print(pressure);
  display.println(" hPa");
  
  display.print("Altitude: ");
  display.print(altitude);
  display.println(" m");
  
  display.display();
  
  // Serial output for debugging
  Serial.println("---------------");
  Serial.print("BMP Temp: "); Serial.print(bmpTemp); Serial.println(" C");
  Serial.print("DHT Temp: "); Serial.print(dhtTemp); Serial.println(" C");
  Serial.print("Humidity: "); Serial.print(humidity); Serial.println(" %");
  Serial.print("Pressure: "); Serial.print(pressure); Serial.println(" hPa");
  Serial.print("Altitude: "); Serial.print(altitude); Serial.println(" m");
  
  delay(2000);
}