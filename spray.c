#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

// WiFi + Firebase
#define WIFI_SSID "IOT"
#define WIFI_PASSWORD "IOT@123456789"
#define API_KEY "AIzaSyD1mqZsTY4ikH6deXRja8aNe1AOaxcX2x4"
#define DATABASE_URL "https://drone-1fea5-default-rtdb.asia-southeast1.firebasedatabase.app"

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

//  Spray control
String status;
int pwm;

// Relay pins
#define RELAY1 D2
#define RELAY2 D5
#define RELAY3 D6
#define RELAY4 D7

// Relay states
String r1, r2, r3, r4;

void setup() {
Serial.begin(115200);

  // Spray pin
pinMode(D1, OUTPUT);

  // Relay pins
pinMode(RELAY1, OUTPUT);
pinMode(RELAY2, OUTPUT);
pinMode(RELAY3, OUTPUT);
pinMode(RELAY4, OUTPUT);

  // Default OFF
digitalWrite(RELAY1, LOW);
digitalWrite(RELAY2, LOW);
digitalWrite(RELAY3, LOW);
digitalWrite(RELAY4, LOW);
  //  WiFi connect
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
}
Serial.println("\nWiFi Connected ");

  //  Firebase setup
config.api_key = API_KEY;
config.database_url = DATABASE_URL;

Firebase.begin(&config, &auth);
Firebase.reconnectWiFi(true);

Serial.println("Firebase Ready ");
}

void loop() {

  // ==================  SPRAY CONTROL ==================
if (Firebase.RTDB.getString(&fbdo, "/spray_command/status")) {
    status = fbdo.stringData();
    Serial.print("Spray Status: ");
    Serial.println(status);
}

if (Firebase.RTDB.getInt(&fbdo, "/spray_command/pwm")) {
    pwm = fbdo.intData();
    Serial.print("PWM: ");
    Serial.println(pwm);
}

int pwm_esp = map(pwm, 0, 255, 0, 1023);

if (status == "ON") {
    analogWrite(D1, pwm_esp);
} else {
    analogWrite(D1, 0);
}

  // ==================  RELAY CONTROL ==================
if (Firebase.RTDB.getString(&fbdo, "/device1/relay1")) {
    r1 = fbdo.stringData();
}

if (Firebase.RTDB.getString(&fbdo, "/device1/relay2")) {
    r2 = fbdo.stringData();
}
if (Firebase.RTDB.getString(&fbdo, "/device1/relay3")) {
    r3 = fbdo.stringData();
}

if (Firebase.RTDB.getString(&fbdo, "/device1/relay4")) {
    r4 = fbdo.stringData();
}

  // Relay logic (change HIGH/LOW if needed)
digitalWrite(RELAY1, (r1 == "ON") ? HIGH : LOW);
digitalWrite(RELAY2, (r2 == "ON") ? HIGH : LOW);
digitalWrite(RELAY3, (r3 == "ON") ? HIGH : LOW);
digitalWrite(RELAY4, (r4 == "ON") ? HIGH : LOW);

  //  Debug
Serial.println("Relay States:");
Serial.println("R1: " + r1);
Serial.println("R2: " + r2);
Serial.println("R3: " + r3);
Serial.println("R4: " + r4);
Serial.println("----------------------");

delay(500);
}