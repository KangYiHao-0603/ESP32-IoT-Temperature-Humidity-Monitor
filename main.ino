 ESP32 connects to Wi-Fi and displays IP address
- User accesses web interface via mobile browser
- Real-time temperature and humidity monitoring
- Remote LED control
- AUTO/MANUAL mode switching
#include <WiFi.h>
#include <WebServer.h>
#include <SimpleDHT.h>

// Global variables

// Hardware pin configuration
int pinDHT11 = 15;   // DHT11 data pin
int LED_PIN = 2;     // LED control pin

// Wi-Fi credentials
// String literals are stored in Flash memory
// Only pointers are stored in RAM to save memory
const char* ssid = "9487";
const char* password = "11111111";

// Sensor object
SimpleDHT11 dht11;

// HTTP server running on port 80
WebServer server(80);

// Sensor data
// DHT11 range fits in 1 byte (0–255)
byte temperature = 0;
byte humidity = 0;

// System state variables
bool ledState = false;     // LED ON/OFF state
bool autoMode = true;      // true = auto mode, false = manual mode
int tempThreshold = 30;    // Temperature threshold for auto control

// Update LED output based on current state
void updateLED() {
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
}

// Initialize and connect to Wi-Fi
bool connectWiFi() {
  WiFi.mode(WIFI_STA);

  // Clear previous Wi-Fi configuration
  WiFi.disconnect(true, true);
  delay(1000);

  Serial.println("Connecting to WiFi...");
  Serial.print("SSID: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  // Retry connection up to 30 times
  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 30) {
    delay(500);
    Serial.print(".");
    retry++;
  }

  Serial.println();
  Serial.print("WiFi status code: ");
  Serial.println(WiFi.status());

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    return true;
  } else {
    Serial.println("WiFi connect failed!");
    return false;
  }
}

// Read temperature and humidity from DHT11
void readDHT() {
  byte temp = 0;
  byte hum = 0;

  int err = dht11.read(pinDHT11, &temp, &hum, NULL);

  if (err != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err=");
    Serial.println(err);
    return;
  }

  // Update global sensor values
  temperature = temp;
  humidity = hum;

  Serial.println("=================================");
  Serial.print("Humidity = ");
  Serial.print((int)humidity);
  Serial.print("% , Temperature = ");
  Serial.print((int)temperature);
  Serial.println("C");

  Serial.print("Mode = ");
  Serial.println(autoMode ? "AUTO" : "MANUAL");

  // Auto control logic
  if (autoMode) {
    ledState = (temperature >= tempThreshold);
    updateLED();
  }

  Serial.print("LED = ");
  Serial.println(ledState ? "ON" : "OFF");
}

// Handle root URL request
// Generate HTML page dynamically
void handleRoot() {
  String html = "<!DOCTYPE html><html><head>";

  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";

  // Auto refresh every 5 seconds
  html += "<meta http-equiv='refresh' content='5'>";

  html += "<title>ESP32 Monitor</title></head><body>";

  html += "<h1>ESP32 Temperature and Humidity Monitor</h1>";

  // Display sensor data
  html += "<p>Temperature: " + String((int)temperature) + " C</p>";
  html += "<p>Humidity: " + String((int)humidity) + " %</p>";

  // Display system status
  html += "<p>LED: " + String(ledState ? "ON" : "OFF") + "</p>";
  html += "<p>Mode: " + String(autoMode ? "AUTO" : "MANUAL") + "</p>";
  html += "<p>Threshold: " + String(tempThreshold) + " C</p>";

  // Control buttons using HTTP requests
  html += "<p><a href='/led/on'><button>LED ON</button></a></p>";
  html += "<p><a href='/led/off'><button>LED OFF</button></a></p>";
  html += "<p><a href='/mode/auto'><button>AUTO MODE</button></a></p>";
  html += "<p><a href='/mode/manual'><button>MANUAL MODE</button></a></p>";

  html += "</body></html>";

  server.send(200, "text/html", html);
}

// Turn LED ON in manual mode
void handleLedOn() {
  autoMode = false;
  ledState = true;
  updateLED();

  server.sendHeader("Location", "/");
  server.send(303);
}

// Turn LED OFF in manual mode
void handleLedOff() {
  autoMode = false;
  ledState = false;
  updateLED();

  server.sendHeader("Location", "/");
  server.send(303);
}

// Switch to auto mode
void handleModeAuto() {
  autoMode = true;

  // Update LED immediately based on temperature
  ledState = (temperature >= tempThreshold);
  updateLED();

  server.sendHeader("Location", "/");
  server.send(303);
}

// Switch to manual mode
void handleModeManual() {
  autoMode = false;

  server.sendHeader("Location", "/");
  server.send(303);
}

// Setup function runs once at startup
void setup() {
  Serial.begin(115200);
  delay(1000);

  // Initialize LED pin
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Connect to Wi-Fi
  if (!connectWiFi()) {
    Serial.println("Stop here because WiFi failed.");
    return;
  }

  // Initial sensor reading
  readDHT();

  // Register HTTP routes
  server.on("/", handleRoot);
  server.on("/led/on", handleLedOn);
  server.on("/led/off", handleLedOff);
  server.on("/mode/auto", handleModeAuto);
  server.on("/mode/manual", handleModeManual);

  // Start web server
  server.begin();
  Serial.println("Web server started.");
}

// Main loop runs continuously
void loop() {
  // Handle incoming HTTP requests
  if (WiFi.status() == WL_CONNECTED) {
    server.handleClient();
  }

  // Non-blocking timer for periodic sensor reading
  static unsigned long lastReadTime = 0;
  unsigned long now = millis();

  if (now - lastReadTime >= 3000) {
    lastReadTime = now;
    readDHT();
  }
}
