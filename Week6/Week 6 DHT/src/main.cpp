#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// --- Pin configuration ---
#define DHTPIN 14        // DHT22 data pin
#define DHTTYPE DHT11    // or DHT11 if needed
#define LDR_PIN 34       // LDR analog pin
#define SDA_PIN 21       // I2C SDA
#define SCL_PIN 22       // I2C SCL

// --- OLED setup ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- DHT sensor setup ---
DHT dht(DHTPIN, DHTTYPE);

// --- Setup function ---
void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 Sensor Display Starting...");

  // Initialize I2C
  Wire.begin(SDA_PIN, SCL_PIN);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Initializing...");
  display.display();

  // Initialize DHT sensor
  dht.begin();
  delay(1000);
}

// --- Main loop ---
void loop() {
  // --- Read Temperature & Humidity ---
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if DHT read failed
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error reading DHT22 sensor!");
  } else {
    // Print to Serial
    Serial.printf("Temperature: %.2f °C  |  Humidity: %.2f %%\n", temperature, humidity);

    // Display on OLED (Page 1)
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("🌡 Temp & Humidity");
    display.setTextSize(2);
    display.setCursor(0, 16);
    display.printf("%.1f C", temperature);
    display.setTextSize(1);
    display.setCursor(0, 40);
    display.printf("Humidity: %.1f %%", humidity);
    display.display();
    delay(3000);
  }

  // --- Read LDR ---
  int adcValue = analogRead(LDR_PIN);
  float voltage = (adcValue / 4095.0) * 3.3;

  // Print to Serial
  Serial.printf("LDR ADC: %d  |  Voltage: %.2f V\n", adcValue, voltage);

  // Display on OLED (Page 2)
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("💡 LDR Sensor Data");
  display.setCursor(0, 16);
  display.printf("ADC Value: %d", adcValue);
  display.setCursor(0, 32);
  display.printf("Voltage: %.2f V", voltage);
  display.display();

  delay(3000); // 3 seconds before next cycle
}
