#include <SPI.h>
#include <LoRa.h>
#include <ArduinoJson.h>
#include "DHT.h"    

const int DHTPIN = 4;       //Đọc dữ liệu từ DHT11 ở chân 2 trên mạch Arduino
const int DHTTYPE = DHT11;  //Khai báo loại cảm biến, có 2 loại là DHT11 và DHT22
int data;
String adress = "";
 
DHT dht(DHTPIN, DHTTYPE);


void setup() {
  Serial.begin(9600);
  while (!Serial);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(A3, INPUT);
  
  Serial.println("LoRa Sender");
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(250E3);
  dht.begin();         // Khởi động cảm biến
}

void Delay(float delayTime, void (func)()){
    unsigned long endTime = millis() + delayTime;
    while(millis() < endTime)
    {
      func();
     
    }
}

void Sender()
{
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
   Serial.print("Received packet '");
   adress = (char)LoRa.read();
   adress += (char)LoRa.read();
   Serial.print(adress);
    // read packet
   if(adress == "AT"){ 
      data = LoRa.read();
      Serial.println(data);
      switch (data) {
          case 48:
              Serial.print("so 0"); 
              digitalWrite(6, LOW);  
              digitalWrite(7, LOW);         
              break;
          case 49:
              Serial.print("so 1");
              digitalWrite(6, LOW);  
              digitalWrite(7, HIGH);               
              break;
          case 50:
              Serial.print("so 2");
              digitalWrite(6, HIGH);  
              digitalWrite(7, LOW);           
              break;
          case 51:
              Serial.print("so 3");
              digitalWrite(6, HIGH);
              digitalWrite(7, HIGH);                           
              break;
          case 52:
              Serial.print("so 4"); 
              digitalWrite(6, LOW);  
              digitalWrite(7, LOW);         
              break;
          case 53:
              Serial.print("so 5");
              digitalWrite(6, LOW);  
              digitalWrite(7, HIGH);               
              break;
          case 54:
              Serial.print("so 6");
              digitalWrite(6, HIGH);  
              digitalWrite(7, LOW);           
              break;
          case 55:
              Serial.print("so 7");
              digitalWrite(6, HIGH);
              digitalWrite(7, HIGH);                           
              break;
          default:           
              break; 
      // mặc định là không làm gì cả
      // bạn có thể có default: hoặc không
      }
    }
   else
   {Serial.print("not for me");}
   

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
   }
}

void loop() {

  int d = analogRead(A3); 
  int percent = map(d, 0, 1023, 0, 100);
  Serial.print(percent);
  Serial.println('%');

  float h = dht.readHumidity();    //Đọc độ ẩm
  float t = dht.readTemperature(); //Đọc nhiệt độ
  Serial.println(t);
  Serial.println(h);
  
  String loraMessage = "";
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["n"] = "1";
  root["t"] = t;
  root["ah"] = h;
  root["sh"] = 100 - percent;
  root.printTo(loraMessage);
  
  
  Serial.print("Sending packet: ");
  Serial.println(loraMessage);
  
  LoRa.beginPacket();
  LoRa.print(loraMessage);
  LoRa.endPacket();

  

  Delay(7000,Sender);
}
