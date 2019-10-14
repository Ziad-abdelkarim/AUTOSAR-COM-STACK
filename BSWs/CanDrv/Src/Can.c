/********************************************************************************************************************************
**

FileName:                          Can.c                                    
AUTOSAR Version:            4.2.2
																																															**
********************************************************************************************************************************/

/********************************************************************************************************************************
**                                                                       includes                                                                                         **
********************************************************************************************************************************/
#include "Can.h"









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
Can_ReturnType    Can_SetControllerMode (uint8 Controller,Can_StateTransitionType Transition) {
	uint32 cancontrollerbaseadress,Itration=0 ;
	cancontrollerbaseadress=Can.CanConfigSet.CanController[Controller].CanControllerBaseAddress;
	if(CanDriverState == CAN_UNINIT){
		CanDevelopmentError=CAN_E_UNINIT;
		return CAN_NOT_OK;
		
	}
/**[SWS_Can_00261] [ The function Can_SetControllerMode(CAN_T_START) shall
set the hardware registers in a way that makes the CAN controller participating on
the network.]**/
else	if ( Transition ==CAN_T_START)
	{
		if(status_Initialization){
/**[SWS_Can_00262] [ The function Can_SetControllerMode(CAN_T_START) shall
wait for limited time until the CAN controller is fully operational. Compare to
SWS_Can_00398.]**/
		while( CanTimeoutDuration & status_Initialization){
			CanTimeoutDuration--;
			Can_EnableControllerInterrupts(Controller);
		    CANEnable(cancontrollerbaseadress); 
		}
		   return CAN_OK;		   
		}
/**[SWS_Can_00409]  When the function Can_SetControllerMode(CAN_T_START)
is entered and the CAN controller is not in state STOPPED it shall detect a invalid
state transition (Compare to SWS_Can_00200).**/
		else{
			CanDevelopmentError=CAN_E_TRANSITION;
			return CAN_NOT_OK;
		}
	}
	
/**	[SWS_Can_00263] [ The function Can_SetControllerMode(CAN_T_STOP) shall set
the bits inside the CAN hardware such that the CAN controller stops participating on
the network.]**/
	else if (Transition ==CAN_T_STOP){
		if(!status_Initialization){
/**[SWS_Can_00264] [ The function Can_SetControllerMode(CAN_T_STOP) shall
wait for a limited time until the CAN controller is really switched off. Compare to
SWS_Can_00398.]**/
		while( CanTimeoutDuration & (!status_Initialization)){
			CanTimeoutDuration--;
/**[SWS_Can_00282] [ The function Can_SetControllerMode(CAN_T_STOP) shall
cancel pending messages. ]*/			
			for(Itration=0x01;Itration<=0x20;Itration){
			CLEAR_PIN(cancontrollerbaseadress,CANIF1CMSK,7); 
			CLEAR_PIN(cancontrollerbaseadress,CANIF1CMSK,2); 
			CLEAR_PIN(cancontrollerbaseadress,CANIF1MCTL,8); 
			GET_ADDRESS_VAL(cancontrollerbaseadress,CANIF1CRQ) = Itration;
			}
			Can_DisableControllerInterrupts(Controller);
		    CANDisable(cancontrollerbaseadress); 
		}
		   return CAN_OK;		   
		}
/**[SWS_Can_00410] [ When the function Can_SetControllerMode(CAN_T_STOP) is
entered and the CAN controller is neither in state STARTED nor in state STOPPED,
it shall detect a invalid state transition (Compare to SWS_Can_00200).] */
		else{
			CanDevelopmentError=CAN_E_TRANSITION;
			return CAN_NOT_OK;
		}
	}

/**[SWS_Can_00265] [ The function Can_SetControllerMode(CAN_T_SLEEP) shall
set the controller into sleep mode.]**/
	else if (Transition ==CAN_T_SLEEP){
/**[SWS_Can_00290] [ If the CAN HW does not support a sleep mode, the function
Can_SetControllerMode(CAN_T_SLEEP) shall set the CAN controller to the logical
sleep mode.]**/
		if(!status_Initialization & ! LogicalSleep[Controller]){
		while( CanTimeoutDuration & (!status_Initialization)  & !(LogicalSleep[Controller])){
			CanTimeoutDuration--;
			Can_DisableControllerInterrupts(Controller);
		    CANDisable(cancontrollerbaseadress); 
			LogicalSleep[Controller]=1;
		}
		   return CAN_OK;		   
		}
/**[SWS_Can_00411] [ When the function Can_SetControllerMode(CAN_T_SLEEP)
is entered and the CAN controller is neither in state STOPPED nor in state SLEEP, it
shall detect a invalid state transition]**/
		else{
			CanDevelopmentError=CAN_E_TRANSITION;
			return CAN_NOT_OK;
		}
	}
	
/**[SWS_Can_00267] [If the CAN HW does not support a sleep mode, the function
Can_SetControllerMode(CAN_T_WAKEUP) shall return from the logical sleep mode,but have no effect to the CAN controller state (as the controller is already in stopped
state).]**/
	else if (Transition ==CAN_T_WAKEUP){
			
		if(status_Initialization & LogicalSleep[Controller]){
/**[SWS_Can_00268] [ The function Can_SetControllerMode(CAN_T_WAKEUP) shall
wait for a limited time until the CAN controller is in STOPPED state. Compare to
SWS_Can_00398.]**/
		while( CanTimeoutDuration & status_Initialization & LogicalSleep[Controller]){
			CanTimeoutDuration--;
			Can_EnableControllerInterrupts(Controller);
		    CANEnable(cancontrollerbaseadress); 
			LogicalSleep[Controller]=0;
			
		}
		   return CAN_OK;		   
		}
/**[SWS_Can_00412]
[When the function Can_SetControllerMode(CAN_T_WAKEUP) is entered and the CAN controller is
neither in state SLEEP nor in state STOPPED, it shall detect a invalid state transition
(Compare to SWS_Can_00200).]**/
		else{
			CanDevelopmentError=CAN_E_TRANSITION;
			return CAN_NOT_OK;
		}
	
	}
	else {
		CanDevelopmentError=CAN_E_PARAM_CONTROLLER;
		return CAN_NOT_OK;
	}
	
}
