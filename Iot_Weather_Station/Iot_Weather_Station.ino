#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP085.h>  // For BMP180
#include <DHT.h>

// WiFi credentials
const char* ssid = "G";      // Replace with your WiFi SSID
const char* password = "thinkbig";   // Replace with your WiFi password

// Web server on port 80
WebServer server(80);

// OLED configuration (I2C, 128x64)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// BMP180 (I2C)
Adafruit_BMP085 bmp;

// DHT11 configuration (Digital)
#define DHTPIN 4        // GPIO 4 for DHT11
#define DHTTYPE DHT11   // Define DHT11 sensor type
DHT dht(DHTPIN, DHTTYPE);

// Store I2C addresses for web server
String i2cAddresses = "";

void setup() {
  Serial.begin(115200);  // Initialize Serial Monitor
  Wire.begin(21, 22);    // Initialize I2C on ESP32 pins (SDA=21, SCL=22)
  
  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // OLED address typically 0x3C
    Serial.println(F("SSD1306 allocation failed"));
    while (1);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Initializing..."));
  display.display();

  // Initialize BMP180
  if (!bmp.begin()) {
    Serial.println(F("BMP180 not found. Check wiring!"));
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(F("BMP180 Error"));
    display.display();
    while (1);
  }

  // Initialize DHT11
  dht.begin();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  // Set up web server routes
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Web server started");

  // Check I2C devices
  // int deviceCount = checkI2CDevices();
  // if (deviceCount >= 2) {
  //   Serial.println("Two or more I2C devices detected. Proceeding...");
  //   display.clearDisplay();
  //   display.setCursor(0, 0);
  //   display.println(F("I2C Check: OK"));
  //   display.display();
  // } else {
  //   Serial.println("Error: Fewer than two I2C devices detected. Check wiring!");
  //   display.clearDisplay();
  //   display.setCursor(0, 0);
  //   display.println(F("I2C Error: <2 devices"));
  //   display.display();
  //   while (1);
  // }
}

void loop() {
  server.handleClient();  // Handle web server requests
  updateSensorDisplay();  // Update OLED with sensor data
  delay(2000);            // Update every 2 seconds
}

// Function to check I2C devices and return count
int checkI2CDevices() {
  int deviceCount = 0;
  i2cAddresses = "";  // Reset address string for web server
  
  Serial.println("Scanning for I2C devices...");
  for (byte address = 0x03; address <= 0x77; address++) {
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();
    
    if (error == 0) {  // Device responded (ACK)
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      i2cAddresses += "0x";
      if (address < 16) i2cAddresses += "0";
      i2cAddresses += String(address, HEX) + "<br>";
      deviceCount++;
    }
  }
  
  Serial.print("Total I2C devices detected: ");
  Serial.println(deviceCount);
  return deviceCount;
}

// Web server root page with beautiful, mobile-friendly design
void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 Sensor Dashboard</title>
  <style>
    body {
      font-family: 'Arial', sans-serif;
      background-color: #f0f4f8;
      margin: 0;
      padding: 0;
      display: flex;
      flex-direction: column;
      justify-content: space-between;
      min-height: 100vh;
    }
    .container {
      width: 90%;
      max-width: 600px;
      margin: 0 auto;
      height: 50vh;
      display: flex;
      flex-direction: column;
      justify-content: center;
      text-align: center;
      background-color: #ffffff;
      border-radius: 15px;
      box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
      padding: 20px;
    }
    h1 {
      font-size: 1.5em;
      color: #333;
      margin-bottom: 20px;
    }
    h2 {
      font-size: 1.3em;
      color: #007bff;
      margin: 10px 0;
    }
    p {
      font-size: 1.2em;
      color: #555;
      margin: 5px 0;
    }
    @media (max-width: 600px) {
      h1 { font-size: 1.3em; }
      h2 { font-size: 1.1em; }
      p { font-size: 1em; }
      .container { padding: 15px; }
    }
  </style>
</head>
<body>
  <div style="height: 25vh;"></div> <!-- Top 25% space -->
  <div class="container">
    <h1>ESP32 Sensor Dashboard</h1>
    <h2>I2C Devices</h2>
    <p>)rawliteral" + i2cAddresses + R"rawliteral(</p>
    <h2>Sensor Readings</h2>
    <p>BMP180 Temperature: )rawliteral" + String(bmp.readTemperature()) + R"rawliteral( °C</p>
    <p>BMP180 Pressure: )rawliteral" + String(bmp.readPressure() / 100.0) + R"rawliteral( hPa</p>)rawliteral";
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    html += "<p>DHT11 Error: Failed to read</p>";
  } else {
    html += "<p>DHT11 Temperature: " + String(t) + " °C</p>";
    html += "<p>DHT11 Humidity: " + String(h) + " %</p>";
  }
  html += R"rawliteral(
  </div>
  <div style="height: 25vh;"></div> <!-- Bottom 25% space -->
</body>
</html>
)rawliteral";
  server.send(200, "text/html", html);
}

// Update OLED with sensor data
void updateSensorDisplay() {
  float bmpTemp = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0;  // Convert to hPa
  float dhtTemp = dht.readTemperature();
  float humidity = dht.readHumidity();

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(F("~IoT Weather Station~"));
  display.println(F("BMP180:"));
  display.print(F("Temp: ")); display.print(bmpTemp); display.println(F(" C"));
  display.print(F("Pres: ")); display.print(pressure); display.println(F(" hPa"));
  display.println(F("DHT11:"));
  if (isnan(dhtTemp) || isnan(humidity)) {
    display.println(F("DHT11 Error"));
  } else {
    display.print(F("Temp: ")); display.print(dhtTemp); display.println(F(" C"));
    display.print(F("Hum: ")); display.print(humidity); display.println(F(" %"));
  }
  display.display();
}
