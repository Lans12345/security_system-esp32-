#include <WiFi.h>
#include <Wire.h>
#include "esp_camera.h"
#include <WiFiClientSecure.h>
#include <Base64.h>
#include <ESP32Mail.h>

#define PIR_PIN 14
#define BUZZER_PIN 12
#define RELAY_PIN 13

bool motionDetected = false;

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";



// Replace with your email credentials
const char* smtp_host = "smtp.gmail.com";
const int smtp_port = 465;
const char* email_address = "olanalans12345@gmail.com";
const char* email_password = "123moviestf";

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RELAY_PIN, LOW);
  Serial.begin(9600);

 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected");

}

void loop() {
  if (digitalRead(PIR_PIN) == HIGH) {
    if (!motionDetected) {
      Serial.println("Motion detected!");
      motionDetected = true;
      digitalWrite(BUZZER_PIN, HIGH);
      digitalWrite(RELAY_PIN, LOW);
      delay(1000);
      digitalWrite(BUZZER_PIN, LOW);
      digitalWrite(RELAY_PIN, HIGH);

           // Replace with your recipient email address
        String recipient = "olanalans12345@gmail.com";
        String subject = "Hello from Arduino";
        String message = "This is a test email sent from Arduino using ESP8266.";

        if (smtpClient.send(recipient, subject, message))
        {
            Serial.println("Email sent successfully!");
        }
        else
        {
            Serial.println("Error sending email.");
        }

    }
  } else {
    if (motionDetected) {
      Serial.println("No motion detected.");
      motionDetected = false;
    }
  }
}