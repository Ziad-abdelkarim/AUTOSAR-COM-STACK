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
#define 	CANCTL                    0x000
#define 	CANIF1MCTL            0x038
#define 	CANIF1CMSK           0x024
#define 	CANIF1CRQ             0x020
#define   GET_ADDRESS_VAL(A,B)           *((volatile uint32_t *)((A)+(B)))
#define    CLEAR_PIN(A,B,P)                    *((volatile uint32_t *)((A)+(B))) & (~(1 << P))
#define    SET_PIN(A,B,P)                          *((volatile uint32_t *)((A)+(B)))|(1 << P))
#define     status_Initialization   (GET_ADDRESS_VAL(cancontrollerbaseadress,CANCTL))&CAN_CTL_INIT

bool LogicalSleep[NUMBER_OF_CONTROLLER];
