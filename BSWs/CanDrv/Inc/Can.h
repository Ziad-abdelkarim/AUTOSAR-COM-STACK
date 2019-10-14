//empty file

#include "ComStack_Types.h"
#include "Can_GeneralTypes.h"
#include "Can_Cfg.h"




//These Are the Values of The COntrol Registers After a Reset Brought From TM4CGH6PM Data Sheet
#define RST_CANCTL 0x00000001
#define RST_CANBRPE 0x00000000
#define RST_CANTST 0x00000000
#define RST_CANBIT 0x00002301
#define RST_CANERR 0x00000000

/*Number Of CAN Controllers Hardware Dependent : TM4CGH6PM Include Two CAN Controllers CAN0,CAN1  */


//MACRO Used To Get the Value of a Desired Address By Calculating of both Base Address and Offset
#define GET_ADDRESS_VAL(A,B) *((volatile uint32_t *)((A)+(B)))


/*
  * ********************************************Global Variables************************************************************
  */
boolean LogicalSleep[NUMBER_OF_CONTROLLERS];
enum  {CAN_UNINIT, CAN_READY}CanDriverState = CAN_UNINIT;

/*
 * *****************************************Functions Prototypes**********************************************************
 */
void Can_MainFunction_Mode(void);
void Can_MainFunction_Busoff(void);
