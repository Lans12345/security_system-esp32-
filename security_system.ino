#include <WiFi.h>
#include <Wire.h>
#include "esp_camera.h"
#include <SMTPClient.h>

#define PIR_PIN 14
#define BUZZER_PIN 12
#define RELAY_PIN 13

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

const char* smtpServer = "smtp.gmail.com";
const int smtpPort = 465;
const char* emailSender = "olanalans12345@gmail.com";
const char* emailRecipient = "olanalans12345@gmail.com";
const char* smtpUser = "olanalans12345@gmail.com";
const char* smtpPassword = "123moviestf";

bool motionDetected = false;

// Camera pins for NodeMCU ESP32
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM     0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM       5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// Email data
SMTPData smtpData;
String emailSubject = "Motion Detected";
String emailMessage = "Motion has been detected! See attached image.";

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

  // Connect to camera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_SVGA;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
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
  // Capture an image
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Failed to capture image");
    return;
  }

  // Set the SMTP Server Email data
  smtpData.setLogin(smtpServer, 465, smtpUser, smtpPassword);
  smtpData.setSender("ESP32", emailSender);
  smtpData.setPriority(0);
  smtpData.setSubject(emailSubject);
  smtpData.addAttachment("image.jpg", fb->buf, fb->len);
  smtpData.setMessage(emailMessage, false);
  smtpData.setRecipient(emailRecipient);
  smtpData.setSendCallback(sendCallback);

  // Send the email
  SmtpClient smtp(smtpServer, 465, smtpUser, smtpPassword, true);
  smtp.addRecipient(emailRecipient);
  smtp.setSender(emailSender);
  smtp.setSubject(emailSubject);
  smtp.setMessage(emailMessage);
  smtp.addAttachment("image.jpg", fb->buf, fb->len);
  if (!smtp.sendEmail()) {
    Serial.println("Error sending Email");
  }

  // Clear all data from the SMTP object
  smtp.empty();

  // Delete the image buffer
  esp_camera_fb_return(fb);
} } else {
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

