// ethernet_communication.h

#ifndef IO_ETHERNET_H
#define IO_ETHERNET_H

#include <stdint.h>
#include "stm32f7xx_hal_conf.h"
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_eth.h"

typedef enum {
    IO_Ethernet_MIN = 0,
    IO_Ethernet_RMII_REF_CLK,     //PA1
    IO_Ethernet_RMII_CSR_DV,      //PA7
    IO_Ethernet_RMII_RXDl,        //PC5
	IO_Ethernet_RMII_RXD0,        //PC4
	IO_Ethernet_RMII_TX_EN,       //PB11
	IO_Ethernet_RMII_TXD0,        //PG13
	IO_Ethernet_RMII_TXD1,        //PG14
	IO_Ethernet_ETH_MDC,          //PC1
	IO_Ethernet_ETH_MDIO,         //PA2
    IO_Ethernet_MAX
} IO_Ethernet_ENUM;


void IO_Ethernet_Init(void);

void IO_Ethernet_Open(void);

void IO_Ethernet_Close(void);

void IO_Ethernet_Transmit(uint8_t* data, uint16_t length);

void IO_Ethernet_Receive(uint8_t* buffer, uint16_t timeout);

#endif  // ETHERNET_COMMUNICATION_H
