#include <WiFi.h>
#include "AdafruitIO_WiFi.h"
#include <ESP32Servo.h>
#include <OneWire.h> 
#include <DallasTemperature.h>

// Thông tin WiFi
#define WIFI_SSID "anhbeo"
#define WIFI_PASS "35357575"

#define TRIG_PIN 14  // Chân TRIG của cảm biến nối với GPIO 4 của ESP32
#define ECHO_PIN 12  // Chân ECHO của cảm biến nối với GPIO 5 của ESP32
// Khai báo chân servo và góc ban đầu
#define SERVO_PIN 25
Servo myServo;

// Thông tin Adafruit IO
#define AIO_USERNAME "quannmin1103"
#define AIO_KEY "aio_ZQHJ73iwvn8FdeCb6qVu8I6Vw1Hf"

AdafruitIO_WiFi io(AIO_USERNAME, AIO_KEY, WIFI_SSID, WIFI_PASS);

// LED Pin
#define LED_PIN 2
#define bomb 13
//nhiet do
#define ONE_WIRE_BUS 33
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);
//anhsang
int photoresistor = 25;

// Feed "den" và "muc nuoc"
AdafruitIO_Feed *den = io.feed("den");
AdafruitIO_Feed *bom = io.feed("bom");
AdafruitIO_Feed *choan = io.feed("choan");

AdafruitIO_Feed *mucNuoc = io.feed("muc nuoc");
AdafruitIO_Feed *doduc = io.feed("doduc");
AdafruitIO_Feed *nhietdo = io.feed("nhietdo");
AdafruitIO_Feed *anhsang = io.feed("anhsang");


void setup() {

  pinMode(photoresistor,INPUT);

  //nhietdo
  sensors.begin();
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(bomb, OUTPUT);
  digitalWrite(bomb, LOW);
  
  //sieuam
  pinMode(TRIG_PIN, OUTPUT);  // Thiết lập chân TRIG là OUTPUT
  pinMode(ECHO_PIN, INPUT);   // Thiết lập chân ECHO là INPUT
  myServo.attach(SERVO_PIN);

  // Khởi động serial
  Serial.begin(9600);

  // Kết nối Adafruit IO
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // Thiết lập callback cho feed "den"
  den->onMessage(handleDenMessage);
  bom->onMessage(bomc);
  choan->onMessage(choanc);

  // Chờ kết nối
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected!");
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

float duc() {
  int rawValue = analogRead(34);
  Serial.println("Raw ADC Value: " + String(rawValue));

  // Giả sử giá trị tối đa của ADC là 4095 (12-bit ADC)
  float randomduc = 4095 - rawValue;  // Đảo ngược giá trị
  randomduc = randomduc / 32;         // Tính toán với số thực

  Serial.println("Calculated Do Duc: " + String(randomduc));
  return randomduc;
}

void loop() {
  // Xử lý Adafruit IO
  io.run();

  // Gửi giá trị ngẫu nhiên lên "muc nuoc"
  float ndomValue = getDistance();
  Serial.print("Khoảng cách: ");
  Serial.print(ndomValue);
  Serial.println(" cm");
  mucNuoc->save(ndomValue);
  Serial.println("Random value sent to muc-nuoc: " + String(ndomValue));


  //nhiet do
  Serial.print(" Requesting temperatures..."); 
  sensors.requestTemperatures();
  Serial.println("DONE"); 
  Serial.print("Temperature is: "); 
  int cbnhiet = sensors.getTempCByIndex(0);
  Serial.print(cbnhiet);
  nhietdo->save(cbnhiet);
  Serial.println("Random value sent to nhietdo: " + String(cbnhiet));

  //anh sang
  int sang = analogRead(photoresistor);
  Serial.print("gia tri cam bien anh sang: ");
  Serial.println(sang);
  anhsang->save(sang);
  Serial.println("Random value sent to muc-nuoc: " + String(sang));

  int ndomduc = duc();
  doduc->save(ndomduc);
  Serial.println("Random value sent to muc-nuoc: " + String(ndomduc));
  delay(5000);  // Đẩy giá trị mỗi 5 giây
}

// Xử lý tin nhắn từ feed "den"
void handleDenMessage(AdafruitIO_Data *data) {
  int value = data->toInt();  // Chuyển đổi giá trị từ feed
  if (value == 1) {
    digitalWrite(LED_PIN, HIGH);  // Bật LED
    Serial.println("LED turned ON");
  } else {
    digitalWrite(LED_PIN, LOW);  // Tắt LED
    Serial.println("LED turned OFF");
  }
}
//may bom
void bomc(AdafruitIO_Data *data) {
  int maybom = data->toInt();  // Chuyển đổi giá trị từ feed
   // Điều khiển LED dựa trên giá trị
    if (maybom == 1) {
      digitalWrite(bomb, HIGH);  // Bật LED
      Serial.println("LED turned ON");
    } else {
      digitalWrite(bomb, LOW);  // Tắt LED
      Serial.println("LED turned OFF");
    }
}

//cho an
void choanc(AdafruitIO_Data *data) {
  int choan = data->toInt();  // Chuyển đổi giá trị từ feed
  if (choan == 1) {
      for (int angle = 0; angle <= 90; angle++) {
      myServo.write(angle);
      delay(15);     
  }
  }
     else {
    }
}