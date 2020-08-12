/*
 * Setup.h
 *
 *  Created on: ??�/??�/????
 *      Author: PC
 */

#ifndef SETUP_H_
#define SETUP_H_

#include "Platform_Types.h"

/*Configuration Section*/

    /*Peripherals*/
    #define UART0_ENABLED 0
    #define UART1_ENABLED 1
    #define BAUD_UART0 115200
    #define BAUD_UART1 115200

    #define CAN0_ENABLED 1

    /*PORTS*/
    #define PORTA 1
    #define PORTB 1
    #define PORTC 1
    #define PORTD 0
    #define PORTE 0
    #define PORTF 1



/*AUTOSAR SPECIFIC CONFIGURATION*/

#define CONTROLLER_ID   (uint8)0
#define SIGNAL1_ID       (uint8)0
#define SIGNAL2_ID       (uint8)1
#define GROUPSIGNAL_ID  (uint8)0
#define SIGNALGROUP_ID  (uint8)0



/*Functions Prototypes Section*/

void UART_Setup(void);
void Port_Init(void);
void CAN_Setup(void);
void Timer_Setup(void);

#endif /* SETUP_H_ */
