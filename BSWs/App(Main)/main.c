/**
 * main.c
 */
/*
#include <stdint.h>
#include <stdbool.h>
#include <inc/tm4c123gh6pm.h>
#include <driverlib/timer.h>
#include <driverlib/interrupt.h>
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <driverlib/pwm.h>
#include <driverlib/pin_map.h>
#include <inc/hw_gpio.h>
*/

#include "PduR.h"
#include "Com.h"
#include "ComTeam_Types.h"
#include "Det.h"
#include "Std_Types.h"
#include "Setup.h"


extern PduR_PBConfigType PduR;
extern Com_Type Com ;
extern CanIf_ConfigType CanIf ;


void AUTOSAR_SETUP(void);

void main()
{

    uint8 Signal1Data = 0x00, Signal2Data= 0x55;
    uint16 GroupSignalData = 0xFFFF;
    const void* Signal1DataPtr = &Signal1Data;
    const void* Signal2DataPtr = &Signal2Data;
    const void* GroupSignalDataPtr = &GroupSignalData;

    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    Port_Init();
    UART_Setup();
    CAN_Setup();
    AUTOSAR_SETUP();

    uint8 Counter = 0;

    if(CanIf_SetControllerMode(CONTROLLER_ID, CANIF_CS_STARTED) == E_OK)
    {
        UARTprintf("CanIf_SetControllerMode E_OK\n");
        Can_MainFunction_Mode();
        while(1)
        {
            Com_MainFunctionTx();
            Counter++;
            SysCtlDelay(SysCtlClockGet()/3);
            if(Counter == 1)
            {
                if(Com_SendSignal(SIGNAL1_ID, Signal1DataPtr) == E_OK)
                {
                    UARTprintf("Com_SendSignal %d\n", Signal1Data);
                    Signal1Data ++;
                }
                if(Com_SendSignal(SIGNAL2_ID, Signal2DataPtr) == E_OK)
                {
                    UARTprintf("Com_SendSignal %d\n", Signal2Data);
                    Signal2Data ++;
                }
                Com_UpdateShadowSignal(GROUPSIGNAL_ID, GroupSignalDataPtr);
                if(Com_SendSignalGroup(SIGNALGROUP_ID) == E_OK)
                {
                    UARTprintf("Com_SendSignalGroup %d\n", GroupSignalData);
                    GroupSignalData--;
                }

                Counter = 0;
            }

        }
    }
    else
    {

    }
}



void AUTOSAR_SETUP(void){

    Can_Init(&Can);
    CanIf_Init(&CanIf);
    PduR_Init(&PduR);
    Com_Init(&(Com.ComConfig));

    return;
}
