#include <WiFi.h>
#include <Wire.h>
#include "esp_camera.h"
#include <ESP32_MailClient.h>

#define PIR_PIN 14
#define BUZZER_PIN 12
#define RELAY_PIN 13

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

const char* smtpServer = "smtp.gmail.com";
const char* emailSender = "olanalans12345@gmail.com";
const char* emailRecipient = "olanalans12345@gmail.com";
const char* smtpUser = "olanalans12345@gmail.com";
const char* smtpPassword = "123moviestf";

bool motionDetected = false;

ESP32_MailClient MailClient;
SMTPData smtpData;
const char* emailSubject = "Motion detected!";
const char* emailMessage = "Motion has been detected by NodeMCU ESP32.";

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RELAY_PIN, LOW);
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");

  // Set the SMTP Server Email data
  smtpData.setLogin(smtpServer, 465, smtpUser, smtpPassword);
  smtpData.setSender("ESP32", emailSender);
  smtpData.setPriority(0);
  smtpData.setSubject(emailSubject);
  smtpData.setMessage(emailMessage, false);
  smtpData.setRecipient(emailRecipient);
  smtpData.setSendCallback(sendCallback);
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

      // Send the email
      if (!MailClient.sendMail(smtpData)) {
        Serial.println("Error sending Email, " + MailClient.smtpErrorReason());
      }

      // Clear all data from the SMTP object
      smtpData.empty();
    }
  } else {
    if (motionDetected) {
      Serial.println("No motion detected.");
      motionDetected = false;
    }
  }
}

void sendCallback(SendStatus msg) {
  // Print the status of the email sending
  Serial.println(msg.info());
}
