
/** 
* @file 		  FD650.H    			定义了硬件相关IO 操作，以及芯片相关配置参数 
* @brief        定义了硬件相关IO 操作，以及芯片相关配置参数
* @author        FD.chd 
* @author        Jun
* @version        A3 
* @date 			2010/07/25    更新说明 :  创建文件 by FD.chd 
* @date 			2012/07/25    更新说明 :  添加注释（doxygen）by jun
* @copyright Copyright (c) 2012 by FUZHOU FUDA HISI MICROELECTRONICS CO.,LTD.              
*/            

// 硬件相关定义, 请根据实际硬件修改本文件
#ifndef _FD650APP_H
#define _FD650APP_H

typedef unsigned char   u_int8;
typedef unsigned short  u_int16;	
typedef unsigned long 	u_int32;

void Led_Show_650( char *acFPStr, unsigned char sec_flag, unsigned char Lock,int syson);
u_int8 key_Show_650(void);
#endif
