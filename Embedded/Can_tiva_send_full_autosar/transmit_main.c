#include <stdio.h>
#include <stdlib.h>

#include "Com.h"
#include "PduR.h"
#include "CanIf.h"
#include <stdbool.h>
#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/can.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/rom.h"

#include "uart.h"
typedef struct {
	uint32 clockSource;
	uint32 msgBuffer; //In case of receive
	uint32 targetId; //In case of receive
	uint32 port_TX;
	uint32 port_RX;
	uint32 port_RX_SS; //port_RX as source signal

} Can_ControllerConfigType;
typedef struct {
	Can_ControllerConfigType **canx_config;
} Can_ConfigType;
extern Can_ConfigType CanConfig;

void Can_Init(const Can_ConfigType* Config);


int main() {
	/*
	Initialize all the modules
	*/
  ConfigureUART();
	
  SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
  InitConsole();
	PduR_Init(&PBPduRConfig);
	CanIf_Init(&canIf_Config);
  //Init CAN module
	Can_Init(&CanConfig);
	
	//assuming msgId = 7, hth = 0, driverId = 1, interfacId = 2
	//msgId: 11 bits, hth: 8 bits, driverId: 4 bits, interfaceId: 4 bits
	while(1) {
		uint32 Id = ((uint32)7 << 16) | ((uint32)0 << 8) | ((uint32)1 << 4) | 2;
		uint8 length = 8;
		char buff[21];
		UARTgets(buff, 21);
		uint64 lmsg = atoll(buff);
		uint8 *msg = (uint8*)(&lmsg);
		ComSendSignal(Id, msg, length);
	}
	

	return 0;
}
