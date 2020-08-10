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
    #define UART0_ENABLED 1
    #define UART1_ENABLED 0
    #define BAUD_UART0 115200
    #define BAUD_UART1 115200

    #define CAN0_ENABLED 1

    /*PORTS*/
    #define PORTA 1
    #define PORTB 1
    #define PORTC 0
    #define PORTD 0
    #define PORTE 0
    #define PORTF 1



/*AUTOSAR SPECIFIC CONFIGURATION*/

#define CONTROLLER_ID   (uint8)0




/*Functions Prototypes Section*/

void UART_Setup(void);
void Port_Init(void);
void CAN_Setup(void);
void Timer_Setup(void);

#endif /* SETUP_H_ */
