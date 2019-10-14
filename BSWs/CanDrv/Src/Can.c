/********************************************************************************************************************************
**                                                                       includes                                                                                         **
********************************************************************************************************************************/
#include "Can_GeneralTypes.h"














/*********************************************************************************************************************************       
Service name:                                       Can_MainFunction_Mode
Service ID[hex]:                                                   0x0c
Sync/Async:                                                  Synchronous
Reentrancy:                                                  Non Reentrant
Parameters (in):                                                    Void
Parameters (inout):                                               Void
Parameters (out):                                                  Void
Return value:                                                        Void
Description:   
                                  This function performs the polling of CAN controller mode transitions.
*******************************************************************************************************************************/

void Can_MainFunction_Mode(void)
{

    uint8 Iteration; /* Used In for loops to specify which CAN controller we dealing with */

    uint32 temp_address_val; /*a Temporary Holder used Across The Function To Hold a Value of Certain Register*/
    boolean Init; /* booleanean Variable used To store The Value Of INIT Bit*/
    enum CanIf_ControllerModeType temp_case;
    if (CanDriverState == CAN_READY)
    {
        for (Iteration = 0; Iteration < NUMBER_OF_CONTROLLERS; Iteration++)
        {
            temp_address_val =
                    GET_ADDRESS_VAL((Can.CanConfigSet.CanController[Iteration].CanControllerBaseAddress),(CAN_O_CTL));
            temp_address_val &= CAN_CTL_INIT;
            if (temp_address_val)
            {
                Init = TRUE;
            }
            else
            {
                Init = FALSE;
            }

            if (GET_ADDRESS_VAL(
                    Can.CanConfigSet.CanController[Iteration].CanControllerBaseAddress,
                    CAN_O_CTL) == RST_CANCTL
                    &&
                    GET_ADDRESS_VAL(
                            Can.CanConfigSet.CanController[Iteration].CanControllerBaseAddress,
                            CAN_O_ERR) == RST_CANERR &&
                            GET_ADDRESS_VAL(Can.CanConfigSet.CanController[Iteration].CanControllerBaseAddress, CAN_O_BIT) == RST_CANBIT &&
                            GET_ADDRESS_VAL(
                                    Can.CanConfigSet.CanController[Iteration].CanControllerBaseAddress, CAN_O_TST) == RST_CANTST &&
                            GET_ADDRESS_VAL(Can.CanConfigSet.CanController[Iteration].CanControllerBaseAddress, CAN_O_BRPE) == RST_CANBRPE)
            {

                /*
                 * CAN controller state UNINIT
                 * The CAN controller is not initialized.
                 * All registers belonging to the CAN module are in reset state, CAN interrupts are disabled.
                 * The CAN Controller is not participating on the CAN bus.
                 */

                temp_case = CANIF_CS_UNINIT;
            }
            else if (Init == TRUE && LogicalSleep[Iteration] == TRUE)
            {
                /*
                 * CAN controller state SLEEP
                 * The hardware settings only differ from state STOPPED for CAN hardware that support a sleep mode
                 * The(wake-up over CAN bus directly supported by CAN hardware).
                 */
                temp_case = CANIF_CS_SLEEP;
            }
            else if (Init == TRUE && LogicalSleep[Iteration] == TRUE)
            {
                /*
                 * CAN controller state STOPPED
                 * In this state the CAN Controller is initialized but does not participate on the bus.
                 * In addition, error frames and acknowledges must not be sent.
                 */
                temp_case = CANIF_CS_STOPPED;
            }
            else if (Init == FALSE)
            {
                /*
                 * CAN controller state STARTED
                 * The controller is in a normal operation mode with complete functionality,
                 which means it participates in the network.
                 * For many controllers leaving the ‘initialization’-mode causes the controller to be started.
                 */
                temp_case = CANIF_CS_STARTED;
            }
            else
            {
                ;
            }

            if (CurrentCanState[Iteration] == temp_case)
            {
                ;
            }
            else
            {
                /*
                 * [SWS_Can_00373] ?The function Can_Mainfunction_Mode shall call the function
                 CanIf_ControllerModeIndication to notify the upper layer about a successful state transition
                 of the corresponding CAN controller referred by abstract CanIf ControllerId,
                 in case the state transition was triggered by function Can_SetControllerMode.
                 */

                //CanIf_ControllerModeIndication(i,temp_case);
                CurrentCanState[Iteration] = temp_case;
            }
        }

    }
    else
    {
        ;
    }
    return;
}







/*********************************************************************************************************************************       
Service name:                                       Can_MainFunction_Bussoff
Service ID[hex]:                                                   0x09
Sync/Async:                                                  Synchronous
Reentrancy:                                                  Non Reentrant
Parameters (in):                                                    Void
Parameters (inout):                                               Void
Parameters (out):                                                  Void
Return value:                                                        Void
Description:   
                                  This function performs the polling of bus-off events that are configured statically as 'to be polled'.
*******************************************************************************************************************************/


void Can_MainFunctionBusoff(void)
{

    /*
     * The Can module may implement the function Can_MainFunction_BusOff as empty define  in case no polling at
     * all is used.
     */

#if (CanBusoffProcessing == INTERRUPT)
    uint8 Iteration; /* Used In for loops to specify which CAN controller we dealing with */

    /* An Array consists of Controllers BASE ADDRESSES To Allow us with
     the help of the offsets and The GET_ADDRESS_Val To Get The Value of
     Specific Registers */

    uint32 temp_address_val;

    boolean BOFF; /*BussOff Indication variable*/

    for (Iteration = 0; Iteration < NUMBER_OF_CONTROLLERS; Iteration++)
    {
        temp_address_val = GET_ADDRESS_VAL(Can.CanConfigSet.CanController[Iteration].CanControllerBaseAddress, CAN_O_STS);
        temp_address_val &= CAN_STS_BOFF;
        if (temp_address_val)
        {
            BOFF = TRUE;
        }
        else
        {
            BOFF = FALSE;
        }
        if (BOFF == TRUE)
        {

            /*The CAN controller is in bus-off state
             * Notify  */

            //CanIf_ControllerBussoff(Iteration);
            // SetController Mode (Optional)
        }
        else
        {
            ; /*The CAN controller is not in bus-off state.*/
        }

    }
#endif

    return;
}
