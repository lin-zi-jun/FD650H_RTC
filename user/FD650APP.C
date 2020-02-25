/** 
* @file 		  FD650APP.C    			提供数码管显示API，并提供一个简单应用函数示例
* @brief        提供数码管显示API，并提供一个简单应用函数示例
* @author        FD.chd 
* @author        Jun
* @version        A3 
* @date 			2010/07/25    更新说明 :  创建文件 by FD.chd 
* @date 			2012/07/25    更新说明 :  添加注释（doxygen）by jun
* @copyright Copyright (c) 2012 by FUZHOU FUDA HISI MICROELECTRONICS CO.,LTD.              
*/ 
 
#include	"FD650.H"		// 修改该文件以适应硬件环境
#include  <string.h>

#define LEDMAPNUM 22		///<数码管码字数组最大值

/** 字符转数码管显示码字*/
typedef struct _led_bitmap
{
	u_int8 character;
	u_int8 bitmap;
} led_bitmap;

/** 字符转数码管显示码字的数组*/
const led_bitmap BCD_decode_tab[LEDMAPNUM] = 
{	
	{'0', 0x3F}, {'1', 0x06}, {'2', 0x5B}, {'3', 0x4F},
	{'4', 0x66}, {'5', 0x6D}, {'6', 0x7D}, {'7', 0x07},
	{'8', 0x7F}, {'9', 0x6F}, {'a', 0x77}, {'A', 0x77},
	{'b', 0x7C}, {'B', 0x7C}, {'c', 0x58}, {'C', 0x39},
	{'d', 0x5E}, {'D', 0x5E}, {'e', 0x79}, {'E', 0x79},
	{'f', 0x71}, {'F', 0x71} 
};//BCD码字映射


//  *	按键码值表				 
//  *	编址   DIG3 DIG2 DIG1 DIG0 
//  *	KI1  	47H 46H 45H 44H 
//  *	KI2 	4FH 4EH 4DH 4CH 
//  *	KI3  	57H 56H 55H 54H 
//  *	KI4 	5FH 5EH 5DH 5CH 
//  *	KI5  	67H 66H 65H 64H 
//  *	KI6 	6FH 6EH 6DH 6CH 
//  *	KI7  	77H 76H 75H 74H 
//  *	KI1+KI2 7FH 7EH 7DH 7CH 

// /** 按键触发*/
// typedef struct _led_bitmap
// {
// 	u_int8 character;F
// 	u_int8 bitmap;
// } led_bitmap;

// /** 按键触发的数组*/
// const led_bitmap BCD_decode_tab[LEDMAPNUM] = 
// {	
// 	{'0', 0x47}, {'1', 0x46}, {'2', 0x45}, {'3', 0x44},
// 	{'4', 0x4F}, {'5', 0x4E}, {'6', 0x4D}, {'7', 0x4C},
// 	{'8', 0x57}, {'9', 0x56}, {'a', 0x55}, {'A', 0x54},
// 	{'b', 0x67}, {'B', 0x66}, {'c', 0x65}, {'C', 0x64},
// 	{'d', 0xFE}, {'D', 0x6E}, {'e', 0x6D}, {'E', 0x6C},
// 	{'f', 0x77}, {'F', 0x76}, {'F', 0x75}, {'F', 0x74}, 
// 	{'f', 0x7F}, {'F', 0x7E}, {'F', 0x7D}, {'F', 0x7C}
// };//按键触发映射


/** 
 * @brief   转换字符为数码管的显示码
 * @param   cTemp 待转换为显示码的字符
 * @return  显示码值,8位无符号
 * @note    码值见BCD_decode_tab[LEDMAPNUM]，如遇到无法转换的字符返回0  
 */ 
 u_int8 Led_Get_Code(char cTemp)
{
	u_int8 i, bitmap=0x00;

	for(i=0; i<LEDMAPNUM; i++)
	{
		if(BCD_decode_tab[i].character == cTemp)
		{
			bitmap = BCD_decode_tab[i].bitmap;
			break;
		}
	}

	return bitmap;
}

/** 
 * @brief   简单的数码管显示应用示例
 * @param   acFPStr  显示的字符串
 * @param   sec_flag	  开启小数点标志位；
 * @param   Lock		  开启小数点标志位；	
 * @return  无
 * @note    以字符串形式写入,经转换为数码管显示的内容,本示例为4位数码管
   @code 
	   Led_Show_650（"ABCD",1,1）;
   @endcode
 */ 
void Led_Show_650( char *acFPStr, unsigned char sec_flag, unsigned char Lock,int syson)
{
	int i, iLenth;
	int	data[4]={0x00, 0x00, 0x00, 0x00};
	if( strcmp(acFPStr, "") == 0 )
	{
		return;
	}
	iLenth = strlen(acFPStr);
	if(iLenth>4)
		iLenth = 4;
	
	for(i=0; i<iLenth; i++)
	{
		data[i] = Led_Get_Code(acFPStr[i]);	 
	}
	FD650_Write(syson);// 开启显示和键盘，8段显示方式
	//发显示数据
	FD650_Write( FD650_DIG0 | (u_int8)data[0] );	//点亮第一个数码管
	if(sec_flag)
		FD650_Write( FD650_DIG1 | (u_int8)data[1] | FD650_DOT ); //点亮第二个数码管
	else
		FD650_Write( FD650_DIG1 | (u_int8)data[1] ); 
	if(Lock)
		FD650_Write( FD650_DIG2 | (u_int8)data[2] | FD650_DOT ); //点亮第三个数码管
	else
		FD650_Write( FD650_DIG2 | (u_int8)data[2] );
	FD650_Write( FD650_DIG3 | (u_int8)data[3] ); //点亮第四个数码管
}

u_int8 key_Show_650(void)
{
	u_int8 key_code;
	key_code = FD650_Read();
	if((key_code!=0X00)&&(key_code!=0XFF)){
		printf("0X%X\r\n",key_code);	
	}
	return key_code;
}


