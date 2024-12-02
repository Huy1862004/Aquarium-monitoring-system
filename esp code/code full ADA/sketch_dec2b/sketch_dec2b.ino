#include <WiFi.h>
#include "AdafruitIO_WiFi.h"
#include <OneWire.h> 
#include <DallasTemperature.h>
// Thông tin WiFi
#define WIFI_SSID "anhbeo"
#define WIFI_PASS "35357575"

// Thông tin Adafruit IO
#define AIO_USERNAME "quannmin1103"
#define AIO_KEY "aio_ZQHJ73iwvn8FdeCb6qVu8I6Vw1Hf"

AdafruitIO_WiFi io(AIO_USERNAME, AIO_KEY, WIFI_SSID, WIFI_PASS);

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

// Feed "den" và "muc nuoc"
AdafruitIO_Feed *den = io.feed("den");
AdafruitIO_Feed *bom = io.feed("bom");
AdafruitIO_Feed *choan = io.feed("choan");
AdafruitIO_Feed *mucNuoc = io.feed("muc nuoc");
AdafruitIO_Feed *doduc = io.feed("doduc");
AdafruitIO_Feed *nhietdo = io.feed("nhietdo");
AdafruitIO_Feed *anhsang = io.feed("anhsang");

const int trig = 34;    // chân trig của HC-SR04
const int echo = 35;    // chân echo của HC-SR04

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

  // Khởi động serial
  Serial.begin(9600);

  // Kết nối Adafruit IO
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // Thiết lập callback cho feed 
  den->onMessage(handleDenMessage);
  bom->onMessage(handlebomMessage);
  choan->onMessage(handlechoanMessage);
  // Chờ kết nối
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected!");
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

  // Xử lý Adafruit IO
  io.run();
  mucNuoc->save(distance);
  doduc->save(ducnuoc);
  doduc->save(nhiet);
  doduc->save(sang);
  Serial.println("Random value sent to muc-nuoc: " + String(distance));
  delay(5000); // Đẩy giá trị mỗi 5 giây
}


// Xử lý tin nhắn từ feed "den"
void handleDenMessage(AdafruitIO_Data *data) {
  int value = data->toInt(); // Chuyển đổi giá trị từ feed
  if (value == 1) {
    digitalWrite(LED_PIN, HIGH); // Bật LED
    Serial.println("LED turned ON");
  } else {
    digitalWrite(LED_PIN, LOW); // Tắt LED
    Serial.println("LED turned OFF");
  }
}

// Xử lý tin nhắn từ feed "bom"
void handlebomMessage(AdafruitIO_Data *data) {
  int bom = data->toInt(); // Chuyển đổi giá trị từ feed
  if (bom == 1) {
    digitalWrite(ledbom, HIGH); // Bật LED
    Serial.println("LED turned ON");
  } else {
    digitalWrite(ledbom, LOW); // Tắt LED
    Serial.println("LED turned OFF");
  }
}


// Xử lý tin nhắn từ feed "choan"
void handlechoanMessage(AdafruitIO_Data *data) {
  int choan = data->toInt(); // Chuyển đổi giá trị từ feed
  if (choan == 1) {
    digitalWrite(ledchoan, HIGH); // Bật LED
    Serial.println("LED turned ON");
  } else {
    digitalWrite(ledchoan, LOW); // Tắt LED
    Serial.println("LED turned OFF");
  }
}
