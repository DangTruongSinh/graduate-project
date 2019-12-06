//Người tạo: Dong Tran
//Đồ án tốt nghiệp

//Thư viện
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "ThingSpeak.h"
#include <Servo.h>

Servo servo;
#include <SPI.h>
#include <MFRC522.h>

// Thông tin về MQTT Broker
#define mqtt_server "m24.cloudmqtt.com" 
#define mqtt_topic_pub "esp8266"   
#define mqtt_topic_sub "esp8266"
#define mqtt_user "test"    
#define mqtt_pwd "12345678"

// Kênh và API key Thinkspeak
unsigned long myChannelNumber = 783084;
const char * myWriteAPIKey = "DCWIDDU9MFHIKF4A";

#define LED1 4
#define LED2 5
#define door 16

// Chân RST, SS của RFID
#define RST_PIN         0           
#define SS_PIN          15       

////Port của CloudMQTT
const uint16_t mqtt_port =  13098; //Port của CloudMQTT

int httpCode;
int dem=0;
float resolution=3.3/1023;
float data = 0;

const char *ssid     = "Bảo Tâm"; 
const char *password = "dongtam09"; 
WiFiClient client;
WiFiClient espClient;
PubSubClient client1(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;

boolean ledStatus1 = 0;
boolean ledStatus2 = 0;
boolean current1 = 0;
boolean current2 = 0;
boolean window_servo = 0;
boolean cbas = 0;
// Create MFRC522 instance.
MFRC522 mfrc522(SS_PIN, RST_PIN);   
MFRC522::MIFARE_Key key;

void setup() {
  pinMode(door, OUTPUT);
  digitalWrite(door, LOW);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  servo.attach(2); //D4
  servo.write(0);
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
  Serial.begin(115200); // Initialize serial communications with the PC
  WiFi.disconnect();
  delay(10);
  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("NodeMcu connected to wifi...");
  Serial.println(ssid);
  Serial.println();
  client1.setServer(mqtt_server, mqtt_port); 
  client1.setCallback(callback);
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  
  httpCode = ThingSpeak.writeField(myChannelNumber, 1, 0, myWriteAPIKey);
}
/**
 * call back.
 */
// Hàm call back để nhận dữ liệu
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String nhankitu ="";
  for (int i = 0; i < length; i++) {
    nhankitu = nhankitu + (char)payload[i];
  }
  Serial.println(nhankitu);
  Serial.print("\r\n");
  if(String(topic)=="esp8266")
  { 
      Serial.println(nhankitu);
      if (nhankitu == "sw2_1")
      {   
        digitalWrite(LED2, HIGH);
        client1.publish(mqtt_topic_pub, "sw2_ON");
        current2 = 1;
        delay(2000);
        Serial.println("SANG DEN 2");
      }
      if (nhankitu == "sw2_0")
      {
        digitalWrite(LED2, LOW);
        client1.publish(mqtt_topic_pub, "sw2_OFF");
        current2 = 0;
        Serial.println("TAT DEN 2");
      }
      if(cbas == 1)
      {
        if (nhankitu == "sw1_1")
        {   
          digitalWrite(LED1 , HIGH);
          client1.publish(mqtt_topic_pub, "sw1_ON");
          current1 = 1;
          servo.write(0);
          delay(2000);
          Serial.println("SANG DEN 1");
        }
        if (nhankitu == "sw1_0")
        {
          digitalWrite(LED1, LOW);
          client1.publish(mqtt_topic_pub, "sw1_OFF");
          current1 = 0;
          servo.write(180);
          delay(2000);
          Serial.println("TAT DEN 1");
        }
        if (nhankitu == "get")
        {
          if (current1 == 1 || ledStatus1 == 1)
            client1.publish(mqtt_topic_pub, "sw1_ON");
          else
            client1.publish(mqtt_topic_pub, "sw1_OFF");
          if (current2 == 1 || ledStatus2 == 1)
            client1.publish(mqtt_topic_pub, "sw2_ON");
          else
            client1.publish(mqtt_topic_pub, "sw2_OFF");
        }
      }
      else 
      {
        if (nhankitu == "sw1_1")
        {   
          servo.write(0);
          client1.publish(mqtt_topic_pub, "sw1_ON");
          current1 = 1;
          Serial.println("MO REM");
        }
        if (nhankitu == "sw1_0")
        {
          servo.write(180);
          client1.publish(mqtt_topic_pub, "sw1_OFF");
          current1 = 0;
          Serial.println("DONG REM");
        }
        if (nhankitu == "get")
        {
          if (current1 == 1 || window_servo == 1)
            client1.publish(mqtt_topic_pub, "sw1_ON");
          else
            client1.publish(mqtt_topic_pub, "sw1_OFF");
          if (current2 == 1 || ledStatus2 == 1)
            client1.publish(mqtt_topic_pub, "sw2_ON");
          else
            client1.publish(mqtt_topic_pub, "sw2_OFF");
        }
      }
  }
}
// Hàm reconnect thực hiện kết nối lại khi mất kết nối với MQTT Broker
void reconnect() {
    // Chờ tới khi kết nối
    while (!client1.connected()) 
    {
      // Thực hiện kết nối với mqtt user và pass
      if (client1.connect("ESP8266Client",mqtt_user, mqtt_pwd)) 
      {
        // Khi kết nối sẽ publish thông báo
        client1.publish(mqtt_topic_pub, "ESP_reconnected");
        // ... và nhận lại thông tin này
        client1.subscribe(mqtt_topic_sub);
      } 
      else
        delay(5000);
    }
}
/**
 * Main loop.
 */
void loop() {
    //Kiểm tra kết nối
    char msg[50];
    int value = random(0,100);
    long lastMsg = 0;
    // check connection, if fail then reconnect
    if (!client1.connected()) 
    {
      reconnect();
    }
    client1.loop();
    data = (analogRead(A0) * resolution) * 100;
    delay(100);
    Serial.println("Sensor light:");
    Serial.println(data);
    if (data > 240)
      cbas = 1;
    else
      cbas = 0;
    digitalWrite(door, LOW);
//      Publish_Data();
//          data =0;
    
    // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent())
        return;
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;
    // Show some details of the PICC (that is: the tag/card)
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    // Check for compatibility
    if (    piccType != MFRC522::PICC_TYPE_MIFARE_MINI
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
        Serial.println(F("This sample only works with MIFARE Classic cards."));
        return;
    }

    String strID = "";
    for (byte i = 0; i < 4; i++) {
      strID +=
      (mfrc522.uid.uidByte[i] < 0x10 ? "0" : "") +
      String(mfrc522.uid.uidByte[i], HEX) +
      (i!=3 ? ":" : "");
    }
    strID.toUpperCase();
    Serial.println(strID);
        if(cbas == 1)
    {
         if(strID.indexOf("57:46:D5:FD")>=0)
         { 
           httpCode = ThingSpeak.writeField(myChannelNumber, 1, 0, myWriteAPIKey);
           ledStatus1 = !ledStatus1;
           digitalWrite(LED1,ledStatus1 );
           if(ledStatus1 ==HIGH)
           {   
              client1.publish(mqtt_topic_pub, "sw1_ON");
              servo.write(0);
              delay(2000);
           }
           else
           {
              client1.publish(mqtt_topic_pub, "sw1_OFF");
              servo.write(180);
              delay(2000);
           }
         }
    }
    else 
    {
      if(strID.indexOf("04:85:3F:5B")>=0) 
      { 
          httpCode = ThingSpeak.writeField(myChannelNumber, 1, 0, myWriteAPIKey);
          window_servo = !window_servo;
          if(window_servo ==HIGH)
          {  
              servo.write(180);
              client1.publish(mqtt_topic_pub, "sw1_ON");
              delay(2000);
           }
           else
           {
              servo.write(0);
              client1.publish(mqtt_topic_pub, "sw1_OFF");
              delay(2000);
           }
      }
      if(strID.indexOf("57:46:D5:FD")>=0)
      { 
          httpCode = ThingSpeak.writeField(myChannelNumber, 1, 0, myWriteAPIKey);
          ledStatus1 = !ledStatus1;
          digitalWrite(LED1,ledStatus1 );
       }
    }
     
    if(strID.indexOf("99:41:3F:5B")>=0) 
    { 
        digitalWrite(door, HIGH);
        httpCode = ThingSpeak.writeField(myChannelNumber, 1, 1, myWriteAPIKey);
        delay(2000);
    }
    if(strID.indexOf("A2:BC:C3:30")>=0) 
    { 
        httpCode = ThingSpeak.writeField(myChannelNumber, 1, 0, myWriteAPIKey);
        ledStatus2 = !ledStatus2;
        digitalWrite(LED2,ledStatus2);
        if(ledStatus2 ==HIGH)
              client1.publish(mqtt_topic_pub, "sw2_ON");
        else
              client1.publish(mqtt_topic_pub, "sw2_OFF");
    }



      // Halt PICC
      mfrc522.PICC_HaltA();
      // Stop encryption on PCD
      mfrc522.PCD_StopCrypto1();
//      httpCode = ThingSpeak.writeField(myChannelNumber, 1, 0, myWriteAPIKey);
}
//
//void Publish_Data()
//{
//  // Write value to Field 1 of a ThingSpeak Channel
//  int httpCode = ThingSpeak.writeField(myChannelNumber, 1, data, myWriteAPIKey);
//  Serial.println(data);
//  // Wait 20 seconds to uodate the channel again
//  delay(100);
//}
    

   
