#include <WiFi.h>
#include <Wire.h>
#include "esp_camera.h"

#define PIR_PIN 14
#define BUZZER_PIN 12
#define RELAY_PIN 13


const char* ssid = "4ac7cf";
const char* password = "260547987";
const char* smtp_server = "smtp.gmail.com";
const int smtp_port = 465; // Gmail SMTP port for SSL
const char* email_from = "olanalans12345@gmail.com";
const char* email_password = "123moviestf";
const char* email_to = "olanalans12345@gmail.com";
const char* email_subject = "Test email from ESP32";
const char* email_body = "This is a test email sent from an ESP32.";

bool motionDetected = false;

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RELAY_PIN, LOW);
  Serial.begin(9600);
}

void loop() {

  SMTPClient smtp(smtp_server, smtp_port);
  if (digitalRead(PIR_PIN) == HIGH) {
    if (!motionDetected) {
      Serial.println("Motion detected!");
      motionDetected = true;
      digitalWrite(BUZZER_PIN, HIGH);
      digitalWrite(RELAY_PIN, LOW);
      delay(1000);
      digitalWrite(BUZZER_PIN, LOW);
      digitalWrite(RELAY_PIN, HIGH);

        if (!smtp.connect()) {
    Serial.println("Failed to connect to SMTP server.");
    return;
  }
  
  // Login to the SMTP server
  if (!smtp.login(email_from, email_password)) {
    Serial.println("Failed to login to SMTP server.");
    smtp.quit();
    return;
  }
  
  // Create the email message
  EmailMessage email;
  email.setSubject(email_subject);
  email.setSender(email_from);
  email.addRecipient(email_to);
  email.setText(email_body);
  
  // Send the email
  if (!smtp.send(email)) {
    Serial.println("Failed to send email.");
    smtp.quit();
    return;
  }
  
  Serial.println("Email sent successfully.");
  smtp.quit();
    }
  } else {
    if (motionDetected) {
      Serial.println("No motion detected.");
      motionDetected = false;
    }
  }
}