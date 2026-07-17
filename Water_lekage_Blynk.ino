// -------------------------------
// Blynk Device Info
#define BLYNK_TEMPLATE_ID "TMPL3FOJXO7wB"
#define BLYNK_TEMPLATE_NAME "water leakage monitoring system"
#define BLYNK_AUTH_TOKEN "tfWuQfOOzog7yqQuQ9kzETK1b4gJezIC"
// -------------------------------

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

// -------------------------------
// GPS Setup
TinyGPSPlus gps;
HardwareSerial SerialGPS(1); // UART1
#define RXD2 16  // GPS TX → ESP32 RX
#define TXD2 17  // GPS RX → ESP32 TX (optional)
// -------------------------------

#define RELAY 5
#define SENSOR1 25
#define SENSOR2 26

char ssid[] = "Tatti";
char pass[] = "8180983906";

long currentMillis1 = 0;
long previousMillis1 = 0;
int interval1 = 1000;
float calibrationFactor1 = 6;
volatile byte pulseCount1;  
byte pulse1Sec1 = 0;
float flowRate1;
unsigned int flowMilliLitres1;
unsigned long totalMilliLitres1;

void IRAM_ATTR pulseCounter1() {
  pulseCount1++;
}

long currentMillis2 = 0;
long previousMillis2 = 0;
int interval2 = 1000;
float calibrationFactor2 = 6;
volatile byte pulseCount2;
byte pulse1Sec2 = 0;
float flowRate2;
unsigned int flowMilliLitres2;
unsigned long totalMilliLitres2;

void IRAM_ATTR pulseCounter2() {
  pulseCount2++;
}

void setup() {
  Serial.begin(115200);
  SerialGPS.begin(9600, SERIAL_8N1, RXD2, TXD2); // GPS baud rate

  pinMode(SENSOR1, INPUT_PULLUP);
  pinMode(SENSOR2, INPUT_PULLUP);
  pinMode(RELAY, OUTPUT);
  pinMode(12, OUTPUT);

  pulseCount1 = 0; flowRate1 = 0; flowMilliLitres1 = 0; totalMilliLitres1 = 0; previousMillis1 = 0;
  pulseCount2 = 0; flowRate2 = 0; flowMilliLitres2 = 0; totalMilliLitres2 = 0; previousMillis2 = 0;

  attachInterrupt(digitalPinToInterrupt(SENSOR1), pulseCounter1, FALLING);
  attachInterrupt(digitalPinToInterrupt(SENSOR2), pulseCounter2, FALLING);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.println("System Initialized...");
}

void loop() {
  Blynk.run();

  // GPS update
  while (SerialGPS.available() > 0) {
    gps.encode(SerialGPS.read());
  }

  // Flow Sensor 1
  currentMillis1 = millis();
  if (currentMillis1 - previousMillis1 > interval1) {
    pulse1Sec1 = pulseCount1; pulseCount1 = 0;
    flowRate1 = ((1000.0 / (millis() - previousMillis1)) * pulse1Sec1) / calibrationFactor1;
    previousMillis1 = millis();
    flowMilliLitres1 = (flowRate1 / 60) * 1000;
    totalMilliLitres1 += flowMilliLitres1;

    Serial.print("Flow rate1: "); Serial.print(int(flowRate1)); Serial.println(" mL/S");
    Blynk.virtualWrite(V0, int(flowRate1));
  }

  // Flow Sensor 2
  currentMillis2 = millis();
  if (currentMillis2 - previousMillis2 > interval2) {
    pulse1Sec2 = pulseCount2; pulseCount2 = 0;
    flowRate2 = ((1000.0 / (millis() - previousMillis2)) * pulse1Sec2) / calibrationFactor2;
    previousMillis2 = millis();
    flowMilliLitres2 = (flowRate2 / 60) * 1000;
    totalMilliLitres2 += flowMilliLitres2;

    Serial.print("Flow rate2: "); Serial.print(int(flowRate2)); Serial.println(" mL/S");
    Blynk.virtualWrite(V1, int(flowRate2));
  }

  // Leakage Detection
  if (flowRate2 < flowRate1 && flowRate2 < 8) {
    digitalWrite(RELAY, HIGH);
    digitalWrite(12, HIGH);

    if (gps.location.isValid()) {
      String mapLink = "https://www.google.com/maps?q=" + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
      String message = "Water Leakage Detected!\n" + mapLink;
      Blynk.logEvent("flow_notify", message);
      Serial.println("Leakage detected!");
      Serial.println(message);
    } else {
      Blynk.logEvent("flow_notify", "Water Leakage Detected! (No GPS fix yet)");
      Serial.println("Leakage detected (no GPS fix yet)");
    }
    delay(5000);
  } else {
    digitalWrite(RELAY, LOW);
    digitalWrite(12, LOW);
  }
}
