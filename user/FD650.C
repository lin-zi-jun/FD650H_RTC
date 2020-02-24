/** 
* @file 		  FD650.C   			读写的实现函数、芯片基本操作函数
* @brief        读写的实现函数、芯片基本操作函数
* @author        FD.chd 
* @author        Jun
* @version        A3 
* @date 			2010/07/25    更新说明 :  创建文件 by FD.chd 
* @date 			2012/07/25    更新说明 :  添加注释（doxygen）by jun
* @copyright Copyright (c) 2012 by FUZHOU FUDA HISI MICROELECTRONICS CO.,LTD.              
*/
#include	"FD650.H"	

/** 
 * @brief   启动FD650
 * @param   无
 * @return  无
 * @note   在SCL高电平期间捕获到SDA的下降沿，使FD650开始工作     
 */ 
void FD650_Start( void )
{
	FD650_SDA_SET;  
	FD650_SDA_D_OUT;
	FD650_SCL_SET;
	FD650_SCL_D_OUT;
	DELAY;
	FD650_SDA_CLR;
	DELAY;      
	FD650_SCL_CLR;
}

/** 
 * @brief   停止FD650
 * @param   无
 * @return  无
 * @note   在SCL高电平期间捕获到SDA的上升沿，使FD650停止工作     
 */ 
void FD650_Stop( void )
{
	FD650_SDA_CLR;
	FD650_SDA_D_OUT;
	DELAY;
	FD650_SCL_SET;
	DELAY;
	FD650_SDA_SET;
	DELAY;
	FD650_SDA_D_IN;
}

/** 
 * @brief   发送一个字节（8位）数据给FD650
 * @param   dat 无符号8位数据
 * @return  无
 * @note   在SCL上升沿写入FD650 ，包括第9个字节读入应答信号	    
 */ 
void FD650_WrByte( u_int8 dat )
{
	u_int8 i;
	FD650_SDA_D_OUT;
	for( i = 0; i != 8; i++ )
	{
		if( dat & 0x80 ) 
		{
		    FD650_SDA_SET;
		}
		else 
		{
		    FD650_SDA_CLR;
		}
		DELAY;
		FD650_SCL_SET;
		dat <<= 1;
		DELAY;  // 可选延时
		FD650_SCL_CLR;
	}
	FD650_SDA_SET;
	FD650_SDA_D_IN;
	DELAY;
	FD650_SCL_SET;
	DELAY;
	FD650_SCL_CLR;
}

/** 
 * @brief   读取FD650的按键码值
 * @param   无
 * @return  返回无符号8位数据
 * @note   在SCL下降沿读取FD650 ，包括第9个字节发送无效应答    
 */ 
u_int8  FD650_RdByte( void )
{
	u_int8 dat,i;
	FD650_SDA_SET;
	FD650_SDA_D_IN;
	dat = 0;
	for( i = 0; i != 8; i++ )
	{
		DELAY;  // 可选延时
		FD650_SCL_SET;
		DELAY;  // 可选延时
		dat <<= 1;
		if( FD650_SDA_IN ) dat++;
		FD650_SCL_CLR;
	}
	FD650_SDA_SET;
	DELAY;
	FD650_SCL_SET;
	DELAY;
	FD650_SCL_CLR;
	return dat;
}


/** 
 * @defgroup FD650_Driver FD650 驱动模块 
 * FD650 驱动模块提供简单统一的接口函数以及用户的宏定义来完成驱动FD650工作
 */ 

/** 
 * @ingroup FD650_Driver
 * @brief   发送FD650的命令参数 cmd
 * @param cmd FD650控制命令参数，无符号16位2字节数 由宏定义决定取值
 * @return  无
 * @note 发送一个16位2字节的命令参数，可完成FD650命令设置，数据显示功能.
	   数码管显示直接使用FD650_Write 函数 ，配合相应的指令即可 。指令格式为：  
 数码管位选择(必须) |   数码管显示键值(必须) |   数码管小数点(可选)  
 * @code 
	FD650_Write(FD650_SYSON_8);// 开启显示和键盘，8段显示方式
	//发显示数据
	FD650_Write( FD650_DIG0 | (u_int8)data[0] );    //点亮第一个数码管
	if(sec_flag)
		FD650_Write( FD650_DIG1 | (u_int8)data[1] | FD650_DOT ); //点亮第二个数码管,并显示小数点
	else
		FD650_Write( FD650_DIG1 | (u_int8)data[1] ); 
	if(Lock)
		FD650_Write( FD650_DIG2 | (u_int8)data[2] | FD650_DOT ); //点亮第三个数码管，并显示小数点
	else
		FD650_Write( FD650_DIG2 | (u_int8)data[2] );
	FD650_Write( FD650_DIG3 | (u_int8)data[3] ); //点亮第四个数码管
 * @endcode 
 */ 
void FD650_Write( u_int16 cmd )	
{
	FD650_Start(); 
	FD650_WrByte(((u_int8)(cmd>>7)&0x3E)|0x40);
	FD650_WrByte((u_int8)cmd);
	FD650_Stop();

	return;
}

/** 
 * @ingroup FD650_Driver
 * @brief   读取FD650的按键码字 
 * @param   无
 * @return  keycode 读到按键按下状态的键值 ，无符号8位1字节数，读到无效按键值返回0
 * @note 	读取FD650的按键码字 ，有键按下返回键值，无键按下返回0。按键码值表见下表
 * @code 
 *	按键码值表				 
 *	编址   DIG3 DIG2 DIG1 DIG0 
 *	KI1  	47H 46H 45H 44H 
 *	KI2 	4FH 4EH 4DH 4CH 
 *	KI3  	57H 56H 55H 54H 
 *	KI4 	5FH 5EH 5DH 5CH 
 *	KI5  	67H 66H 65H 64H 
 *	KI6 	6FH 6EH 6DH 6CH 
 *	KI7  	77H 76H 75H 74H 
 *	KI1+KI2 7FH 7EH 7DH 7CH 

 *	Keycode=FD650_Read( );		 // keycode 为键值
 *	Key_Fun ( Keycode) ;		 // 按键处理函数 
 * @endcode 
 */ 
u_int8 FD650_Read( void )		
{
	u_int8 keycode = 0;
	
	FD650_Start(); 
	FD650_WrByte((u_int8)(FD650_GET_KEY>>7)&0x3E|0x01|0x40);
	keycode=FD650_RdByte();
	FD650_Stop();
	if( (keycode&0x00000040) ==0)
		keycode = 0;
	return keycode;
}

