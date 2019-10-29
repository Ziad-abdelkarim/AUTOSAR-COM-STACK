/********************************************************************************************************************************
 **
 FileName:                   CanSM.c
 AUTOSAR Version:            4.2.2
                                                                                                                                                                          **
 ********************************************************************************************************************************/
/********************************************************************************************************************************
 **                                                          Includes                                                                                                **
 ********************************************************************************************************************************/
#include "CanSM.h"
/********************************************************************************************************************************
 **                                                       Global Variables                                                                                       **
 ********************************************************************************************************************************/










 /********************************************************************************************************************************
**                                                        Local Functions                                                                                        **
*********************************************************************************************************************************/












/********************************************************************************************************************************
**                                                      Functions  definition                                                                        **
*********************************************************************************************************************************/
/*********************************************************************************************************************************
 Service name:                                                  CanSM_ControllerBusOff
 Service ID[hex]:                                               0x04
 Sync/Async:                                                    Synchronous
 Reentrancy:                                                    Reentrant
 Parameters (in):                                               ControllerId      --> CAN controller, which detected a bus-off event
 Parameters (out):                                              None
 Return value:                                                  None
 Description:                                                   This callback function notifies the CanSM about a bus-off event on a certain CAN
                                                                controller, which needs to be considered with the specified bus-off recovery
                                                                handling for the impacted CAN network.
 *******************************************************************************************************************************/


void CanSM_ControllerBusOff(uint8 ControllerId)
{

}

/*********************************************************************************************************************************
 Service name:                                                  CanSM_ControllerModeIndication
 Service ID[hex]:                                               0x07
 Sync/Async:                                                    Synchronous
 Reentrancy:                                                    Reentrant
 Parameters (in):                                               ControllerId CAN controller, whose mode has changed
                                                                ControllerMode Notified CAN controller mode
 Parameters (out):                                              None
 Return value:                                                  None
 Description:                                                   This callback shall notify the CanSM module about a CAN
                                                                controller mode change.
 *******************************************************************************************************************************/
/*void CanSM_ControllerModeIndication(uint8 ControllerId,CanIf_ControllerModeType ControllerMode)
{

}*/
