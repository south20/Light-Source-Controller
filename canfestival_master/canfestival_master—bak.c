
/* File generated by gen_cfile.py. Should not be modified. */

#include "canfestival_master.h"

/**************************************************************************/
/* Declaration of mapped variables                                        */
/**************************************************************************/
UNS16 Controlword = 0x1F;		/* Mapped at index 0x6040, subindex 0x00 */
UNS16 Statusword = 0x0;		/* Mapped at index 0x6041, subindex 0x00 */

/**************************************************************************/
/* Declaration of value range types                                       */
/**************************************************************************/

#define valueRange_EMC 0x9F /* Type for index 0x1003 subindex 0x00 (only set of value 0 is possible) */
UNS32 Master_valueRangeTest (UNS8 typeValue, void * value)
{
  switch (typeValue) {
    case valueRange_EMC:
      if (*(UNS8*)value != (UNS8)0) return OD_VALUE_RANGE_EXCEEDED;
      break;
  }
  return 0;
}

/**************************************************************************/
/* The node id                                                            */
/**************************************************************************/
/* node_id default value.*/
UNS8 Master_bDeviceNodeId = 0x00;

/**************************************************************************/
/* Array of message processing information */

const UNS8 Master_iam_a_slave = 0;

TIMER_HANDLE Master_heartBeatTimers[1];

/*
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

                               OBJECT DICTIONARY

$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/

/* index 0x1000 :   Device Type. */
                    UNS32 Master_obj1000 = 0x0;	/* 0 */
                    subindex Master_Index1000[] = 
                     {
                       { RO, uint32, sizeof (UNS32), (void*)&Master_obj1000, NULL }
                     };

/* index 0x1001 :   Error Register. */
                    UNS8 Master_obj1001 = 0x0;	/* 0 */
                    subindex Master_Index1001[] = 
                     {
                       { RO, uint8, sizeof (UNS8), (void*)&Master_obj1001, NULL }
                     };

/* index 0x1003 :   Pre-defined Error Field */
                    UNS8 Master_highestSubIndex_obj1003 = 0; /* number of subindex - 1*/
                    UNS32 Master_obj1003[] = 
                    {
                      0x0	/* 0 */
                    };
                    subindex Master_Index1003[] = 
                     {
                       { RW, valueRange_EMC, sizeof (UNS8), (void*)&Master_highestSubIndex_obj1003, NULL },
                       { RO, uint32, sizeof (UNS32), (void*)&Master_obj1003[0], NULL }
                     };

/* index 0x1005 :   SYNC COB ID. */
                    UNS32 Master_obj1005 = 0x40000080;	/* 1073741952 */
                    subindex Master_Index1005[] = 
                     {
                       { RW, uint32, sizeof (UNS32), (void*)&Master_obj1005, NULL }
                     };

/* index 0x1006 :   Communication / Cycle Period. */
                    UNS32 Master_obj1006 = 0x2000000;	/* SYNC周期 ，单位是us */
                    subindex Master_Index1006[] = 
                     {
                       { RW, uint32, sizeof (UNS32), (void*)&Master_obj1006, NULL }
                     };

/* index 0x100C :   Guard Time */ 
                    UNS16 Master_obj100C = 0x0;   /* 0 */

/* index 0x100D :   Life Time Factor */ 
                    UNS8 Master_obj100D = 0x0;   /* 0 */

/* index 0x1014 :   Emergency COB ID */
                    UNS32 Master_obj1014 = 0x80 + 0x00;   /* 128 + NodeID */

/* index 0x1016 :   Consumer Heartbeat Time */
                    UNS8 Master_highestSubIndex_obj1016 = 0;
                    UNS32 Master_obj1016[]={0};

/* index 0x1017 :   Producer Heartbeat Time */ 
                    UNS16 Master_obj1017 = 0x0;   /* 0 */

/* index 0x1018 :   Identity. */
                    UNS8 Master_highestSubIndex_obj1018 = 4; /* number of subindex - 1*/
                    UNS32 Master_obj1018_Vendor_ID = 0x0;	/* 0 */
                    UNS32 Master_obj1018_Product_Code = 0x0;	/* 0 */
                    UNS32 Master_obj1018_Revision_Number = 0x0;	/* 0 */
                    UNS32 Master_obj1018_Serial_Number = 0x0;	/* 0 */
                    subindex Master_Index1018[] = 
                     {
                       { RO, uint8, sizeof (UNS8), (void*)&Master_highestSubIndex_obj1018, NULL },
                       { RO, uint32, sizeof (UNS32), (void*)&Master_obj1018_Vendor_ID, NULL },
                       { RO, uint32, sizeof (UNS32), (void*)&Master_obj1018_Product_Code, NULL },
                       { RO, uint32, sizeof (UNS32), (void*)&Master_obj1018_Revision_Number, NULL },
                       { RO, uint32, sizeof (UNS32), (void*)&Master_obj1018_Serial_Number, NULL }
                     };

/* index 0x1280 :   Client SDO 1 Parameter. */
                    UNS8 Master_highestSubIndex_obj1280 = 3; /* number of subindex - 1*/
                    UNS32 Master_obj1280_COB_ID_Client_to_Server_Transmit_SDO = 0x601;	/* 1537 */
                    UNS32 Master_obj1280_COB_ID_Server_to_Client_Receive_SDO = 0x581;	/* 1409 */
                    UNS8 Master_obj1280_Node_ID_of_the_SDO_Server = 0x1;	/* 1 */
                    subindex Master_Index1280[] = 
                     {
                       { RO, uint8, sizeof (UNS8), (void*)&Master_highestSubIndex_obj1280, NULL },
                       { RW, uint32, sizeof (UNS32), (void*)&Master_obj1280_COB_ID_Client_to_Server_Transmit_SDO, NULL },
                       { RW, uint32, sizeof (UNS32), (void*)&Master_obj1280_COB_ID_Server_to_Client_Receive_SDO, NULL },
                       { RW, uint8, sizeof (UNS8), (void*)&Master_obj1280_Node_ID_of_the_SDO_Server, NULL }
                     };

/* index 0x1281 :   Client SDO 2 Parameter. */
                    UNS8 Master_highestSubIndex_obj1281 = 3; /* number of subindex - 1*/
                    UNS32 Master_obj1281_COB_ID_Client_to_Server_Transmit_SDO = 0x602;	/* 1410 */
                    UNS32 Master_obj1281_COB_ID_Server_to_Client_Receive_SDO = 0x582;	/* 1538 */
                    UNS8 Master_obj1281_Node_ID_of_the_SDO_Server = 0x2;	/* 0 */
                    subindex Master_Index1281[] = 
                     {
                       { RO, uint8, sizeof (UNS8), (void*)&Master_highestSubIndex_obj1281, NULL },
                       { RW, uint32, sizeof (UNS32), (void*)&Master_obj1281_COB_ID_Client_to_Server_Transmit_SDO, NULL },
                       { RW, uint32, sizeof (UNS32), (void*)&Master_obj1281_COB_ID_Server_to_Client_Receive_SDO, NULL },
                       { RW, uint8, sizeof (UNS8), (void*)&Master_obj1281_Node_ID_of_the_SDO_Server, NULL }
                     };

/* index 0x1400 :   Receive PDO 1 Parameter. */
                    UNS8 Master_highestSubIndex_obj1400 = 6; /* number of subindex - 1*/
                    UNS32 Master_obj1400_COB_ID_used_by_PDO = 0x181;	/* 512 */
                    UNS8 Master_obj1400_Transmission_Type = 0xFF;	/* 255 */
                    UNS16 Master_obj1400_Inhibit_Time = 0x0;	/* 0 */
                    UNS8 Master_obj1400_Compatibility_Entry = 0x0;	/* 0 */
                    UNS16 Master_obj1400_Event_Timer = 0x0;	/* 0 */
                    UNS8 Master_obj1400_SYNC_start_value = 0x0;	/* 0 */
                    subindex Master_Index1400[] = 
                     {
                       { RO, uint8, sizeof (UNS8), (void*)&Master_highestSubIndex_obj1400, NULL },
                       { RW, uint32, sizeof (UNS32), (void*)&Master_obj1400_COB_ID_used_by_PDO, NULL },
                       { RW, uint8, sizeof (UNS8), (void*)&Master_obj1400_Transmission_Type, NULL },
                       { RW, uint16, sizeof (UNS16), (void*)&Master_obj1400_Inhibit_Time, NULL },
                       { RW, uint8, sizeof (UNS8), (void*)&Master_obj1400_Compatibility_Entry, NULL },
                       { RW, uint16, sizeof (UNS16), (void*)&Master_obj1400_Event_Timer, NULL },
                       { RW, uint8, sizeof (UNS8), (void*)&Master_obj1400_SYNC_start_value, NULL }
                     };

/* index 0x1600 :   Receive PDO 1 Mapping. */
                    UNS8 Master_highestSubIndex_obj1600 = 1; /* number of subindex - 1*/
                    UNS32 Master_obj1600[] = 
                    {
                      0x60410010	/* 1614807056 */
                    };
                    subindex Master_Index1600[] = 
                     {
                       { RW, uint8, sizeof (UNS8), (void*)&Master_highestSubIndex_obj1600, NULL },
                       { RW, uint32, sizeof (UNS32), (void*)&Master_obj1600[0], NULL }
                     };

/* index 0x1800 :   Transmit PDO 1 Parameter. */
                    UNS8 Master_highestSubIndex_obj1800 = 0x6; /* number of subindex - 1*/
                    UNS32 Master_obj1800_COB_ID_used_by_PDO = 0x201;	/* 385 */
                    UNS8 Master_obj1800_Transmission_Type = 0xFF;	/* 255 */
                    UNS16 Master_obj1800_Inhibit_Time = 0x0;	/* 0 */
                    UNS8 Master_obj1800_Compatibility_Entry = 0x0;	/* 0 */
                    UNS16 Master_obj1800_Event_Timer = 0x05;	/* 0 */
                    UNS8 Master_obj1800_SYNC_start_value = 0x0;	/* 0 */
                    subindex Master_Index1800[] = 
                     {
                       { RO, uint8, sizeof (UNS8), (void*)&Master_highestSubIndex_obj1800, NULL },
                       { RW, uint32, sizeof (UNS32), (void*)&Master_obj1800_COB_ID_used_by_PDO, NULL },
                       { RW, uint8, sizeof (UNS8), (void*)&Master_obj1800_Transmission_Type, NULL },
                       { RW, uint16, sizeof (UNS16), (void*)&Master_obj1800_Inhibit_Time, NULL },
                       { RW, uint8, sizeof (UNS8), (void*)&Master_obj1800_Compatibility_Entry, NULL },
                       { RW, uint16, sizeof (UNS16), (void*)&Master_obj1800_Event_Timer, NULL },
                       { RW, uint8, sizeof (UNS8), (void*)&Master_obj1800_SYNC_start_value, NULL }
                     };

/* index 0x1A00 :   Transmit PDO 1 Mapping. */
                    UNS8 Master_highestSubIndex_obj1A00 = 1; /* number of subindex - 1*/
                    UNS32 Master_obj1A00[] = 
                    {
                      0x60400010	/* 1614872592 */
                    };
                    subindex Master_Index1A00[] = 
                     {
                       { RW, uint8, sizeof (UNS8), (void*)&Master_highestSubIndex_obj1A00, NULL },
                       { RW, uint32, sizeof (UNS32), (void*)&Master_obj1A00[0], NULL }
                     };

/* index 0x6040 :   Mapped variable Controlword */
                    subindex Master_Index6040[] = 
                     {
                       { RW, uint16, sizeof (UNS16), (void*)&Controlword, NULL }
                     };

/* index 0x6041 :   Mapped variable Statusword */
                    subindex Master_Index6041[] = 
                     {
                       { RO, uint16, sizeof (UNS16), (void*)&Statusword, NULL }
                     };

/**************************************************************************/
/* Declaration of pointed variables                                       */
/**************************************************************************/

const indextable Master_objdict[] = 
{
  { (subindex*)Master_Index1000,sizeof(Master_Index1000)/sizeof(Master_Index1000[0]), 0x1000},
  { (subindex*)Master_Index1001,sizeof(Master_Index1001)/sizeof(Master_Index1001[0]), 0x1001},
  { (subindex*)Master_Index1005,sizeof(Master_Index1005)/sizeof(Master_Index1005[0]), 0x1005},
  { (subindex*)Master_Index1006,sizeof(Master_Index1006)/sizeof(Master_Index1006[0]), 0x1006},
  { (subindex*)Master_Index1018,sizeof(Master_Index1018)/sizeof(Master_Index1018[0]), 0x1018},
  { (subindex*)Master_Index1280,sizeof(Master_Index1280)/sizeof(Master_Index1280[0]), 0x1280},
  { (subindex*)Master_Index1281,sizeof(Master_Index1281)/sizeof(Master_Index1281[0]), 0x1281},
  { (subindex*)Master_Index1400,sizeof(Master_Index1400)/sizeof(Master_Index1400[0]), 0x1400},
  { (subindex*)Master_Index1600,sizeof(Master_Index1600)/sizeof(Master_Index1600[0]), 0x1600},
  { (subindex*)Master_Index1800,sizeof(Master_Index1800)/sizeof(Master_Index1800[0]), 0x1800},
  { (subindex*)Master_Index1A00,sizeof(Master_Index1A00)/sizeof(Master_Index1A00[0]), 0x1A00},
  { (subindex*)Master_Index6040,sizeof(Master_Index6040)/sizeof(Master_Index6040[0]), 0x6040},
  { (subindex*)Master_Index6041,sizeof(Master_Index6041)/sizeof(Master_Index6041[0]), 0x6041},
};

const indextable * Master_scanIndexOD (CO_Data *d, UNS16 wIndex, UNS32 * errorCode)
{
	(void)d;
	int i;
	switch(wIndex){
		case 0x1000: i = 0;break;
		case 0x1001: i = 1;break;
		case 0x1005: i = 2;break;
		case 0x1006: i = 3;break;
		case 0x1018: i = 4;break;
		case 0x1280: i = 5;break;
		case 0x1281: i = 6;break;
		case 0x1400: i = 7;break;
		case 0x1600: i = 8;break;
		case 0x1800: i = 9;break;
		case 0x1A00: i = 10;break;
		case 0x6040: i = 11;break;
		case 0x6041: i = 12;break;
		default:
			*errorCode = OD_NO_SUCH_OBJECT;
			return NULL;
	}
	*errorCode = OD_SUCCESSFUL;
	return &Master_objdict[i];
}

/* 
 * To count at which received SYNC a PDO must be sent.
 * Even if no pdoTransmit are defined, at least one entry is computed
 * for compilations issues.
 */
s_PDO_status Master_PDO_status[1] = {s_PDO_status_Initializer};

const quick_index Master_firstIndex = {
  0, /* SDO_SVR */
  5, /* SDO_CLT */
  6, /* PDO_RCV */
  7, /* PDO_RCV_MAP */
  8, /* PDO_TRS */
  9 /* PDO_TRS_MAP */
};

const quick_index Master_lastIndex = {
  0, /* SDO_SVR */
  5, /* SDO_CLT */
  6, /* PDO_RCV */
  7, /* PDO_RCV_MAP */
  8, /* PDO_TRS */
  9 /* PDO_TRS_MAP */
};

const UNS16 Master_ObjdictSize = sizeof(Master_objdict)/sizeof(Master_objdict[0]); 

CO_Data masterObjdict_Data = CANOPEN_NODE_DATA_INITIALIZER(Master);

void canopen_send_sdo(uint16_t message_sdo[])
{
	  unsigned long abortCode=0;
    int8_t      nodeID=0;          /* ID       */
    uint16_t    index=0;           /* ?÷òy     */
    uint8_t     subIndex=0;        /* ×ó?÷òy   */
    uint8_t     dataType=uint8;    /* êy?YààDí ???°?D???aà?ó|??ê?uint8?í?éò?í¨ó? */
    uint32_t    count=4;           /* êy?Y3¤?è */
    uint8_t     data[4];    
    
	for(int i = 0; i < 9; ++i)
	{
		//printf("canopen_send_sdo ：message_sdo[i] = 0x%x \r\n", message_sdo[i]);
	}
	for(int i = 0; i < 9; ++i)
	{
		//printf("canopen_send_sdo ：message_sdo[0][i] = 0x%x \r\n", message_sdo[0][i]);
	}
    nodeID=(int8_t)(message_sdo[0]&0xf);  
    index=(uint16_t)((message_sdo[3]<<8)+message_sdo[2]);
    subIndex=(uint8_t)(message_sdo[4]);
    for(int i=0; i<4; i++)
        data[0+i]=(uint8_t)message_sdo[5+i];
	
	//printf("message_sdo[1] = 0x%x \r\n", message_sdo[1]);
	switch(message_sdo[1])
	{
		case 0x2f:
			count = 1;
			break;
		case 0x2b:
			count = 2;
			break;
		case 0x27:
			count = 3;
			break;
		case 0x23:
			count = 4;
			break;
		default:
			count = 4;
			break;
			
	}
    //d; nodeId; index; subIndex; count; dataType; data[4]; useBlockMode;
    //delay_ms(5000);
    writeNetworkDict(&masterObjdict_Data, nodeID, index, subIndex, count, dataType, &data, 0);
	if(getWriteResultNetworkDict(&masterObjdict_Data, 0x01, &abortCode) != SDO_FINISHED)
	{
		//printf("Result : Failed in getting information for slave 0x01, AbortCode :%4.4x \r\n", abortCode);
	}
	else
	{
		//printf("sendData OK! \r\n");
	}
	closeSDOtransfer(&masterObjdict_Data, 0x01, SDO_CLIENT);

    
}

void canopen_send_sdo2(uint16_t message_sdo[])
{
	  unsigned long abortCode=0;
    int8_t      nodeID=0;          /* ID       */
    uint16_t    index=0;           /* ?÷òy     */
    uint8_t     subIndex=0;        /* ×ó?÷òy   */
    uint8_t     dataType=uint8;    /* êy?YààDí ???°?D???aà?ó|??ê?uint8?í?éò?í¨ó? */
    uint32_t    count=4;           /* êy?Y3¤?è */
    uint8_t     data[4];    
    
	printf("canopen_send_sdo ：message_sdo2[i] \r\n");
	for(int i = 0; i < 9; ++i)
	{
		//printf("canopen_send_sdo ：message_sdo[i] = 0x%x \r\n", message_sdo[i]);
	}
	for(int i = 0; i < 9; ++i)
	{
		//printf("canopen_send_sdo ：message_sdo[0][i] = 0x%x \r\n", message_sdo[0][i]);
	}
    nodeID=(int8_t)(message_sdo[0]&0xf);  
    index=(uint16_t)((message_sdo[3]<<8)+message_sdo[2]);
    subIndex=(uint8_t)(message_sdo[4]);
    for(int i=0; i<4; i++)
        data[0+i]=(uint8_t)message_sdo[5+i];
	
	//printf("message_sdo[1] = 0x%x \r\n", message_sdo[1]);
	switch(message_sdo[1])
	{
		case 0x2f:
			count = 1;
			break;
		case 0x2b:
			count = 2;
			break;
		case 0x27:
			count = 3;
			break;
		case 0x23:
			count = 4;
			break;
		default:
			count = 4;
			break;
			
	}
    //d; nodeId; index; subIndex; count; dataType; data[4]; useBlockMode;
    //delay_ms(5000);
    writeNetworkDict(&masterObjdict_Data, nodeID, index, subIndex, count, dataType, &data, 0);
	if(getWriteResultNetworkDict(&masterObjdict_Data, 0x02, &abortCode) != SDO_FINISHED)
	{
		//printf("Result : Failed in getting information for slave 0x01, AbortCode :%4.4x \r\n", abortCode);
	}
	else
	{
		//printf("sendData OK! \r\n");
	}
	closeSDOtransfer(&masterObjdict_Data, 0x02, SDO_CLIENT);

    
}


