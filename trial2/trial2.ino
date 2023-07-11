#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "Oneplus Nord";           // Replace with your Wi-Fi SSID
const char* password = "manav123";   // Replace with your Wi-Fi password
const char* apiEndpoint = "http://192.168.240.70:5000/data";  // Replace with your Flask server IP address and port

const int dotButtonPin = D5;   // Pin for dot button
const int dashButtonPin = D6;   // Pin for dash button
const int slashButtonPin = D7; // Pin for slash button

String morseCode = "";  // Stores the concatenated morse code string
String previousCode = ""; // Stores the previous morse code string

void setup() {
  Serial.begin(9600);

  pinMode(dotButtonPin, INPUT_PULLUP);
  pinMode(dashButtonPin, INPUT_PULLUP);
  pinMode(slashButtonPin, INPUT_PULLUP);

  connectToWiFi();
}

void loop() {
  // Read button inputs
  int dotButtonState = digitalRead(dotButtonPin);
  int dashButtonState = digitalRead(dashButtonPin);
  int slashButtonState = digitalRead(slashButtonPin);

  // Check if buttons are pressed
  if (dotButtonState == LOW) {
    morseCode += ".";
    delay(200); // Delay to avoid multiple inputs from a single press
  }

  if (dashButtonState == LOW) {
    morseCode += "_";
    delay(200);
  }

  if (slashButtonState == LOW) {
    morseCode += " ";
    delay(200);
  }

  // Check if the morse code string has changed
  if (morseCode != previousCode) {
    Serial.println(morseCode);
    sendDataToAPI(morseCode);  // Send the data to Flask server
    previousCode = morseCode; // Update the previous code
  }
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connected to WiFi");
}

void sendDataToAPI(String data) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    if (http.begin(client, apiEndpoint)) {
      http.addHeader("Content-Type", "application/json");

      String payload = "{\"text\":\"" + data + "\"}";

      int httpResponseCode = http.POST(payload);

      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println("API response: " + response);
      } else {
        Serial.print("Error sending data to API. Error code: ");
        Serial.println(httpResponseCode);
      }

      http.end();
    } else {
      Serial.println("Unable to connect to API endpoint.");
    }
  } else {
    Serial.println("WiFi connection lost. Unable to send data to API.");
  }
}
