/********************************************************************************************************************************
**                                                                       includes                                                                                         **
********************************************************************************************************************************/
#include "Can_GeneralTypes.h"














/*********************************************************************************************************************************       
Service name:                                       Can_SetControllerMode
Service ID[hex]:                                                   0x03
Sync/Async:                                                  Asynchronous
Reentrancy:                                                  Non Reentrant
Parameters (in):                                    Controller ---> CAN controller for which the status shall be changed
                                                               Transition ---> Transition value to request new CAN controller state
Parameters (inout):                                               None
Parameters (out):                                                  None
Return value:                                                Can_ReturnType
Description:   
                                  This function performs software triggered state transitions of the CAN controller
                                  State machine.
*******************************************************************************************************************************/
Can_ReturnType Can_SetControllerMode (uint8 Controller,Can_StateTransitionType Transition) {
	
	
}
