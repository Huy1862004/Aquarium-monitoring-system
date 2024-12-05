#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Thông tin WiFi
const char* ssid = "anhbeo";
const char* password = "35357575";

// Thông tin MQTT
const char* mqtt_server = "192.168.2.10"; // IP Raspberry
const char* topic_congtacden = "device/led";
const char* topic_maybom = "device/bom";
const char* topic_maychoan = "device/servo";
const char* topic_sensor_mucnuoc = "sensor/mucnuoc";
const char* topic_sensor_nhietdo = "sensor/nhietdo";
const char* topic_sensor_cuongdoas = "sensor/cuongdoas";
const char* topic_sensor_doduc = "sensor/doduc";

WiFiClient espClient;
PubSubClient client(espClient);

// Chân LED, máy bơm, cảm biến và servo
#define TRIG_PIN 14
#define ECHO_PIN 12
#define SERVO_PIN 25
#define LED_PIN 2
#define BOM_PIN 13

Servo myServo;
// Cảm biến nhiệt độ
#define ONE_WIRE_BUS 33
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Cảm biến ánh sáng
int photoresistor = 34;

void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0'; // Chuyển payload thành chuỗi

  String message = String((char*)payload);

  // Điều khiển đèn
  if (String(topic) == topic_congtacden) {
    digitalWrite(LED_PIN, message == "1" ? HIGH : LOW);
  }


  // Điều khiển máy bơm
  if (String(topic) == topic_maybom) {
    digitalWrite(BOM_PIN, message == "1" ? HIGH : LOW);
  }
  // Điều khiển cho cá ăn
  if (String(topic) == topic_maychoan && message == "1") {
    for (int angle = 0; angle <= 90; angle++) {
      myServo.write(angle);
      delay(15);
    }
  }
}

void setup() {
  Serial.begin(9600);


  // Cấu hình chân đầu vào/đầu ra
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(BOM_PIN, OUTPUT);
  digitalWrite(BOM_PIN, LOW);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  myServo.attach(SERVO_PIN);
  // Khởi tạo cảm biến nhiệt độ
  sensors.begin();

  // Kết nối WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");

  // Kết nối MQTT
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  while (!client.connected()) {
    if (client.connect("ESP32Client")) {
      client.subscribe(topic_congtacden);
      client.subscribe(topic_maybom);
      client.subscribe(topic_maychoan);
    } else {
      delay(1000);
    }
  }
}

float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2;
}

float getSoilMoisture() {
  int rawValue = analogRead(34);
  return (4095 - rawValue) / 32.0;
}

void loop() {
  if (!client.connected()) {
    while (!client.connect("ESP32Client")) {
      delay(500);
    }
    client.subscribe(topic_congtacden);
    client.subscribe(topic_maybom);
    client.subscribe(topic_maychoan);
  }
  client.loop();

  // Đọc dữ liệu từ cảm biến và gửi qua lên rasp
  float mucNuoc = getDistance();
  client.publish(topic_sensor_mucnuoc, String(mucNuoc).c_str());



  sensors.requestTemperatures();
  float nhietDo = sensors.getTempCByIndex(0);
  client.publish(topic_sensor_nhietdo, String(nhietDo).c_str());

  //cuong do as
  int cuongDoAS = analogRead(photoresistor) ;
  client.publish(topic_sensor_cuongdoas, String(cuongDoAS).c_str());

  //doduc
  float doDuc = getSoilMoisture();
  client.publish(topic_sensor_doduc, String(doDuc).c_str());

  delay(500);
}
