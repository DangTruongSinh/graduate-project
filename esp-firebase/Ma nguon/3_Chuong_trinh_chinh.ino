
#include <dht_nonblocking.h>
#include <Arduino_JSON.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>

#include <FirebaseArduino.h>
#define FIREBASE_HOST "demo1-d32ad.firebaseio.com"
#define FIREBASE_AUTH "OAXbDiJ8eBdpI35nPgqul8HlqMrHbz5fdNcsEuh5"
#define WIFI_SSID "Tran Khang"
#define WIFI_PASSWORD "0699215119096"
#define SS_PIN 10
#define RST_PIN 9
#define DHT_SENSOR_TYPE  DHT_TYPE_11
#define buzzer 1
static const int DHT_SENSOR_PIN =  5; //D1
String read_rfid;
String data ;
String ok_rfid_1 = "80137aa6";

MFRC522 mfrc522(SS_PIN, RST_PIN);
StaticJsonBuffer<256> jb;
JsonObject& obj = jb.createObject();
DHT_nonblocking dht(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
void setup() {
  pinMode (16, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  digitalWrite(buzzer, HIGH);
  delay(500);
  digitalWrite(buzzer, LOW);
  delay(500);
  //Firebase.stream("khoa");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  //obj["ten chanel:"] = String("doantotnghiep");
  Firebase.stream("/khoa");
  Firebase.setString("khoa", "off");
}
static bool measure_environment (float* temperature , float* humidity)
{
  static unsigned long measurement_timestamp = millis();
  if (millis() - measurement_timestamp > 2000 )
  {
    if (dht.measure(temperature, humidity) == true)
    {
      measurement_timestamp = millis();
      return (true);
    }
  }
  return (false);


}


void dump_byte_array(byte *buffer, byte bufferSize) {
  read_rfid = "";
  for (byte i = 0; i < bufferSize; i++) {
    read_rfid = read_rfid + String(buffer[i], HEX);
  }
}
void open_lock() {
  //Use this routine when working with Relays and Solenoids etc.
  digitalWrite(16, HIGH);
  //delay(5000);
  // digitalWrite(16,HIGH);
}
void close_lock() {
  //Use this routine when working with Relays and Solenoids etc.
  digitalWrite(16, LOW);
}

void loop() {

  float temperature;
  float  humidity;
  if (Firebase.failed()) {
    Serial.println("streaming error");
    Serial.println(Firebase.error());
  }
  if (measure_environment(&temperature, &humidity) == true)
  {
    Firebase.setFloat("nhietdo", temperature);
    Serial.println(temperature);

    Firebase.setFloat("doam", humidity);
  }
  if (Firebase.available()) {
    FirebaseObject event = Firebase.readEvent();
    String eventType = event.getString("type");
    eventType.toLowerCase();
    if (eventType == "put") {
      Serial.println(event.getString("data"));
      data = event.getString("data");
      if (data == "ON" || data == "on") {
        open_lock();
      }
      else if (data == "OFF" || data == "off") {
        close_lock();
      }
    }
  }



  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println(read_rfid);
  if (read_rfid == ok_rfid_1 ) {

    //ok, open the door.
    digitalWrite(16, HIGH);
    delay(5000);
    digitalWrite(16, LOW);

  }

}
