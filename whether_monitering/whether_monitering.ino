#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <MQ135.h>

// OLED display width and height
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// OLED I2C address
#define OLED_ADDR 0x3C

// Create OLED display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Define pins for ESP32
#define DHTPIN 4            // GPIO pin for DHT11 sensor
#define DHTTYPE DHT11       // DHT11 sensor type
#define MQ135_PIN 34        // Analog pin for MQ135 (ESP32 ADC pin)
#define RAIN_SENSOR_PIN 35  // Analog pin for Rain Sensor (ESP32 ADC pin)

// Initialize sensors
DHT dht(DHTPIN, DHTTYPE);
MQ135 mq135_sensor(MQ135_PIN);

// Variables to hold sensor data
float temperature;
float humidity;
float airQuality;
int rainAnalogValue;

void setup() {
  // Start serial communication
  Serial.begin(115200);

  // Initialize DHT11 sensor
  dht.begin();

  // Initialize OLED display with I2C
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Clear the display buffer
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Print initialization message
  display.setCursor(0, 0);
  display.println(F("Weather Monitor Init"));
  display.display();
  delay(2000);
}

void loop() {
  // Read sensor data
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  airQuality = mq135_sensor.getPPM();
  rainAnalogValue = analogRead(RAIN_SENSOR_PIN);

  // Display and print temperature with status
  displayTemperature();
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");
  Serial.println(getTemperatureStatus(temperature));
  delay(3000);  // Show temperature for 3 seconds

  // Display and print humidity
  displayHumidity();
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  delay(3000);  // Show humidity for 3 seconds

  // Display and print air quality with status
  displayAirQuality();
  Serial.print("Air Quality (PPM): ");
  Serial.println(airQuality);
  Serial.println(getAirQualityStatus(airQuality));
  delay(3000);  // Show air quality for 3 seconds

  // Display and print rain sensor value with status
  displayRainValue();
  Serial.print("Rain Value: ");
  Serial.println(rainAnalogValue);
  Serial.println(getRainStatus(rainAnalogValue));
  delay(3000);  // Show rain sensor value for 3 seconds
}

// Function to display temperature with status
void displayTemperature() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(F("Temp: "));
  display.print(temperature);
  display.println(F(" C"));

  // Print temperature status
  display.setCursor(0, 16);
  display.print(getTemperatureStatus(temperature));
  display.display();
}

// Function to display humidity
void displayHumidity() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(F("Humidity: "));
  display.print(humidity);
  display.println(F(" %"));
  display.display();
}

// Function to display air quality with status
void displayAirQuality() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(F("Air Quality: "));
  display.print(airQuality);
  display.println(F(" PPM"));

  // Print air quality status
  display.setCursor(0, 16);
  display.print(getAirQualityStatus(airQuality));
  display.display();
}

// Function to display rain sensor value with status
void displayRainValue() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(F("Rain Value: "));
  display.println(rainAnalogValue);

  // Print rain detection status
  display.setCursor(0, 16);
  display.print(getRainStatus(rainAnalogValue));
  display.display();
}

// Function to get temperature status with "emoji" style feedback
String getTemperatureStatus(float temp) {
  if (temp > 35) {
    return "High Temp! :( ";   // High temperature warning
  } else if (temp < 15) {
    return "Cold Temp! :/";    // Low temperature
  } else {
    return "Comfortable :)";   // Normal temperature
  }
}

// Function to get air quality status with "emoji" style feedback
String getAirQualityStatus(float airQuality) {
  if (airQuality < 100) {
    return "Good Air :)";      // Good air quality
  } else if (airQuality < 300) {
    return "Moderate :/";      // Moderate air quality
  } else {
    return "Bad Air! :( ";     // Poor air quality
  }
}

// Function to get rain detection status
String getRainStatus(int rainValue) {
  if (rainValue > 800) {
    return "Rain Detected! :(";  // Rain detected
  } else {
    return "No Rain :)";         // No rain
  }
}
