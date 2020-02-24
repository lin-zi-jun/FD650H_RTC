/** 
* @file 		  FD650.C   			��д��ʵ�ֺ�����оƬ������������
* @brief        ��д��ʵ�ֺ�����оƬ������������
* @author        FD.chd 
* @author        Jun
* @version        A3 
* @date 			2010/07/25    ����˵�� :  �����ļ� by FD.chd 
* @date 			2012/07/25    ����˵�� :  ����ע�ͣ�doxygen��by jun
* @copyright Copyright (c) 2012 by FUZHOU FUDA HISI MICROELECTRONICS CO.,LTD.              
*/
#include	"FD650.H"	

/** 
 * @brief   ����FD650
 * @param   ��
 * @return  ��
 * @note   ��SCL�ߵ�ƽ�ڼ䲶��SDA���½��أ�ʹFD650��ʼ����     
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
 * @brief   ֹͣFD650
 * @param   ��
 * @return  ��
 * @note   ��SCL�ߵ�ƽ�ڼ䲶��SDA�������أ�ʹFD650ֹͣ����     
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
 * @brief   ����һ���ֽڣ�8λ�����ݸ�FD650
 * @param   dat �޷���8λ����
 * @return  ��
 * @note   ��SCL������д��FD650 ��������9���ֽڶ���Ӧ���ź�	    
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
		DELAY;  // ��ѡ��ʱ
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
 * @brief   ��ȡFD650�İ�����ֵ
 * @param   ��
 * @return  �����޷���8λ����
 * @note   ��SCL�½��ض�ȡFD650 ��������9���ֽڷ�����ЧӦ��    
 */ 
u_int8  FD650_RdByte( void )
{
	u_int8 dat,i;
	FD650_SDA_SET;
	FD650_SDA_D_IN;
	dat = 0;
	for( i = 0; i != 8; i++ )
	{
		DELAY;  // ��ѡ��ʱ
		FD650_SCL_SET;
		DELAY;  // ��ѡ��ʱ
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
 * @defgroup FD650_Driver FD650 ����ģ�� 
 * FD650 ����ģ���ṩ��ͳһ�Ľӿں����Լ��û��ĺ궨�����������FD650����
 */ 

/** 
 * @ingroup FD650_Driver
 * @brief   ����FD650��������� cmd
 * @param cmd FD650��������������޷���16λ2�ֽ��� �ɺ궨�����ȡֵ
 * @return  ��
 * @note ����һ��16λ2�ֽڵ���������������FD650�������ã�������ʾ����.
	   �������ʾֱ��ʹ��FD650_Write ���� �������Ӧ��ָ��� ��ָ���ʽΪ��  
 �����λѡ��(����) |   �������ʾ��ֵ(����) |   �����С����(��ѡ)  
 * @code 
	FD650_Write(FD650_SYSON_8);// ������ʾ�ͼ��̣�8����ʾ��ʽ
	//����ʾ����
	FD650_Write( FD650_DIG0 | (u_int8)data[0] );    //������һ�������
	if(sec_flag)
		FD650_Write( FD650_DIG1 | (u_int8)data[1] | FD650_DOT ); //�����ڶ��������,����ʾС����
	else
		FD650_Write( FD650_DIG1 | (u_int8)data[1] ); 
	if(Lock)
		FD650_Write( FD650_DIG2 | (u_int8)data[2] | FD650_DOT ); //��������������ܣ�����ʾС����
	else
		FD650_Write( FD650_DIG2 | (u_int8)data[2] );
	FD650_Write( FD650_DIG3 | (u_int8)data[3] ); //�������ĸ������
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
 * @brief   ��ȡFD650�İ������� 
 * @param   ��
 * @return  keycode ������������״̬�ļ�ֵ ���޷���8λ1�ֽ�����������Ч����ֵ����0
 * @note 	��ȡFD650�İ������� ���м����·��ؼ�ֵ���޼����·���0��������ֵ�����±�
 * @code 
 *	������ֵ��				 
 *	��ַ   DIG3 DIG2 DIG1 DIG0 
 *	KI1  	47H 46H 45H 44H 
 *	KI2 	4FH 4EH 4DH 4CH 
 *	KI3  	57H 56H 55H 54H 
 *	KI4 	5FH 5EH 5DH 5CH 
 *	KI5  	67H 66H 65H 64H 
 *	KI6 	6FH 6EH 6DH 6CH 
 *	KI7  	77H 76H 75H 74H 
 *	KI1+KI2 7FH 7EH 7DH 7CH 

 *	Keycode=FD650_Read( );		 // keycode Ϊ��ֵ
 *	Key_Fun ( Keycode) ;		 // ������������ 
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
