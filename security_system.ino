#include <WiFi.h>
#include <Wire.h>
#include "esp_camera.h"

const int PIR_PIN = 14;
const int RELAY_PIN = 12;
const int LED_PIN = 13;
const int BUZZER_PIN = 15;
const int CAMERA_XCLK_PIN = 4;
const int CAMERA_SDA_PIN = 18;
const int CAMERA_SCL_PIN = 19;
const int CAMERA_CS_PIN = 5;

boolean isMotionDetected = false;

void setup()
{
    Serial.begin(115200);
    pinMode(PIR_PIN, INPUT);
    pinMode(RELAY_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    // Initialize the camera
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = 32;
    config.pin_d1 = 35;
    config.pin_d2 = 34;
    config.pin_d3 = 5;
    config.pin_d4 = 39;
    config.pin_d5 = 18;
    config.pin_d6 = 36;
    config.pin_d7 = 19;
    config.pin_xclk = CAMERA_XCLK_PIN;
    config.pin_pclk = 22;
    config.pin_vsync = 23;
    config.pin_href = 26;
    config.pin_sscb_sda = CAMERA_SDA_PIN;
    config.pin_sscb_scl = CAMERA_SCL_PIN;
    config.pin_pwdn = -1;
    config.pin_reset = -1;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 10;
    config.fb_count = 1;

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK)
    {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }
}

void loop()
{
    if (digitalRead(PIR_PIN) == HIGH)
    {
        // Motion detected
        if (!isMotionDetected)
        {
            Serial.println("Motion detected");
            digitalWrite(RELAY_PIN, HIGH);
            digitalWrite(LED_PIN, HIGH);
            BUZZER_PIN.value(1)
                delay(1000);
            noTone(BUZZER_PIN);
            isMotionDetected = true;

            // Capture an image
            camera_fb_t *fb = esp_camera_fb_get();
            Serial.print("Image size: ");
            Serial.println(fb->len);
            esp_camera_fb_return(fb);
        }
    }
    else
    {
        // No motion detected
        if (isMotionDetected)
        {
            Serial.println("No motion detected");
            digitalWrite(RELAY_PIN, LOW);
            digitalWrite(LED_PIN, LOW);
            isMotionDetected = false;
        }
    }
    delay(100);
}