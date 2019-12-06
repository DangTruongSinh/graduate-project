
 /*--------------File Info-------------------------------------------------------
** 文   件   名:  Config.h
** 最后修改日期:  2008.3.28
** 版        本:  V1.0
** 描        述:  配置文件、裁剪不需要的功能					
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



/**************全局变量************/
//fap 歌词
extern FATFS fs;            // Work area (file system object) for logical drive
extern FIL fsrc, fdst,fap,file,fii;      // file objects
extern FRESULT res,re;         // FatFs function common result code
extern UINT br, bw,bt;;         // File R/W count
extern FILINFO finfo,fno;
extern DIR dirs;

extern u32 datasize;
extern u8 LrcEnable;
extern u8 bt30MS;  //全局变量
extern u8 tag_table[512];	//TAG存放
extern u8 LrcBuffer[4096];	//全部歌词存放位置
extern u8 LRC_DATA[512]; //存放一句歌词地址

 struct FileList
  {
    char filename[150];
    struct FileList *next;
	struct FileList *back;	 //指向上一个
  };
  typedef struct FileList dnode;
  typedef dnode *dlink;

extern  dlink head;

typedef struct 
{
	u16 LastX;  //上次的坐标
	u16 LastY;
	u16 NowX; //现在的坐标
	u16 NowY;						   	    
	u8  Key_NowState;// 现在笔的状态
	u8  Key_LastState;//上一次笔的状态
}Touch_Key;



#endif
