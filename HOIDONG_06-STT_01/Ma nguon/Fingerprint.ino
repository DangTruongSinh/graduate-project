#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Fingerprint.h>
#include <Wire.h>
#include <Servo.h> 
#define SERVO_PIN 12
Servo gServo;
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1', '4', '7', '*'},
  {'2', '5', '8', '0'},
  {'3', '6', '9', '#'},
  {'A', 'B', 'C', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad
Keypad mykeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
LiquidCrystal_I2C lcd(0x27, 16, 2);
char class_x = '0';
char class_y = '3';
byte index = 0;
char matkhau[10] = {};
String password_admin = "1111";
int id=0;
int id_del=0;
byte index_id = 0;
char id_buff[] = {};
uint8_t getFingerprintEnroll();



SoftwareSerial mySerial(10, 11);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t getFingerprintEnroll();

bool check_dangky = false;
bool check_delete = false;
boolean out_fingerprint = false;
char statu[4] = {'0', '0', '0', '0'};
void setup() {
  // put your setup code here, to run once:
  ////lcd////////
 lcd.init();
  lcd.backlight();
  lcd.display();
  Serial.begin(9600);
  ////////////finger/////////
    finger.begin(57600);//Khoi tao van tay
  if (finger.verifyPassword()) {
    //Serial.println("Found fingerprint sensor!");
  } else {
    //Serial.println("Did not find fingerprint sensor :(");
     }
      //////////////buzz///////
       pinMode(13,OUTPUT);
       digitalWrite(13,1);
       delay(100);
       digitalWrite(13,0);
   ////////////////servo/////////////
     gServo.attach(SERVO_PIN); 
      gServo.write(180);
     delay(1000);
   
}

void loop() {
  // put your main code here, to run repeatedly:
 char key = mykeypad.getKey();
  switch (class_x)
  {
    case '0':
      {
        //Serial.println("1. QUET VAN TAY");
        //Serial.println("2. QUAN TRI ADMIN");
        //+++++++++++++
        hienthi_lcd_clear(0, 0, "1.QUET VAN TAY");
        hienthi_lcd(1, 0, "2.QUAN TRI ADMIN");

        delay(500);
        
        while (class_x == '0')
        {
          char key = mykeypad.getKey();

          if (key == '1')
          {
            class_x = '1';
            break; 
          }
         
          if (key == '2')
          {
            class_x = '2';
            class_y = '3';
            hienthi_lcd_clear(0, 0, "Nhap MK Admin:");
            lcd.setCursor(0, 1);
            break; 
          }


        }
        break;
      }
    case '1':
      {
        //Serial.println("Waiting for valid finger...");
        hienthi_lcd_clear(0, 0, "DANG QUET...");
        while (!out_fingerprint)
        {
          
          getFingerprintIDez();
          delay(50);
        }
        out_fingerprint = false;

        break;
      }
   
    case '2':
      {
        switch (class_y)
        {
          case '3':
            {
              if (key)
              {
                if (key == '#')
                {
                  matkhau[index] = '\0';
                  //Serial.print("ban vua nhap: ");
                  //Serial.println(matkhau);
                  if (String(matkhau) == password_admin)
                  {
                    //Serial.println("Xin Chao Admin!");
                    hienthi_lcd_clear(0, 0, "Xin Chao Admin!");

                  //  StrClear(matkhau, index);     //Xoa chuoi mat khau da luu
                    index = 0;
                    delay(1000);
                    class_y = '0';                // Ve lai menu
                  }
                  else
                  {
                    //Serial.println("Nhap lai mat khau!");
                    hienthi_lcd_clear(0, 0, "Nhap lai mat khau:");
                    lcd.setCursor(0, 1);
                  //  StrClear(matkhau, index);
                    index = 0;
                  }
                }
                else if (key == 'D')                  //Back
                {
                  class_x = '0';
                  //StrClear(matkhau, index);
                  index = 0;
                }
                else if (index < 4)
                {
                  matkhau[index] = key;
                  index++;
                  //Serial.print(key); // Bo o duoi, để khi ấn back ko bị in ra D
                  lcd.print("*");
                }
               
              }
              break;
            }
          case '0':
            {
              //Serial.println("1. THEM VAN TAY");
              //Serial.println("2. XOA VAN TAY");
              //Serial.println("D. BACK");
              hienthi_lcd_clear(0, 0, "1.THEM");
              hienthi_lcd(0, 8, "2.XOA");
              hienthi_lcd(1, 0, "3.OPEN");
              hienthi_lcd(1, 8, "D.BACK");
              
              delay(500);
              while (class_y == '0')
              {
                char key = mykeypad.getKey();
                if (key == '1')
                {
                  class_y = '1';
                  hienthi_lcd_clear(0, 0, "Nhap ID Can Them:");
                  lcd.setCursor(0, 1);
                }
                if (key == '2')
                {
                  class_y = '2';
                  hienthi_lcd_clear(0, 0, "Nhap ID Can Xoa:");
                  lcd.setCursor(0, 1);
                }
               /* if (key == '3')
                {
                  class_y = '3';
                  hienthi_lcd_clear(0, 0, "Nhap Lai MK cu:");
                  lcd.setCursor(0, 1);
                }*/
                if (key == 'D')
                {
                  class_y = 'D';
                }
                if(key == '3')
                {
                  class_y = '4';
                }
              }
              break;
            }
          
          case '1':
            {
              //Serial.println("Chuong trinh them van tay");
             
              id = 0;                         //reset id truoc
              char  id_buff[]={};
              index_id = 0;
              
              while (!check_dangky)
              {
                char key = mykeypad.getKey();
                
               ////////////////////////////////////////
                if (key)
                {
                  if (key == '#')
                  { 
                    String str_id = String(id_buff);
                    id = str_id.toInt();
                    //Serial.print("eee");
                    //Serial.print(id);
                    check_dangky = 1;               //Co thể chạy chương trình enroll
                  }
                  else if (key == 'D'){
                      class_y = '0'; 
                      id = 0;     //De khong chay getFingerprintEnroll();
                      check_dangky = 1;
                    }
                    else
                    {
                      id_buff[index] = key;
                      index_id++;
                      check_dangky = 0;
                      lcd.print(key);
                  }
                }
                 //////////////////////////////////////// 
             }               
                if (id != 0)
                getFingerprintEnroll();
              delay(1000);
              //id = 0;
              check_dangky = false;
              class_y = '0';
              class_x = '2';
              break;
            }
          case '2':
            {
              //Serial.println("Chuong trinh XOa van tay");
              id = 0;                         //reset id truoc
            char id_buff[]={};
              index_id = 0;
              while (!check_delete)
              {
                char key = mykeypad.getKey();
                if (key)
                {
                  if (key == '#')
                  {
                    String str_id = String(id_buff);
                    id = str_id.toInt();
                    //Serial.print("ID XOA: ");
                    //Serial.println(id);
                    check_delete = 1 ;                    //Co thể chạy chương trình delete
                  }
                  else if (key == 'D')
                  {
                    class_y = '0';
                    check_delete = 1;
                    id = 0;
                  }
                  else
                  {
                    id_buff[index_id] = key;
                    index_id++;
                  check_delete = 0;
                  lcd.print(key);
               
                  }
                }
              }
                 
              id_del=id;
              if (id != 0)
                deleteFingerprint(id);
              id = 0;
              check_delete = false;
              class_y = '0';
              break;
            }
        
          case 'D':
            {
              class_y = '0';
              class_x = '0';
              break;
            }
          case '4':
            {
              hienthi_lcd_clear(0,5,"MOI VAO");
              digitalWrite(13,1);
              delay(100);
              digitalWrite(13,0);
              delay(100);
              digitalWrite(13,1);
              delay(100);
              digitalWrite(13,0);
              delay(100);
              digitalWrite(13,1);
              delay(100);
              digitalWrite(13,0);
                
              
              gServo.write(0);
              delay(1000);
  
              gServo.write(90);
              delay(1000);
  
              gServo.write(180);
              delay(1000);
              class_y = '0';
              class_x = '0';
              break;

            }
        }
       break;
      }
 
  }
}
//-----------FINDGER----------------------------------------

//-----------Enroll-----------------------------------------
uint8_t getFingerprintEnroll() {

  int p = -1;
  //Serial.print("Waiting for valid finger to enroll as #"); //Serial.println(id);
  hienthi_lcd_clear(0, 0, "THEM VAN TAY");
  hienthi_lcd(1, 3, " VOI ID:");
  lcd.print(id);
  while (p != FINGERPRINT_OK)
  {
    p = finger.getImage();
    switch (p) 
    {
      case FINGERPRINT_OK:
        //Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        //Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        //Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        //Serial.println("Imaging error");
        break;
      default:
        //Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) 
  {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }

  //Serial.println("Remove finger");
  hienthi_lcd_clear(0, 0, "Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  //Serial.print("ID "); //Serial.println(id);
  p = -1;
  //Serial.println("Place same finger again");
  hienthi_lcd_clear(0, 0, "Same finger");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        //Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        //Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        //Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        //Serial.println("Imaging error");
        break;
      default:
        //Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  //Serial.print("Creating model for #");  //Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    //Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    //Serial.println("Fingerprints did not match");
    return p;
  } else {
    //Serial.println("Unknown error");
    return p;
  }

  //Serial.print("ID "); //Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    //Serial.println("Stored!");
    hienthi_lcd_clear(0, 2, "THEM VAN TAY");
    hienthi_lcd(1,2," THANH CONG");
     ////////keu buzz////
  digitalWrite(13,1);
  delay(100);
  digitalWrite(13,0);
  delay(100);
  digitalWrite(13,1);
  delay(100);
  digitalWrite(13,0);
  delay(100);
  digitalWrite(13,1);
  delay(100);
  digitalWrite(13,0);
    delay(1000);
   
  
    ///////////////////////truyen qua raspberry////////////////
    char dataString[50] = {0};
    sprintf(dataString,"%03X",555);
    Serial.println(dataString);
    sprintf(dataString,"%03X",id);// convert a value to hexa 
    Serial.println(dataString);   // send the data
    delay(1000); 
    //////////////////////////////////////////////////////
    return p;// give the loop some break
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
  } else if (p == FINGERPRINT_BADLOCATION) {
    //Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    //Serial.println("Error writing to flash");
    return p;
  } else {
    //Serial.println("Unknown error");
    return p;
  }
}
//-----------FINGER- PRINT----------------------------------
uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      //Serial.println("Imaging error");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    //Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    //Serial.println("Did not find a match");
    return p;
  } else {
    //Serial.println("Unknown error");
    return p;
  }

  // found a match!
  //Serial.print("Found ID #"); //Serial.print(finger.fingerID);
  //Serial.print(" with confidence of "); //Serial.println(finger.confidence);
}

  // returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  char dataString[50] = {0};
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  {hienthi_lcd_clear(0,0,"ban khong phai");
  hienthi_lcd(1,3," nhan vien");
  digitalWrite(13,1);
  delay(3000);
  digitalWrite(13,0);
  delay(20);
  class_x = '0';    
  out_fingerprint = true;      // check de out
  delay(1);
  return -1;
  };

  // found a match!
  //Serial.print("Found ID #"); //Serial.print(finger.fingerID);
  //Serial.print(" with confidence of "); //Serial.println(finger.confidence);
  ////////////TRUYEN CHO RASPBERRY//////////////
  sprintf(dataString,"%03X",222);
  Serial.println(dataString);
  sprintf(dataString,"%02X",finger.fingerID); // convert a value to hexa 
  Serial.println(dataString);   // send the data
  /////////////////////////////////////////////////
  hienthi_lcd_clear(0, 0, "Xin chao ID :");
  lcd.print(finger.fingerID);
  delay(1000);                  // give the loop some break
  hienthi_lcd(1, 2, " MOI VAO ");
  delay (1000);
  ////////keu buzz////////
  digitalWrite(13,1);
  delay(100);
  digitalWrite(13,0);
  delay(100);
  digitalWrite(13,1);
  delay(100);
  digitalWrite(13,0);
  delay(100);
  digitalWrite(13,1);
  delay(100);
  digitalWrite(13,0);
 
  //////quay servo/////////
  gServo.write(0);
  delay(1000);
  gServo.write(90);
  delay(1000);
  gServo.write(180);
  delay(1000);

  
  
//  gServo.write(70);
  //  hienthi_lcd(1, 0, "Do Tin Cay: ");
  //  lcd.print(finger.confidence);
  //+++++++++++++++++++++++
  hienthi_lcd_clear(0, 0, "Dang tai du lieu...");

  //++++++++++++++++++++++
  delay(1);
//  gServo.write(10);
 // delay(50);
  class_x = '0';               // Quay lai menu
  out_fingerprint = true;      // check de out
  delay(1);                  //  KO DELAY LÀ BỊ LỖI
  //class_x = '0'; 
  return finger.fingerID;
}

//-----------DeleteFinger-----------------------------------

uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;
char dataString[50]={0};
  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    //Serial.println("Deleted!");
    hienthi_lcd_clear(0,4, "DA XOA !");
    delay(1000);
    ////keu buzz////
  digitalWrite(13,1);
  delay(100);
  digitalWrite(13,0);
  delay(100);
  digitalWrite(13,1);
  delay(100);
  digitalWrite(13,0);
  delay(100);
  digitalWrite(13,1);
  delay(100);
  digitalWrite(13,0);
     ////////////TRUYEN CHO RASPBERRY//////////////
  sprintf(dataString,"%03X",444);
  Serial.println(dataString);
  sprintf(dataString,"%02X",id_del); // convert a value to hexa 
  Serial.println(dataString);   // send the data
  id_del=0;
  /////////////////////////////////////////////////
    
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
     hienthi_lcd_clear (0,0, "communication error");
     delay(2000);
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    //Serial.println("Could not delete in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    //Serial.println("Error writing to flash");
    return p;
  } else {
    //Serial.print("Unknown error: 0x"); //Serial.println(p, HEX);
    return p;
  }
}
//-----------LCD--------------------------------------------
void hienthi_lcd(int hang, int cot, char *content)
{
  lcd.setCursor(cot, hang);
  lcd.print(content);
}

void hienthi_lcd_clear(int hang, int cot, char *content)
{
  lcd.clear();
  lcd.setCursor(cot, hang);
  lcd.print(content);
}

  // returns -1 if failed, otherwise returns ID #
