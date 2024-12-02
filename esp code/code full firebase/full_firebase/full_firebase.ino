#include <WiFi.h>
#include <FirebaseESP32.h>
#include <OneWire.h> 
#include <DallasTemperature.h>

// Thông tin WiFi
#define WIFI_SSID "anhbeo"
#define WIFI_PASS "35357575"

// Firebase Config và Auth
FirebaseConfig config;
FirebaseAuth auth;

// LED Pin
#define LED_PIN 2
#define ledbom 27
#define ledchoan 26

//nhiet do
#define ONE_WIRE_BUS 33
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

//anhsang
int photoresistor = 25;

const int trig = 34;    // chân trig của HC-SR04
const int echo = 35;    // chân echo của HC-SR04

// Tạo đối tượng Firebase
FirebaseData firebaseData;
void setup() {
    //anh sang
  pinMode(photoresistor,INPUT);

  //nhietdo
   sensors.begin();


  // Cấu hình LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(ledbom, OUTPUT);
  digitalWrite(ledbom, LOW);
  pinMode(ledchoan, OUTPUT);
  digitalWrite(ledchoan, LOW);


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

  pinMode(trig,OUTPUT);   // chân trig sẽ phát tín hiệu
  pinMode(echo,INPUT);    // chân echo sẽ nhận tín hiệu
}

void loop() {
  unsigned long duration; // biến đo thời gian
    int distance;           // biến lưu khoảng cách
    /* Phát xung từ chân trig */
    digitalWrite(trig,0);   // tắt chân trig
    delayMicroseconds(2);
    digitalWrite(trig,1);   // phát xung từ chân trig
    delayMicroseconds(5);   // xung có độ dài 5 microSeconds
    digitalWrite(trig,0);   // tắt chân trig
    /* Tính toán thời gian */
    // Đo độ rộng xung HIGH ở chân echo. 
    duration = pulseIn(echo,HIGH);  
    // Tính khoảng cách đến vật.
    distance = int(duration/2/29.412);
    /* In kết quả ra Serial Monitor */
    Serial.print(distance);
    Serial.println("cm");
    delay(200);
// do do duc nuoc
  int ducnuoc = analogRead(32);

//nhiet do
  Serial.print(" Requesting temperatures..."); 
  sensors.requestTemperatures();
  Serial.println("DONE"); 
  Serial.print("Temperature is: "); 
  int nhiet = sensors.getTempCByIndex(0);
  Serial.print(nhiet);

//anh sang
  int sang = analogRead(photoresistor);
  Serial.print("gia tri cam bien anh sang: ");
  Serial.println(sang);

  // Đọc giá trị từ "congtacden"
  if (Firebase.getString(firebaseData, "/congtacden")) {
    String congtacden = firebaseData.stringData();
    Serial.println("Cong tac den: " + congtacden);

    // Điều khiển LED dựa trên giá trị
    if (congtacden == "1") {
      digitalWrite(LED_PIN, HIGH); // Bật LED
      Serial.println("LED turned ON");
    } else {
      digitalWrite(LED_PIN, LOW); // Tắt LED
      Serial.println("LED turned OFF");
    }
  } else {
    Serial.println("Failed to read congtacden: " + firebaseData.errorReason());
    
  }

//bom  
 if (Firebase.getString(firebaseData, "/maybom")) {
    String maybom = firebaseData.stringData();
    Serial.println("Cong tac den: " + maybom);

    // Điều khiển LED dựa trên giá trị
    if (maybom == "1") {
      digitalWrite(ledbom, HIGH); // Bật LED
      Serial.println("LED turned ON");
    } else {
      digitalWrite(ledbom, LOW); // Tắt LED
      Serial.println("LED turned OFF");
    }
  } else {
    Serial.println("Failed to read congtacden: " + firebaseData.errorReason());
    
  }

//may cho an
 if (Firebase.getString(firebaseData, "/maychoan")) {
    String maychoan = firebaseData.stringData();
    Serial.println("Cong tac den: " + maychoan);

    // Điều khiển LED dựa trên giá trị
    if (maychoan == "1") {
      digitalWrite(ledchoan, HIGH); // Bật LED
      Serial.println("LED turned ON");
    } else {
      digitalWrite(ledchoan, LOW); // Tắt LED
      Serial.println("LED turned OFF");
    }
  } else {
    Serial.println("Failed to read congtacden: " + firebaseData.errorReason());
    
  }


//gui data len Firebase
  if (Firebase.setInt(firebaseData, "/sensor/Muc_nuoc", distance)) {
    Serial.println("Random value sent to Muc_nuoc: " + String(distance));
  } else {
    Serial.println("Failed to send value: " + firebaseData.errorReason());
  }

  if (Firebase.setInt(firebaseData, "/sensor/Nhiet_do", nhiet)) {
    Serial.println("Random value sent to Muc_nuoc: " + String(nhiet));
  } else {
    Serial.println("Failed to send value: " + firebaseData.errorReason());
  }

  if (Firebase.setInt(firebaseData, "/sensor/Cuong_do_as", sang)) {
    Serial.println("Random value sent to Muc_nuoc: " + String(sang));
  } else {
    Serial.println("Failed to send value: " + firebaseData.errorReason());
  }

  if (Firebase.setInt(firebaseData, "/sensor/Do_duc", ducnuoc)) {
    Serial.println("Random value sent to Muc_nuoc: " + String(ducnuoc));
  } else {
    Serial.println("Failed to send value: " + firebaseData.errorReason());
  }

  delay(1000); // Chờ 5 giây trước khi lặp lại
}
