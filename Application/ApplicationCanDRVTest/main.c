

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


   // uint8 SignalData;
    //uint8 counter;

    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    Port_Init();
    UART_Setup();
    CAN_Setup();
    AUTOSAR_SETUP();



    if(CanIf_SetControllerMode(CONTROLLER_ID, CANIF_CS_STARTED) == E_OK)
    {
        //UARTprintf("CanIf_SetControllerMode E_OK\n");
        Can_MainFunction_Mode();
        while(1)
        {
            Com_MainFunctionRx();
           SysCtlDelay(SysCtlClockGet() / 100);
           /*counter++;
           if(counter == 10){
            if(Com_ReceiveSignal(2, &SignalData) == E_OK)
                {
                    UARTprintf("%d\n", SignalData);
                }
            if(Com_ReceiveSignal(3, &SignalData) == E_OK)
                {
                    UARTprintf("%d\n", SignalData);
                }
            counter=0;
           }*/

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

