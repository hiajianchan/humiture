#ifndef __BC95_H
#define __BC95_H

#include "usart.h"
#include "stdio.h"
#include "string.h"


#define BC95_PWR_ON_TEST
//#undef  BC95_PWR_ON_TEST

#define UE_LOCAL_UDP_PORT        "5631"
//#define SERVER_HOST_UDP_IP       "47.94.206.163"
#define SERVER_HOST_UDP_IP       "39.106.184.51"
#define SERVER_HOST_UDP_PORT     "1123"
//#define SERVER_HOST_UDP_PORT     "1234"

//Ä¬ÈÏÆµ¶Î
#define SET_UE_DEFAULT_BAND      "AT+NBAND=5"         
#define UE_DEFAULT_BAND          "+NBAND:5"


#define SET_UE_REBOOT            "AT+NRB"

#define QUERY_UE_BAND            "AT+NBAND?"
#define SET_UE_BAND_5            "AT+NBAND=5"
#define SET_UE_BAND_8            "AT+NBAND=8"

#define QUERY_UE_CONNECT_MODE    "AT+NCONFIG?"
#define SET_UE_AUTOCONNECT       "AT+NCONFIG=AUTOCONNECT,TRUE"
#define SET_UE_MANUALCONNECT     "AT+NCONFIG=AUTOCONNECT,FALSE"

#define QUERY_UE_FUNC            "AT+CFUN?"
#define SET_UE_FUNC_0            "AT+CFUN=0"
#define SET_UE_FUNC_1            "AT+CFUN=1"

#define QUERY_UE_SIGNAL_QTY      "AT+CSQ"

#define QUERY_UE_ATTACH_STATS    "AT+CGATT?"
#define UE_ATTACHED_STATS        "+CGATT:1"
#define SET_UE_ATTACH            "AT+CGATT=1"

#define QUERY_UE_EREG_STATS      "AT+CEREG?"
#define UE_EREGISTERING_STATS    "+CEREG:0,2"
#define UE_EREGISTERED_STATS     "+CEREG:0,1"
#define SET_UE_EREG              "AT+CEREG=1"

#define QUERY_UE_SCCON_STATS     "AT+CSQ"
#define SET_UE_SCCON             "AT+CSCON=1"



typedef struct
{
	char manufacture_id[12];
	char device_module[18];
	char firmware_version[30];
	char frequency_band[10];
} BC95_UE_INFO_typedef;


void BC95_power_on(void);
uint8_t* BC95_check_ack(char *str);
uint8_t  BC95_send_cmd(char *cmd,char *ack,uint16_t waittime);
uint8_t creat_UDP_socket(char* local_port);
uint8_t  send_UDP_msg(char *socket,char *hostIP,char *port,char *dataLen,char *data);
uint8_t* receive_udp(char *socket,char *dataLen);
void get_str_data(char* des,char pos,char len);
uint8_t query_net_status(void);
void BC95_Test_Demo(void);
void int2str(int n,char *str);


#endif

