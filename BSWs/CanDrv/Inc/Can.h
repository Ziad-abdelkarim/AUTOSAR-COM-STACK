/********************************************************************************************************************************
**

FileName:                          Can.h                                      
AUTOSAR Version:            4.2.2
																																															**
********************************************************************************************************************************/

/********************************************************************************************************************************
**                                                                       includes                                                                                  				       **
********************************************************************************************************************************/
#include "Can_GeneralTypes.h"
#include "ComStack_Types.h"
#include "Can_Cfg.h"
//These Are the Values of The COntrol Registers After a Reset Brought From TM4CGH6PM Data Sheet

#define RST_CANCTL                0x00000001
#define RST_CANBRPE               0x00000000
#define RST_CANTST                0x00000000
#define RST_CANBIT                0x00002301
#define RST_CANERR                0x00000000
#define 	CANCTL                0x000
#define 	CANIF1MCTL            0x038
#define 	CANIF1CMSK            0x024
#define 	CANIF1CRQ             0x020
//MACRO Used To Get the Value of a Desired Address By Calculating of both Base Address and Offset
#define   GET_ADDRESS_VAL(A,B)           *((volatile uint32_t *)((A)+(B)))
#define    CLEAR_BIT(A,B,P)                    *((volatile uint32_t *)((A)+(B))) & (~(1 << P))
#define    SET_BIT(A,B,P)                          *((volatile uint32_t *)((A)+(B)))|(1 << P))
#define     status_Initialization   (GET_ADDRESS_VAL(cancontrollerbaseadress,CANCTL))&CAN_CTL_INIT
boolean LogicalSleep[NUMBER_OF_CONTROLLERS];
enum  {CAN_UNINIT, CAN_READY}CanDriverState = CAN_UNINIT;

void Can_MainFunction_Mode(void);
void Can_MainFunction_Busoff(void);
Can_ReturnType    Can_SetControllerMode (uint8 Controller,Can_StateTransitionType Transition);
