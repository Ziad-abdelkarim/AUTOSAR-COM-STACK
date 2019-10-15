/********************************************************************************************************************************
**

FileName:                          Can.h                                      
AUTOSAR Version:            4.2.2
																																															**
********************************************************************************************************************************/
#ifndef CAN_H
#define CAN_H
/********************************************************************************************************************************
**                                                                       includes                                                                                  				       **
********************************************************************************************************************************/
#include "Can_GeneralTypes.h"
#include "ComStack_Types.h"
#include <CanIf_stub.h>
#include "Can_Cfg.h"
#define CONTROLLER_ZERO            0U
//These Are the Values of The COntrol Registers After a Reset Brought From TM4CGH6PM Data Sheet
#define CONTROLLER_ONE             1U
#define MIN_OBJECT_HANDLER         0x01U
#define MAX_OBJECT_HANDLER         0x20U
#define INDEX_ZERO                 0U
#define INDEX_ONE                  1U
#define RST_CANCTL                0x00000001
#define RST_CANTST                0x00000000
#define RST_CANBRPE               0x00000000
#define RST_CANBIT                0x00002301
#define RST_CANERR                0x00000000
#define CAN0_IF1CRQ_R           (*((volatile uint32_t *)0x40040020))
#define CAN0_IF1MCTL_R          (*((volatile uint32_t *)0x40040038))
#define 	CANCTL                0x000
#define 	CANIF1MCTL            0x038
#define 	CANIF1CMSK            0x024
#define 	CANIF1CRQ             0x020
#define WRNRD_Bit               7
#define NEWDAT_Bit              15
#define CANIF1CMSK_TXRQST_Bit   2
#define   GET_ADDRESS_VAL(A,B)           *((volatile uint32_t *)((A)+(B)))
#define    CLEAR_BIT(A,B,P)                    *((volatile uint32_t *)((A)+(B))) & (~(1 << P))
#define    SET_BIT(A,B,P)                          *((volatile uint32_t *)((A)+(B)))|(1 << P))
#define     status_Initialization   (GET_ADDRESS_VAL(cancontrollerbaseadress,CANCTL))&CAN_CTL_INIT
#define      GetBit(reg,n)              ((reg>>n)&1)
boolean LogicalSleep[NUMBER_OF_CONTROLLERS];
uint32 CanTimeoutDuration=5000;
void Can_MainFunction_Mode(void);
void Can_MainFunction_Busoff(void);
Can_ReturnType    Can_SetControllerMode (uint8 Controller,Can_StateTransitionType Transition);
void Can0_InterruptHandler(void);
Can_ReturnType    Can_Write( Can_HwHandleType Hth,const Can_PduType* PduInfo);
void Can_EnableControllerInterrupts(uint8 Controller);
void Can1_InterruptHandler(void);
void Can_DisableControllerInterrupts(uint8 Controller);
void Can_Init(const Can_ConfigType* Config);
void CanIf_RxIndication(const Can_HwType* Mailbox,const PduInfoType* PduInfoPtr);
#endif /* CAN_H */

Std_ReturnType Can_SetBaudrate( uint8 Controller, uint16 BaudRateConfigID );