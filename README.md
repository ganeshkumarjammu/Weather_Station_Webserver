
# ESP32 Weather Station with BMP180, DHT11, and OLED

This project creates a weather station using an ESP32 microcontroller, BMP180 (pressure and temperature sensor), DHT11 (temperature and humidity sensor), and a 128x64 OLED display (SSD1306). Sensor data is displayed on the OLED and served via a mobile-optimized webpage when the ESP32 connects to a mobile hotspot.

## Table of Contents

1. [Features](https://grok.com/chat/ee2a612a-0608-4bb6-a9cd-6898cdf0fe2c#features)
2. [Hardware Requirements](https://grok.com/chat/ee2a612a-0608-4bb6-a9cd-6898cdf0fe2c#hardware-requirements)
3. [Connections](https://grok.com/chat/ee2a612a-0608-4bb6-a9cd-6898cdf0fe2c#connections)
4. [Software Requirements](https://grok.com/chat/ee2a612a-0608-4bb6-a9cd-6898cdf0fe2c#software-requirements)
5. [Library Installation](https://grok.com/chat/ee2a612a-0608-4bb6-a9cd-6898cdf0fe2c#library-installation)
6. [Arduino IDE Setup](https://grok.com/chat/ee2a612a-0608-4bb6-a9cd-6898cdf0fe2c#arduino-ide-setup)
7. [Usage](https://grok.com/chat/ee2a612a-0608-4bb6-a9cd-6898cdf0fe2c#usage)
8. [Troubleshooting](https://grok.com/chat/ee2a612a-0608-4bb6-a9cd-6898cdf0fe2c#troubleshooting)
9. [Resources and References](https://grok.com/chat/ee2a612a-0608-4bb6-a9cd-6898cdf0fe2c#resources-and-references)
10. [License](https://grok.com/chat/ee2a612a-0608-4bb6-a9cd-6898cdf0fe2c#license)

## Features

- Displays temperature (BMP180 and DHT11), humidity (DHT11), pressure (BMP180), and altitude (BMP180) on a 128x64 OLED.
- Serves a mobile-optimized webpage with real-time sensor data using Tailwind CSS.
- Connects to a mobile hotspot for web access.
- Serial output for debugging.
- Updates data every 2 seconds on both OLED and webpage.

## Hardware Requirements

- ESP32 Development Board (e.g., ESP32 DevKitC)
- BMP180 Barometric Pressure Sensor
- DHT11 Temperature and Humidity Sensor
- 128x64 OLED Display (SSD1306, I2C interface)
- Breadboard and jumper wires
- USB cable for programming
- Stable 3.3V/5V power source (recommended for reliability)

## Connections

| Component          | ESP32 Pin | Notes                                  |
| ------------------ | --------- | -------------------------------------- |
| **BMP180**         |           |                                        |
| VCC                | 3.3V      | Use 3.3V power                         |
| GND                | GND       | Common ground                          |
| SDA                | GPIO 21   | I2C data line (4.7kΩ pull-up to 3.3V)  |
| SCL                | GPIO 22   | I2C clock line (4.7kΩ pull-up to 3.3V) |
| **DHT11**          |           |                                        |
| VCC                | 5V        | Use 3.3V power                         |
| GND                | GND       | Common ground                          |
| Data               | GPIO 12   | Sensor data  pin                       |
| **OLED (SSD1306)** |           |                                        |
| VCC                | 3.3V      | Use 3.3V power                         |
| GND                | GND       | Common ground                          |
| SDA                | GPIO 21   | Shares I2C bus with BMP180             |
| SCL                | GPIO 22   | Shares I2C bus with BMP180             |

**Notes**:

- Connect 4.7kΩ pull-up resistors between SDA/SCL and 3.3V for I2C communication.
- DHT11 data pin requires a 4.7kΩ-10kΩ pull-up resistor to 3.3V.
- Ensure secure connections to avoid intermittent issues.

## Software Requirements

- **Arduino IDE**: Version 2.x or later ([Download](https://www.arduino.cc/en/software)).
- **ESP32 Board Support**: Install via Arduino IDE Boards Manager.
- **Libraries** (direct GitHub links):
    - [Adafruit BMP085 Library](https://github.com/adafruit/Adafruit-BMP085-Library) (compatible with BMP180)
    - [DHT Sensor Library](https://github.com/adafruit/DHT-sensor-library)
    - [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
    - [Adafruit SSD1306 Library](https://github.com/adafruit/Adafruit_SSD1306)
    - ESP32 `WiFi` and `WebServer` libraries (included with [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32))

## Library Installation

1. **Open Arduino IDE**:
    - Launch the Arduino IDE on your computer.
2. **Install Libraries via Library Manager**:
    - Go to `Sketch > Include Library > Manage Libraries`.
    - Search and install the following libraries:
        - `Adafruit BMP085 Library` by Adafruit
        - `DHT sensor library` by Adafruit
        - `Adafruit GFX Library` by Adafruit
        - `Adafruit SSD1306` by Adafruit
    - Alternatively, download the libraries from their GitHub repositories (linked above) and add them via `Sketch > Include Library > Add .ZIP Library`.
3. **Verify Installation**:
    - After installation, check `Sketch > Include Library` to ensure the libraries appear in the list.
4. **ESP32 Core Includes WiFi and WebServer**:
    - No separate installation is needed for `WiFi` and `WebServer` libraries if the ESP32 board support is installed.

## Arduino IDE Setup

1. **Install Arduino IDE**:
    - Download and install from [Arduino Official Website](https://www.arduino.cc/en/software).
2. **Add ESP32 Board Support**:
    - Go to `File > Preferences`.
    - Add the following URL to Additional Boards Manager URLs:
        
        ```
        https://raw.githubusercontent.com/espressif/arduino-esp32/master/package/package_esp32_index.json
        ```
        
    - Navigate to `Tools > Board > Boards Manager`, search for "ESP32", and install `esp32` by Espressif Systems.
3. **Configure WiFi**:
    - Open the `.ino` file (from the previous artifact) in Arduino IDE.
    - Replace `YourHotspotSSID` and `YourHotspotPassword` with your mobile hotspot’s SSID and password.
4. **Select Board and Port**:
    - Go to `Tools > Board > ESP32 Arduino` and select `ESP32 Dev Module` (or your specific board).
    - Select the correct port under `Tools > Port` (e.g., COMx on Windows or /dev/ttyUSBx on Linux/Mac).
5. **Upload Code**:
    - Connect the ESP32 to your computer via USB.
    - Click `Verify` to check for compilation errors.
    - Click `Upload` to flash the code.
    - If upload fails, hold the BOOT button on the ESP32 during upload to enter bootloader mode.
6. **Monitor Output**:
    - Open the Serial Monitor (`Tools > Serial Monitor`, set to 115200 baud) to view the IP address and debug output.

## Usage

1. **Power On**:
    - Connect the ESP32 to a stable power source (USB or external 3.3V/5V supply).
2. **Check Serial Monitor**:
    - View the ESP32’s IP address (e.g., `192.168.x.x`) and sensor data in the Serial Monitor.
3. **Access Webpage**:
    - Connect your mobile device to the same mobile hotspot as the ESP32.
    - Open a browser and enter the IP address from the Serial Monitor (e.g., `http://192.168.x.x`).
    - The webpage displays temperature, humidity, pressure, and altitude, updating every 2 seconds.
4. **OLED Display**:
    - The OLED shows real-time sensor data alongside the webpage.

Note:  Remove the 5V Pin of the DHT11 connected to ESP32 if OLED is not showing anything , then power on after OLED is showing messages then connect DHT11 5V Pin Jumper Wire.

## Troubleshooting

### I2C Issues

- **No Devices Detected**:
    - Run an [I2C Scanner](https://github.com/scanlime/arduino-i2c-scanner) to verify BMP180 (address: 0x77) and OLED (address: 0x3C or 0x3D).
    - Check SDA (GPIO 21) and SCL (GPIO 22) wiring. Ensure 4.7kΩ pull-up resistors are connected to 3.3V.
    - Test with shorter I2C wires (< 20 cm) to reduce noise.
- **Intermittent or Failed Communication**:
    - Verify 3.3V power stability (use an external power supply if USB is insufficient).
    - Check for loose connections or damaged wires.
    - Ensure no other devices interfere with the I2C bus.
- **Incorrect I2C Address**:
    - Some OLED modules use 0x3D instead of 0x3C. Modify the code (`display.begin(SSD1306_SWITCHCAPVCC, 0x3D)`) and test.
- **I2C Bus Lockup**:
    - Power cycle the ESP32 and sensors.
    - Add a small delay (e.g., `delay(10)`) after I2C operations if issues persist.

### General Issues

- **WiFi Connection Failure**:
    - Verify SSID and password in the code match your mobile hotspot.
    - Restart the hotspot and ESP32.
    - Check for IP conflicts (use a unique IP range in the hotspot settings).
    - Ensure the hotspot signal is strong and not overloaded.
- **Webpage Not Loading**:
    - Confirm your mobile device is on the same hotspot network as the ESP32.
    - Check the IP address in the Serial Monitor.
    - Disable mobile data to force WiFi usage.
    - Test the server by accessing `/data` (e.g., `http://192.168.x.x/data`) to verify JSON output.
- **Sensor Reading Errors**:
    - **DHT11**: Ensure the data pin (GPIO 4) has a 4.7kΩ-10kΩ pull-up resistor. Avoid heat sources or direct sunlight.
    - **BMP180**: Verify I2C connections. Recalibrate altitude using local sea-level pressure (`bmp.readSealevelPressure()`).
    - Update libraries to the latest versions via the Library Manager.
- **OLED Not Displaying**:
    - Confirm I2C address and wiring. Test with an Adafruit SSD1306 example sketch.
    - Adjust contrast with `display.setContrast()` if the display is faint.
- **Boot Errors (e.g., `RTCWDT_RTC_RESET`, `invalid header: 0xffffffff`)**:
    - **Cause**: Corrupted firmware or power issues.
    - **Solution**:
        - Re-upload the sketch, ensuring the correct board and port are selected.
        - Use a high-quality USB cable and stable power source.
        - Hold the BOOT button during upload to enter bootloader mode.
        - Update the ESP32 Arduino Core to the latest version.
- **ESP32 Crashes or Resets**:
    - Ensure sufficient power (avoid USB power limitations; use a dedicated 3.3V/5V supply).
    - Check for infinite loops or excessive delays in the code (the provided code is non-blocking).
    - Reduce I2C bus speed via `Wire.setClock(100000)` if needed.

## Resources and References

- **BMP180**:
    - [Datasheet](https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bmp180-ds000.pdf)
    - [Adafruit BMP085 Library](https://github.com/adafruit/Adafruit-BMP085-Library)
    - [Guide](https://learn.adafruit.com/using-the-bmp085-with-raspberry-pi/using-the-adafruit-bmp085-library)
- **DHT11**:
    - [Datasheet](https://www.mouser.com/datasheet/2/758/DHT11-Technical-Data-Sheet-Translated-Version-1143054.pdf)
    - [Adafruit DHT Library](https://github.com/adafruit/DHT-sensor-library)
    - [Guide](https://learn.adafruit.com/dht)
- **OLED (SSD1306)**:
    - [Datasheet](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf)
    - [Adafruit SSD1306 Library](https://github.com/adafruit/Adafruit_SSD1306)
    - [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
    - [Guide](https://learn.adafruit.com/monochrome-oled-breakouts)
- **ESP32**:
    - [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)
    - [Pinout Reference](https://randomnerdtutorials.com/esp32-pinout-reference-gpios/)
    - [Web Server Guide](https://randomnerdtutorials.com/esp32-wi-fi-web-server/)
- **Web Development**:
    - [Tailwind CSS](https://tailwindcss.com/docs) (used via CDN)
    - [ESP32 WebServer Tutorial](https://randomnerdtutorials.com/esp32-web-server-arduino-ide/)
- **I2C Debugging**:
    - [I2C Scanner](https://github.com/scanlime/arduino-i2c-scanner)
    - [I2C Troubleshooting Guide](https://learn.adafruit.com/working-with-i2c-devices)
- **ESP32 Troubleshooting**:
    - [ESP32 Boot Issues](https://randomnerdtutorials.com/solve-esp32-flash-memory-problems/)

## License

This project is licensed under the MIT License. See the [LICENSE](https://grok.com/chat/LICENSE) file for details.