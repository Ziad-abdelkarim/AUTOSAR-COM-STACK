
/*
 * Can_MainFunctionRead.h
 *
 *  Created on: 10/10/2019
 *      Author: 3nani
 */

#include "Can_GeneralTypes.h"

#ifndef CAN_MAINFUNCTIONREAD_H_
#define CAN_MAINFUNCTIONREAD_H_



#define WRNRD_Bit               7
#define NEWDAT_Bit              15
#define CANIF1CMSK_TXRQST_Bit   2



#define CanController0          0
#define CanController1          1
#define MAX_NUMBER_OF_HANDLERS  5


#define SetBit(reg,n)              reg|=(1<<n)
#define RstBit(reg,n)              reg&=~(1<<n)
#define ToggleBit(reg,n)           reg^=(1<<n)
#define GetBit(reg,n)              ((reg>>n)&1)
#define SetNibH (reg)              reg|=0b11110000
#define SetNibL (reg)              reg|=0b00001111
#define RstNibH (reg)              reg&=0b00001111
#define RstNibL (reg)              reg&=0b11110000
#define GetNibH (reg)              reg=(reg>>4)
#define GetNibL (reg)              reg=((reg<<4)>>4)
#define SetNibValueH               (reg,value) reg&=(0b00001111)+
#define SetNibValueL               (reg,value) reg&=(0b11110000)+(0b0000n)



#endif /* CAN_MAINFUNCTIONREAD_H_ */

/**************************************************************************************************
Name:                                     CanIf_RxIndication
Reentrancy:                                 Reentrant
Sync/Async:                                Synchronous
Description:
                    This service indicates a successful reception of a received CAN RX LPDU
                    to the CanIf after passing all filters and validation checks.

                                                                                                                                       **
**************************************************************************************************/

void CanIf_RxIndication(const Can_HwType* Mailbox,const PduInfoType* PduInfoPtr);
