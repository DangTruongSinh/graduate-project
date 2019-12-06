
 /*--------------File Info-------------------------------------------------------
** ��   ��   ��:  Config.h
** ����޸�����:  2008.3.28
** ��        ��:  V1.0
** ��        ��:  �����ļ����ü�����Ҫ�Ĺ���					
**------------------------------------------------------------------------------
** Created   by:    		
** Created date:			
*******************************************************************************/
#ifndef __Config_h
#define __Config_h    
//#include "stm32f10x_lib.h" 
//#include "stm32f10x_type.h"

#include "stm32f10x.h"                  // Device header
#include "integer.h"
#include "ff.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>



/**************ȫ�ֱ���************/
//fap ���
extern FATFS fs;            // Work area (file system object) for logical drive
extern FIL fsrc, fdst,fap,file,fii;      // file objects
extern FRESULT res,re;         // FatFs function common result code
extern UINT br, bw,bt;;         // File R/W count
extern FILINFO finfo,fno;
extern DIR dirs;

extern u32 datasize;
extern u8 LrcEnable;
extern u8 bt30MS;  //ȫ�ֱ���
extern u8 tag_table[512];	//TAG���
extern u8 LrcBuffer[4096];	//ȫ����ʴ��λ��
extern u8 LRC_DATA[512]; //���һ���ʵ�ַ

 struct FileList
  {
    char filename[150];
    struct FileList *next;
	struct FileList *back;	 //ָ����һ��
  };
  typedef struct FileList dnode;
  typedef dnode *dlink;

extern  dlink head;

typedef struct 
{
	u16 LastX;  //�ϴε�����
	u16 LastY;
	u16 NowX; //���ڵ�����
	u16 NowY;						   	    
	u8  Key_NowState;// ���ڱʵ�״̬
	u8  Key_LastState;//��һ�αʵ�״̬
}Touch_Key;



#endif
