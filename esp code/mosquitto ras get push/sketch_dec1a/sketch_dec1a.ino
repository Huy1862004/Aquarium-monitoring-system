#include <WiFi.h>
#include <PubSubClient.h>

// Thông tin WiFi
const char* ssid = "anhbeo";
const char* password = "35357575";

// Thông tin MQTT
const char* mqtt_server = "192.168.2.10"; // IP Raspberry
const char* subscribe_topic = "led/control";
const char* publish_topic = "sensor/data";

WiFiClient espClient;
PubSubClient client(espClient);

// Chân LED
const int ledPin = 2; // Chân D2 trên ESP32

void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0'; // Chuyển payload thành chuỗi
  String message = String((char*)payload);

  if (String(topic) == subscribe_topic) {
    if (message == "1") {
      digitalWrite(ledPin, HIGH);
    } else if (message == "0") {
      digitalWrite(ledPin, LOW);
    }
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  while (!client.connected()) {
    if (client.connect("ESP32Client")) {
      client.subscribe(subscribe_topic);
    } else {
      delay(1000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    while (!client.connect("ESP32Client")) {
      delay(1000);
    }
    client.subscribe(subscribe_topic);
  }
  client.loop();

  // Gửi dữ liệu ví dụ
  int randomNumber = random(2, 301);  // Hàm random sinh số từ 2 đến 300
  String message = String(randomNumber);  // Chuyển số ngẫu nhiên thành chuỗi
  client.publish(publish_topic, message.c_str());  // Gửi số ngẫu nhiên qua MQTT
  delay(2000);  // Chờ 2 giây trước khi gửi lại
}
