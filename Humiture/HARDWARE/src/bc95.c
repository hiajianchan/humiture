#include "bc95.h"
#include "SHT2x.h"

uint8_t RecvSignal[] = "+NSONMI:";
uint8_t RealSingal[] = ",7000,";
//uint8_t RealSingal[] = ",1234,";
uint8_t AT_NSORF[] = "AT+NSORF=0,256\r";

#define	GPRS_BUFFER_SIZE		(255)

uint8_t GPRSRecvFlag = 0;

uint8_t GPRSRecvBuffer[GPRS_BUFFER_SIZE];
uint8_t GPRSRecvData[GPRS_BUFFER_SIZE];


uint8_t check_ack_timeout = 10;
uint8_t ue_exist_flag = 0;
uint8_t ue_need_reboot_flag = 0;

//#define  delay_ms    Delay  
static void HexToStr(char *str, const char *hex, int hex_len);

//��鷵�ص���Ӧ�Ƿ����Ԥ��
//�������ΪԤ�ڷ��ص��ַ���
//����0��Ϊ��⵽Ԥ��ֵ
//����ֵ��Ԥ���ַ����ڵ�λ��
uint8_t* BC95_check_ack(char *str)
{
	char *strx=0;
	if(usart2_rcvd_flag)		
	{ 
		usart2_rcvd_buf[usart2_rcvd_len]='\0';
		strx=strstr((const char*)usart2_rcvd_buf,(const char*)str);
	} 
	return (uint8_t*)strx;
}

//����atָ���
//cmd:atָ�ack��Ԥ����Ӧ��waittime,��ʱʱ��
//����0�����ͳɹ�
//����1�����ͳ�ʱ
uint8_t BC95_send_cmd(char *cmd,char *ack,uint16_t waittime)
{
	uint8_t res=0; 
	usart2_rcvd_flag=0;
	usart2_rcvd_len = 0;
	GPRSRecvFlag = 0;
	memset(usart2_rcvd_buf,0,USART2_RX_BUF_LEN);
//	printf("%s\r\n",cmd);
	USARTx_printf(USART2,"%s\r\n",cmd);
	USARTx_printf(USART1,"%s\r\n",cmd);
	if(ack&&waittime)
	{
		while(--waittime)	
		{
			delay_ms(20);
			if(usart2_rcvd_flag)
			{
				
				if(BC95_check_ack(ack))break;
				usart2_rcvd_flag=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 
//�ϵ���򣬼��ģ���Ƿ����ӣ���������Ƿ�Ϊ�Զ�ģʽ���Ƿ�Ϊ��Ҫ��Ƶ��

void BC95_power_on(void)
{
	//BC95_send_cmd(SET_UE_REBOOT,"REBOOT",100);
	check_ack_timeout = 10;
	ue_exist_flag = 1;
	while(BC95_send_cmd("AT","OK",100)&&check_ack_timeout)
	{
		if(check_ack_timeout)
		{
			check_ack_timeout--;
			ue_exist_flag = 0;
			usart_send_str(USART1,"�ȴ�ģ���ϵ� \r\n");
		}
		delay_ms(1000);
	}
	//�ж�ģ���Ƿ����Զ�����ģʽ�����������ģ�����ó��Զ�ģʽ
	if(ue_exist_flag&&!BC95_send_cmd(QUERY_UE_CONNECT_MODE,"AUTOCONNECT,FALSE",100))
	{
		check_ack_timeout = 3;
		while(check_ack_timeout)
		{
			check_ack_timeout--;
			if(BC95_send_cmd(SET_UE_AUTOCONNECT,"OK",100))
			{
				USARTx_printf(USART1,"����Ϊ�Զ�ģʽ�ɹ���\r\n");
				break;
			}
			delay_ms(1000);
		}
		ue_need_reboot_flag =1;
	}
	//�ж�ģ���Ƿ���Ĭ������Ƶ�Σ�������������ó�Ĭ��Ƶ��
	if(ue_exist_flag&&BC95_send_cmd(QUERY_UE_BAND,UE_DEFAULT_BAND,100))
	{
		;
//		BC95_send_cmd(SET_UE_DEFAULT_BAND,UE_DEFAULT_BAND,100);
//		USARTx_printf(USART1,"����Ĭ��Ƶ�Σ�\r\n");
//		ue_need_reboot_flag = 1;
	}
	//����ģ����Ч����
	if(ue_exist_flag&&ue_need_reboot_flag)
	{
		ue_need_reboot_flag = 0;
		check_ack_timeout = 10;
		BC95_send_cmd(SET_UE_REBOOT,"REBOOT",100);
		USARTx_printf(USART1,"����ģ�飡\r\n");
		while(check_ack_timeout&&!BC95_check_ack("Neul"))
		{
			if(BC95_check_ack("Neul"))
			{
				break;
			}else
			{
				check_ack_timeout--;
				delay_ms(1000);
			}
		}
	}
}



//���ģ�������״̬���������LED1����˸��LED1����Ϊ����ע���ɹ�
//�˺������������״̬������鸽��ע��״̬������״̬����ʹ��BC95_send_cmd���������
//����ע��ʧ�ܻ��߳�ʱ����0������1����ע���ɹ�������2�����ɹ�
uint8_t query_net_status(void)
{
	uint8_t res = 0;
	uint8_t attached_flag = 0;
	uint8_t registered_flag = 0;
	check_ack_timeout = 20;
//	led_need_blink = 1;
	while(!(attached_flag&&registered_flag)&&check_ack_timeout)
	{
		if(!BC95_send_cmd(QUERY_UE_SCCON_STATS,SET_UE_SCCON,100))
		{
			attached_flag = 1;
			registered_flag = 1;
			res = 1;
//			led_need_blink =0;
//			setLEDs(LED1);
			USARTx_printf(USART1,"������ע���ɹ���r\n");
			break;
		}else
		{
			if(!attached_flag)
			{
				if(!BC95_send_cmd(QUERY_UE_ATTACH_STATS,UE_ATTACHED_STATS,100))
				{
					USARTx_printf(USART1,"�����ɹ�!\r\n");
					attached_flag = 1;
					res =2;
				}else
				{
					USARTx_printf(USART1,"���ڸ���...\r\n");
//					setLEDs(LED1);
					attached_flag = 0;
				}
			}
			if(attached_flag&&!registered_flag)
			{
				if(attached_flag&&!BC95_send_cmd(QUERY_UE_EREG_STATS,UE_EREGISTERED_STATS,100))
				{
					USARTx_printf(USART1,"ע���ɹ���\r\n");
					registered_flag = 1;
//					led_need_blink =0;
//					setLEDs(LED1);
					res =1;
				}else
				{
					USARTx_printf(USART1,"����ע��...\r\n");
					registered_flag = 0;
				}
			}				
		}
		check_ack_timeout--;
		delay_ms(500);
		if(!check_ack_timeout&&!attached_flag&&!registered_flag)
		{
//			led_need_blink =0;
//			ResetLEDs(LED1);
			USARTx_printf(USART1,"������ע��ʧ�ܣ�\r\n");
		}
	}
	return res;
}

//��ȡ���ݣ���ȡ���ջ�������������ݱ��浽des,posΪ��ʼ��ַ��lenΪ��ȡ����
void get_str_data(char* des,char pos,char len)
{
	memcpy(des,usart2_rcvd_buf+pos,len);
}	

//����UDP���ӣ����뱾��UDP�˿ںţ�����0-6��socket id�ţ�
uint8_t creat_UDP_socket(char* local_port)
{
	char data[10]="";
	uint8_t socket_id = 7;
	char temp[64]="AT+NSOCR=DGRAM,17,";
	strcat(temp,local_port);
	strcat(temp,",1");
	if(!BC95_send_cmd(temp,"OK",100))
	{
		get_str_data(data,2,1);
		socket_id = (uint8_t)myatoi(data);
		USARTx_printf(USART1,"Socket�����ɹ������ID--> %d��\r\n",socket_id);
		return socket_id;
	}
	USARTx_printf(USART1,"Socket����ʧ�ܣ��Ѿ�������˿ڱ�ռ�ã�\r\n");
	return socket_id;
}
//�������ݺ���������socket,����IP��Զ�������˿ڣ����ݳ��ȣ�����
//������ʱʹ���ַ�������
//����ֵ0�����ͳɹ�������UDPΪ���Ĵ��䣬���������Ƿ���յ�ģ�����޷�ȷ�ϵģ�
//����ֵ1������ʧ��
uint8_t send_UDP_msg(char *socket,char *hostIP,char *port,char *dataLen,char *data)
{
	char ptr[600]="AT+NSOST=";
	char *buf,b[200],*len;

	
	buf = b;
	printf("send_UDP_msg\r\n");
	HexToStr(buf,(const char*)data,strlen(data));
	printf("buf:%s\r\n",buf);

	strcat(ptr,socket);
	strcat(ptr,",");
	strcat(ptr,hostIP);
	strcat(ptr,",");
	strcat(ptr,port);
	strcat(ptr,",");
	strcat(ptr,dataLen);
	strcat(ptr,",");
	
	
	strcat(ptr,buf);
	if(!BC95_send_cmd(ptr,"OK",200))
	{
		USARTx_printf(USART1,"��������--> %s��\r\n",ptr);	
		return 0;
	}
	return 1;
}
//�������ݴ��������ݲ��ṩʵ�ַ���
uint8_t *receive_udp(char *socket,char *dataLen)
{

	return 0;
}

#define		FULL		(0xFF)

/* �����ַ���*/
uint8_t LookForStr(uint8_t *s, uint8_t *t)
{
	uint8_t i = 0;
	uint8_t *s_temp;
	uint8_t *m_temp;
	uint8_t *t_temp;
	
	if (s==0 || t==0) return 0;
	
	for (s_temp=s; *s_temp!='\0'; s_temp++,i++)
	{
		for (m_temp=s_temp, t_temp=t; *t_temp!='\0' && *t_temp==*m_temp; t_temp++, m_temp++);
		if (*t_temp == '\0')
		{
			return i;
		}
	}
	
	return FULL;
}

typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed   char  INT8S;                    /* Signed    8 bit quantity                           */
typedef unsigned short INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed   short INT16S;                   /* Signed   16 bit quantity                           */
typedef unsigned long  INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed   long  INT32S;                   /* Signed   32 bit quantity                           */
typedef float          FP32;                     /* Single precision floating point                    */
typedef double         FP64;                     /* Double precision floating point                    */

#ifndef __cplusplus
    typedef enum {FALSE = 0, TRUE = !FALSE} bool;
#endif

BOOLEAN GetField(INT8U *pData, INT8U *pField, INT32S nFieldNum, INT32S nMaxFieldLen)
{
    INT32S i, i2, nField;
    //
    // Validate params
    //
    if(pData == NULL || pField == NULL || nMaxFieldLen <= 0)
    {
        return FALSE;
    }

    //
    // Go to the beginning of the selected field
    //
    i = 0;
    nField = 0;
    while(nField != nFieldNum && pData[i])
    {
        if(pData[i] == ',')
        {
            nField++;
        }

        i++;

        if(pData[i] == NULL)
        {
            pField[0] = '\0';
            return FALSE;
        }
    }

    if(pData[i] == ',' || pData[i] == '*')
    {
        pField[0] = '\0';
        return FALSE;
    }

    //
    // copy field from pData to Field
    //
    i2 = 0;
    while(pData[i] != ',' && pData[i] != '*' && pData[i])
    {
        pField[i2] = pData[i];
        i2++;
        i++;

        //
        // check if field is too big to fit on passed parameter. If it is,
        // crop returned field to its max length.
        //
        if(i2 >= nMaxFieldLen)
        {
            i2 = nMaxFieldLen - 1;
            break;
        }
    }
    pField[i2] = '\0';

    return TRUE;
}

void int2str(int n,char *str)
{
	 char buf[10] = "";
	 int i = 0;
	 int len = 0;
	 int temp = n < 0 ? -n: n;  // tempΪn�ľ���ֵ

	 if (str == NULL)
	 {
			 return;
	 }
	 while(temp)
	 {
			 buf[i++] = (temp % 10) + '0';  //��temp��ÿһλ�ϵ�������buf
			 temp = temp / 10;
	 }

	 len = n < 0 ? ++i: i;  //���n�Ǹ����������Ҫһλ���洢����
	 str[i] = 0;            //ĩβ�ǽ�����0
	 while(1)
	 {
			 i--;
			 if (buf[len-i-1] ==0)
			 {
					 break;
			 }
			 str[i] = buf[len-i-1];  //��buf������������ַ������ַ���
	 }
	 if (i == 0 )
	 {
			 str[i] = '-';          //����Ǹ��������һ������
	 }
}

static void HexToStr(char *str, const char *hex, int hex_len)
{
	int i = 0;
	unsigned char tmp = 0;
	
	for(i = 0; i < hex_len; i++)
	{
		tmp = hex[i] >> 4;
		if(tmp > 9)
			*str++ = tmp + 0x37;
		else
			*str++ = tmp + '0';
		
		tmp = hex[i] & 0xF;
		if(tmp > 9)
			*str++ = tmp + 0x37;
		else
			*str++ = tmp + '0';
	}
}

static int StrToHex(char *hex, const char *str)
{
	int hex_len = strlen(str)/2;
	unsigned char tmp_val = 0;
	int i = 0;
	
	for(i = 0; i < hex_len; i++)
	{
		tmp_val = ((str[2 * i] > '9') ? (str[2 * i] - 0x37) : (str[2 * i] - '0'));
		*hex = tmp_val << 4;
		tmp_val = ((str[2 * i + 1] > '9') ? (str[2 * i + 1] - 0x37) : (str[2 * i + 1] - '0'));
		*hex++ |= tmp_val;
	}
	return hex_len;
}



extern u32  sys1mstick;
#define GetSysTick()  sys1mstick;

#define MAXFIELD	250


uint8_t RecvGPRSData(uint8_t *buffer, uint8_t *pDat)
{
	uint16_t i, len;
	uint8_t  Dat[MAXFIELD];
	GetField(buffer,Dat,3,MAXFIELD);
	len = atoi((char *)Dat);
	
	GetField(buffer,Dat,4,MAXFIELD);
	StrToHex(pDat,Dat);
	USARTx_printf(USART1," \r\n%s","Rev Dat: ");
	USARTx_printf(USART1," %s",pDat);
	#if 0
	for(i=0; i < len; i++)
	{
	  USARTx_printf(USART1," %d",pDat[i]);
	}
	USARTx_printf(USART1," \r\n");
	#endif
	return len;
}

//ע�������ɹ�֮��ѭ���������ݡ�
void BC95_Test_Demo(void)
{
	uint8_t k, ret,len;
   u32  tick = 0,start_tick=0;
	
	
	start_tick = GetSysTick();	
#ifdef BC95_PWR_ON_TEST
	BC95_power_on();
#endif
	if(query_net_status())
	{
		creat_UDP_socket(UE_LOCAL_UDP_PORT);
		
		while(1)
		{
			tick++;
			delay_ms(1000);
			printf("tick:%d\r\n",tick);
			
			//һ�����ϱ�һ������
			if (tick%20 == 0)
			{	
				char buf[100]={0},buf_len[4]={0},*p,sub[10]={0};
				char sub2[10]={0};     //ʪ��
				
				tick = 0;
				SHT2x_Test();
				printf("\r\n�¶ȣ�%.5f\r\n",g_sht2x_param.TEMP_HM);
				printf("ʪ�ȣ�%.5f\r\n",g_sht2x_param.HUMI_HM);
				printf("\r\n�¶ȣ�%.5f\r\n",g_sht2x_param.TEMP_POLL);
				printf("ʪ�ȣ�%.5f\r\n",g_sht2x_param.HUMI_POLL);

				p = buf;
				//���¶ȸ�ֵ��sub
				sprintf(sub,"%.5f",g_sht2x_param.TEMP_HM,buf);
				//��ʪ�ȸ�ֵΪsub2
				sprintf(sub2,"%.5f",g_sht2x_param.HUMI_HM,buf);
				
				strcat(buf,sub);	
				strcat(buf,",");   //�����Զ��Ÿ���
				strcat(buf,sub2);
				memset(sub,0,sizeof(sub));
				memset(sub2,0,sizeof(sub2));
				#if 0
				sprintf(sub,"%.5f",g_sht2x_param.HUMI_HM,buf);
				strcat(buf,sub);
				memset(sub,0,sizeof(sub));
				sprintf(sub,"%.5f",g_sht2x_param.TEMP_POLL,buf);
				strcat(buf,sub);
				memset(sub,0,sizeof(sub));
				sprintf(sub,"%.5f",g_sht2x_param.HUMI_POLL,buf);
				strcat(buf,sub);
				memset(sub,0,sizeof(sub));
				#endif
				len = strlen(buf);
				sprintf(buf_len,"%d",len);

				send_UDP_msg("0",SERVER_HOST_UDP_IP,SERVER_HOST_UDP_PORT,buf_len,buf);
				
			}	
//			led_send_blink = 6;

				if (GPRSRecvFlag == 1)	// ???GPRS??
				{
					GPRSRecvFlag = 0;
					//if ( LookForStr(GPRSRecvBuffer, RecvSignal) != FULL )
						
					if(BC95_check_ack(RecvSignal))
					{
						
							usart2_rcvd_flag=0;
							usart2_rcvd_len = 0;
							GPRSRecvFlag = 0;						

							USARTx_printf(USART2,"%s\r\n",AT_NSORF);
						  USARTx_printf(USART1,"%s\r\n",AT_NSORF);
						
						  
					}
					else if (BC95_check_ack(RealSingal))
					{
						RecvGPRSData(usart2_rcvd_buf,GPRSRecvBuffer);
						
							usart2_rcvd_flag=0;
							usart2_rcvd_len = 0;
							GPRSRecvFlag = 0;	

					}
					
					memset(GPRSRecvBuffer, 0, GPRS_BUFFER_SIZE);
				}			
		}
	}
}


