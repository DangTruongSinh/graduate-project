#include "R305.h"
#include "USART.h"
#include "Touch.h"
#include "LCD.h"
uint16_t fingerID, confidence;
uint8_t recvPacket[20];
uint32_t thePassword = 0;
uint32_t theAddress = 0xFFFFFFFF;

//dung USART2 de giao tiep voi R305
	USART_TypeDef* USARTy = USART2;
//dung USART1 de hien thi
	

void writePacket(uint32_t addr, uint8_t packettype, uint16_t len, uint8_t *packet)
{
	uint16_t sum;
	uint8_t i;
	
  USART_SendChar(USARTy, (uint8_t)(FINGERPRINT_STARTCODE >> 8));
  USART_SendChar(USARTy, (uint8_t)FINGERPRINT_STARTCODE);
  USART_SendChar(USARTy, (uint8_t)(addr >> 24));
  USART_SendChar(USARTy, (uint8_t)(addr >> 16));
  USART_SendChar(USARTy, (uint8_t)(addr >> 8));
  USART_SendChar(USARTy, (uint8_t)(addr));
  USART_SendChar(USARTy, (uint8_t)packettype);
  USART_SendChar(USARTy, (uint8_t)(len >> 8));
  USART_SendChar(USARTy, (uint8_t)(len));
 
  sum = (len>>8) + (len&0xFF) + packettype;
  for (i=0; i< len-2; i++) {
    USART_SendChar(USARTy, (uint8_t)(packet[i]));
    sum += packet[i];
  }

  USART_SendChar(USARTy, (uint8_t)(sum>>8));
  USART_SendChar(USARTy, (uint8_t)sum);
}

uint8_t getReply(uint8_t packet[])
{
  uint8_t reply[20], idx, i, packettype;
	uint16_t len;
	idx = 0;

  while (1)
  {
    while (!USART_GetFlagStatus(USARTy, USART_FLAG_RXNE)) {}
    // something to read!
    reply[idx] = USART_ReceiveData(USARTy);
    if ((idx == 0) && (reply[0] != (FINGERPRINT_STARTCODE >> 8)))
      continue;
    idx++;
    
    // check packet!
    if (idx >= 9) {
      if ((reply[0] != (FINGERPRINT_STARTCODE >> 8)) ||
          (reply[1] != (FINGERPRINT_STARTCODE & 0xFF)))
          return FINGERPRINT_BADPACKET;
			
      packettype = reply[6];
      len = reply[7];
			
      len <<= 8;
      len |= reply[8];
      len -= 2;
			
      if (idx <= (len+10)) continue;
      packet[0] = packettype;      
      for (i=0; i<len; i++) {
        packet[1+i] = reply[9+i];
      }
      return len;
    }
  }
}

uint8_t verifyPassword(void)
{
	uint8_t len;
	/*
	uint8_t packet[] = {FINGERPRINT_VERIFYPASSWORD, 
                      (thePassword >> 24), (thePassword >> 16),
                      (thePassword >> 8), thePassword};
	*/
  uint8_t packet[] = {0x13, 0, 0 ,0 ,0};	//uint32 password = 0;
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, 7, packet);	//0x01
  len = getReply(recvPacket); 
  if ((len == 1) && (recvPacket[0] == FINGERPRINT_ACKPACKET) && (recvPacket[1] == FINGERPRINT_OK)) return 1;
  return 0;
}

int8_t getImage(void)
{
	uint8_t len;
  uint8_t packet[] = {FINGERPRINT_GETIMAGE};
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, 3, packet);
  len = getReply(recvPacket);  
  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;
  return recvPacket[1];
}

int8_t image2Tz(uint8_t slot)
{
	uint8_t len;
	//uint8_t packet[] = {FINGERPRINT_IMAGE2TZ, slot};
	uint8_t packet[] = {FINGERPRINT_IMAGE2TZ, 0};
	packet[1] = slot;
	
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
  len = getReply(recvPacket);
  
  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;
  return recvPacket[1];
}

int8_t createModel(void)
{
	uint8_t len;
  uint8_t packet[] = {FINGERPRINT_REGMODEL};
	
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
  len = getReply(recvPacket);
  
  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;
  return recvPacket[1];
}

int8_t storeModel(uint16_t id)
{
	uint8_t len;
  //uint8_t packet[] = {FINGERPRINT_STORE, 0x01, id >> 8, id & 0xFF};
	uint8_t packet[] = {FINGERPRINT_STORE, 0x01, 0, 0};
	packet[2] = id >> 8;
	packet[3] = id & 0xFF;
	
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
  len = getReply(recvPacket);
  
  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;
  return recvPacket[1];
}

int8_t deleteModel(uint16_t id)
{
		uint8_t len;
    //uint8_t packet[] = {FINGERPRINT_DELETE, id >> 8, id & 0xFF, 0x00, 0x01};
		uint8_t packet[] = {FINGERPRINT_DELETE, 0, 0, 0x00, 0x01};
		packet[1] = id >> 8;
		packet[2] = id & 0xFF;
		
    writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
    len = getReply(recvPacket);
        
    if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
        return -1;
    return recvPacket[1];
}

int8_t emptyDatabase(void)
{
	uint8_t len;
  uint8_t packet[] = {FINGERPRINT_EMPTY};
	
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
  len = getReply(recvPacket);
  
  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;
  return recvPacket[1];
}

int8_t fingerFastSearch(void)
{
	uint8_t len;
  // high speed search of slot #1 starting at page 0x0000 and page #0x00A3 
  uint8_t packet[] = {FINGERPRINT_HISPEEDSEARCH, 0x01, 0x00, 0x00, 0x00, 0xA3};
	
	fingerID = 0xFFFF;
  confidence = 0xFFFF;
	
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
  len = getReply(recvPacket);
  
  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;

  fingerID = recvPacket[2];
  fingerID <<= 8;
  fingerID |= recvPacket[3];
  
  confidence = recvPacket[4];
  confidence <<= 8;
  confidence |= recvPacket[5];
  
  return recvPacket[1];
}

void delay_2s(void)
{
	int i = 2000000;
	while(i--);
}

uint8_t fingerEnroll(uint8_t id)
{
	int8_t p = -1;
	
	//USART_SendString(USART1, "Dat tay vao cam bien...\n");
	LCD_SHOW_STRING(10,60,"Dat tay vao", BLACK,WHITE);
	while (p != FINGERPRINT_OK)
	{
    p = getImage();
	}
	
	p = image2Tz(1);
	if(p != FINGERPRINT_OK)
	{
		//USART_SendString(USART1, "Loi!\n");
		return 0;
	}
	//USART_SendString(USART1, "Lay ta ra.\n");
	LCD_SHOW_STRING(10,60,"Lay tay ra", BLACK,WHITE);
	delay_2s();
	p = -1;
	while (p != FINGERPRINT_NOFINGER)
	{
    p = getImage();
  }
	
	//USART_SendString(USART1, "Dat tay lai xac nhan...\n");
	LCD_SHOW_STRING(10,60,"Dat tay vao lai xac nhan", BLACK,WHITE);
	p = -1;
	while (p != FINGERPRINT_OK) {
    p = getImage();
	}
	
	p = image2Tz(2);
	if(p != FINGERPRINT_OK)
	{
		//USART_SendString(USART1, "Loi!\n");
		return 0;
	}
	//USART_SendString(USART1, "Tao Model.\n");
	p = createModel();
	if(p != FINGERPRINT_OK)
	{
		//USART_SendString(USART1, "Loi!\n");
		return 0;
	}
	
	p = storeModel(id);
	if (p == FINGERPRINT_OK)
	{
		//USART_SendString(USART1, "Luu thanh cong!\n");
		LCD_SHOW_STRING(10,60,"Luu thanh cong!             \n", BLACK,WHITE);
		return 1;
	}
	else
	{
		//USART_SendString(USART1, "Luu khong thanh cong!\n");
		LCD_SHOW_STRING(10,60,"Luu khong thanh cong!        \n", BLACK,WHITE);
		return 0;
	}
}

int8_t fingerIDSearch(void)
{
	int8_t p = -1;
	
	//USART_SendString(USART1, "Dat tay vao cam bien...\n");
	
	while(p != FINGERPRINT_OK)
	{
		p = getImage();
		if(Pen_Point.Key_Sta==Key_Down)
		return -1;
	}

  p = image2Tz(1);
  if (p != FINGERPRINT_OK)
	{
		//USART_SendString(USART1, "Loi!\n");
		return -1;
	}	
	delay_2s();
	
  p = fingerFastSearch();
  if (p == FINGERPRINT_PACKETRECIEVEERR)
	{
		//USART_SendString(USART1, "Loi!\n");
		return -1;
	}
	else if (p == FINGERPRINT_NOTFOUND) {
		//USART_SendString(USART1, "Khong tim thay.\n");
		return -2;
	}
  //USART_SendString(USART1, "Ket qua tim kiem: ID ");
	USART_SendChar(USART1, fingerID + '0');
	USART_SendChar(USART1, '\n');
	
  return fingerID; 
}

//-------mycode
uint8_t verifypass(void)
{
		uint8_t len;
  uint8_t packet[] = {0x13, 0, 0 ,0 ,0};	//uint32 password = 0;
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, 7, packet);	//0x01
  len = getReply(recvPacket);
  
  if ((len == 1) && (recvPacket[0] == FINGERPRINT_ACKPACKET) && (recvPacket[1] == FINGERPRINT_OK))
    return 1;

  return 0;
}
