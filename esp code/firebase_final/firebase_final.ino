#include <WiFi.h>
#include <FirebaseESP32.h>
#include <ESP32Servo.h>
#include <OneWire.h> 
#include <DallasTemperature.h>

// Thông tin WiFi
#define WIFI_SSID "anhbeo"
#define WIFI_PASS "35357575"

#define TRIG_PIN 14  // Chân TRIG của cảm biến nối với GPIO 4 của ESP32
#define ECHO_PIN 12  // Chân ECHO của cảm biến nối với GPIO 5 của ESP32

//anh sang
int photoresistor = 25;

//nhiet do
#define ONE_WIRE_BUS 33
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

// Khai báo chân servo và góc ban đầu
#define SERVO_PIN 25
Servo myServo;

// Firebase Config và Auth
FirebaseConfig config;
FirebaseAuth auth;

// LED Pin
#define LED_PIN 2
#define bom 13

// Tạo đối tượng Firebase
FirebaseData firebaseData;

void setup() {
  // Cấu hình LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(bom, OUTPUT);
  digitalWrite(bom, LOW);
  //sieuam
  pinMode(TRIG_PIN, OUTPUT);  // Thiết lập chân TRIG là OUTPUT
  pinMode(ECHO_PIN, INPUT);   // Thiết lập chân ECHO là INPUT
  myServo.attach(SERVO_PIN);

  //anh sang
  pinMode(photoresistor,INPUT);

  //nhietdo
   sensors.begin();

  // Kết nối WiFi
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi connected!");

  // Cấu hình Firebase
  config.host = "smartcity-389da-default-rtdb.firebaseio.com";
  config.signer.tokens.legacy_token = "Y77ZiNO4GwsNG3c5PbrBaBxWeoxMdhO27jvJkrg4";

  // Kết nối Firebase
  Firebase.begin(&config, &auth);
  Serial.println("Connected to Firebase!");
}

float getDistance() {
  // Phát xung HIGH 10 micro giây trên chân TRIG
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  // Đo thời gian tín hiệu ECHO ở mức HIGH
  long duration = pulseIn(ECHO_PIN, HIGH);
  // Tính khoảng cách (cm) (tốc độ âm thanh ~ 343 m/s)
  float distance = duration * 0.034 / 2;
  return distance;
}

float duc(){
int rawValue = analogRead(34);
    Serial.println("Raw ADC Value: " + String(rawValue));
    // Giả sử giá trị tối đa của ADC là 4095 (12-bit ADC)
    float randomduc = 4095 - rawValue;  // Đảo ngược giá trị
    randomduc = randomduc / 32;        // Tính toán với số thực
    Serial.println("Calculated Do Duc: " + String(randomduc));
    return randomduc;
}

void loop() {
  // Đọc giá trị từ "congtacden"
  if (Firebase.getString(firebaseData, "/congtacden")) {
    String congtacden = firebaseData.stringData();
    Serial.println("Cong tac den: " + congtacden);
    // Điều khiển LED dựa trên giá trị
    if (congtacden == "1") {
      digitalWrite(LED_PIN, HIGH);  // Bật LED
      Serial.println("LED turned ON");
    } else {
      digitalWrite(LED_PIN, LOW);  // Tắt LED
      Serial.println("LED turned OFF");
    }
  } else {
    Serial.println("Failed to read congtacden: " + firebaseData.errorReason());
  }


  // Đọc giá trị từ "congtacden"
  if (Firebase.getString(firebaseData, "/maybom")) {
    String maybom = firebaseData.stringData();
    Serial.println("Cong tac den: " + maybom);
    // Điều khiển LED dựa trên giá trị
    if (maybom == "1") {
      digitalWrite(bom, HIGH);  // Bật LED
      Serial.println("LED turned ON");
    } else {
      digitalWrite(bom, LOW);  // Tắt LED
      Serial.println("LED turned OFF");
    }
  } else {
    Serial.println("Failed to read congtacden: " + firebaseData.errorReason());
  }

//servo
if (Firebase.getString(firebaseData, "/maychoan")) {
    String choan = firebaseData.stringData();
    Serial.println("Cong tac den: " + choan);
    // Điều khiển LED dựa trên giá trị
    if (choan == "1") {
      for (int angle = 0; angle <= 90; angle++) {
      myServo.write(angle);
      delay(15);     
   }
  }
     else {
    }
  } else {
    Serial.println("Failed to read congtacden: " + firebaseData.errorReason());
  }
  
  //gui len
  //khoang cach
  float kc = getDistance();
  Serial.print("Khoảng cách: ");
  Serial.print(kc);
  Serial.println(" cm");
  if (Firebase.setInt(firebaseData, "/sensor/Muc_nuoc", kc)) {
    Serial.println("Random value sent to Muc_nuoc: " + String(kc));
  } else {
    Serial.println("Failed to send value: " + firebaseData.errorReason());
  }

//nhiet do
  Serial.print(" Requesting temperatures..."); 
  sensors.requestTemperatures();
  Serial.println("DONE"); 
  Serial.print("Temperature is: "); 
  int nhiet = sensors.getTempCByIndex(0);
  Serial.print(nhiet);
  if (Firebase.setInt(firebaseData, "/sensor/Nhiet_do", nhiet)) {
    Serial.println("Random value sent to nhiet do: " + String(nhiet));
  } else {
    Serial.println("Failed to send value: " + firebaseData.errorReason());
  }


//anh sang
  int sang = analogRead(photoresistor);
  Serial.print("gia tri cam bien anh sang: ");
  Serial.println(sang);
  if (Firebase.setInt(firebaseData, "/sensor/Cuong_do_as", sang)) {
    Serial.println("Random value sent to danh anhsang: " + String(sang));
  } else {
    Serial.println("Failed to send value: " + firebaseData.errorReason());
  }

//doduc  
  int ducnuoc = duc();
  if (Firebase.setInt(firebaseData, "/sensor/Do_duc", ducnuoc)) {
    Serial.println("Random value sent to Muc_nuoc: " + String(ducnuoc));
  } else {
    Serial.println("Failed to send value: " + firebaseData.errorReason());
  }


  delay(1000);  // Chờ 1 giây trước khi lặp lại
}
