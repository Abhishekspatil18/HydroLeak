// -------------------------------
// ✅ Blynk Device Info
#define BLYNK_TEMPLATE_ID "TMPL3FOJXO7wB"
#define BLYNK_TEMPLATE_NAME "water leakage monitoring system"
#define BLYNK_AUTH_TOKEN "tfWuQfOOzog7yqQuQ9kzETK1b4gJezIC"
// -------------------------------

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <TinyGPS++.h>
#include <ESP_Mail_Client.h>

// -------------------------------
// Wi-Fi Credentials
char ssid[] = "Tatti";
char pass[] = "8180983906";

// -------------------------------
// Gmail SMTP Info
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL "mahawatermonitoring18@gmail.com"        // Sender Gmail
#define AUTHOR_PASSWORD "rtoxdspcmiqummrp"       // App password (16 chars)
#define RECIPIENT_EMAIL "abhishekspatil1805@gmail.com" // Where to send alert

SMTPSession smtp;
ESP_Mail_Session session;
SMTP_Message message;

// -------------------------------
// GPS Setup
TinyGPSPlus gps;
#define GPS_RX_PIN 16
#define GPS_TX_PIN 17
#define GPS_BAUD 9600

// -------------------------------
// Flow Sensors + Actuators
#define RELAY 5
#define BUZZER 12
#define SENSOR1 25
#define SENSOR2 26

// Flow Sensor 1 Variables
long currentMillis1 = 0;
long previousMillis1 = 0;
int interval1 = 1000;
float calibrationFactor1 = 6;
volatile byte pulseCount1;
byte pulse1Sec1 = 0;
float flowRate1;
unsigned int flowMilliLitres1;
unsigned long totalMilliLitres1;

void IRAM_ATTR pulseCounter1() { pulseCount1++; }

// Flow Sensor 2 Variables
long currentMillis2 = 0;
long previousMillis2 = 0;
int interval2 = 1000;
float calibrationFactor2 = 6;
volatile byte pulseCount2;
byte pulse1Sec2 = 0;
float flowRate2;
unsigned int flowMilliLitres2;
unsigned long totalMilliLitres2;

void IRAM_ATTR pulseCounter2() { pulseCount2++; }

// -------------------------------
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("🚀 IoT Water Leakage + GPS System");

  // Initialize Wi-Fi + Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Initialize GPS
  Serial2.begin(GPS_BAUD, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);

  // Pins
  pinMode(SENSOR1, INPUT_PULLUP);
  pinMode(SENSOR2, INPUT_PULLUP);
  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(SENSOR1), pulseCounter1, FALLING);
  attachInterrupt(digitalPinToInterrupt(SENSOR2), pulseCounter2, FALLING);

  // SMTP Gmail setup
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.time.ntp_server = "pool.ntp.org";
  session.time.gmt_offset = 330;  // India +5:30
  session.time.day_light_offset = 0;
}

// -------------------------------
// Function to send Gmail alert
void sendLeakageEmail(float lat, float lon) {
  if (!smtp.connect(&session)) {
    Serial.println("❌ SMTP connection failed.");
    return;
  }

  String mapLink = "https://www.google.com/maps?q=" + String(lat, 6) + "," + String(lon, 6);
  String messageText = "🚨 Water Leakage Detected!\nLocation: " + mapLink;

  message.sender.name = "ESP32 Water System";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "⚠ Water Leakage Alert!";
  message.addRecipient("Owner", RECIPIENT_EMAIL);
  message.text.content = messageText.c_str();

  if (MailClient.sendMail(&smtp, &message)) {
    Serial.println("✅ Email sent successfully!");
  } else {
    Serial.println("❌ Email sending failed!");
  }
  smtp.closeSession();
}

// -------------------------------
void loop() {
  Blynk.run();

  // Read GPS continuously
  while (Serial2.available() > 0) {
    gps.encode(Serial2.read());
  }

  // -------------------------------
  // Flow Sensor 1
  currentMillis1 = millis();
  if (currentMillis1 - previousMillis1 > interval1) {
    pulse1Sec1 = pulseCount1;
    pulseCount1 = 0;
    flowRate1 = ((1000.0 / (currentMillis1 - previousMillis1)) * pulse1Sec1) / calibrationFactor1;
    previousMillis1 = currentMillis1;
    flowMilliLitres1 = (flowRate1 / 60) * 1000;
    totalMilliLitres1 += flowMilliLitres1;

    Serial.print("Flow rate1: ");
    Serial.print(int(flowRate1));
    Serial.println(" mL/S");

    Blynk.virtualWrite(V0, int(flowRate1));
  }

  // Flow Sensor 2
  currentMillis2 = millis();
  if (currentMillis2 - previousMillis2 > interval2) {
    pulse1Sec2 = pulseCount2;
    pulseCount2 = 0;
    flowRate2 = ((1000.0 / (currentMillis2 - previousMillis2)) * pulse1Sec2) / calibrationFactor2;
    previousMillis2 = currentMillis2;
    flowMilliLitres2 = (flowRate2 / 60) * 1000;
    totalMilliLitres2 += flowMilliLitres2;

    Serial.print("Flow rate2: ");
    Serial.print(int(flowRate2));
    Serial.println(" mL/S");

    Blynk.virtualWrite(V1, int(flowRate2));
  }

  // -------------------------------
  // Leakage Detection
  if (flowRate2 < flowRate1 && flowRate2 < 8) {
    Serial.println("⚠ Water Leakage Detected!");
    digitalWrite(RELAY, HIGH);
    digitalWrite(BUZZER, HIGH);
    Blynk.logEvent("flow_notify", "🚨 Water Leakage Detected!");

    // Send Gmail alert if GPS fix is available
    if (gps.location.isValid()) {
      sendLeakageEmail(gps.location.lat(), gps.location.lng());
    } else {
      Serial.println("⚠ GPS location not available yet.");
    }
    delay(5000); // Optional delay to avoid sending multiple emails too quickly
  } else {
    digitalWrite(RELAY, LOW);
    digitalWrite(BUZZER, LOW);
  }

  delay(200); // Small loop delay
}