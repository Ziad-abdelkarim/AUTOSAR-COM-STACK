<<<<<<< Updated upstream
/********************************************************************************************************************************
**

FileName:                          Can.c
AUTOSAR Version:            4.2.2
																																															**
********************************************************************************************************************************/

/********************************************************************************************************************************
**                                                                       includes                                                                                         **
********************************************************************************************************************************/
#include "Can_GeneralTypes.h"
#include "Std_Types.h"
#include "Can.h"





=======




typedef struct{
  uint8_t MessageObjectNumber ,
  bool confirmation,
  PDUIDTYPE PduId
}MessageObjectType;

MessageObjectType MessageObject[NUMBER_OF_CONTROLLERS][NUMBER_OF_HOH][MAX_NUMBER_OF_HANDLERS];

typedef enum  {
  CAN_READY,
	CAN_UNINIT
}CanDriveState;

const CanHardwareObject* Can_GetHardwareObjectOfHTH(Can_HwHandleType HTH){
  const CanHardwareObject *ADDRESS;
  for(uint8 HOI=0;HOI<NUMBER_OF_HOH;HOI++){
    if(Can.CanConfigSet.CanHardwareObject[HOI].CanObjectId==HTH){
      ADDRESS=&CanConfigSet.CanHardwareObject[HOI];
      return ADDRESS;
    }
  }
}



/*********************************************************************************************************************************
Service name:                                                Can_Write
Service ID[hex]:                                             0x06
Sync/Async:                                                  Synchronous
Reentrancy:                                                  Reentrant (thread-safe)
Parameters (in):                                             Hth ---> information which HW-transmit handle shall be used for transmit.
                                                                      Implicitly this is also the information about the controller to use
                                                                      because the Hth numbers are unique inside one hardware unit.
                                                             PduInfo ---> Pointer to SDU user memory, DLC and Identifier.
Parameters (inout):                                          None
Parameters (out):                                            None
Return value:                                                Can_ReturnType
Description:
                                                             This function is called by CanIf to pass a CAN message to CanDrv for
                                                             transmission.
*******************************************************************************************************************************/

Can_ReturnType Can_Write(Can_HwHandleType Hth,const Can_PduType* PduInfo)
{



      tCANMsgObject  sCANMessage;
      uint8 id =0;
  /*
  4- The function Can_Write shall return CAN_BUSY if a preemptive call of Can_Write has been issued, that could not be handled reentrant
  (i.e. a call with the same HTH). ⌋ (SRS_BSW_00312, SRS_Can_01049)[SWS_Can_00214]
  */



  /*
  5- The function Can_Write shall be non-blocking.[SWS_Can_00275]
  */


      /*
      6- If default error detection for the Can module is enabled: The function Can_Write shall raise the error CAN_E_UNINIT and shall return
      CAN_NOT_OK if the driver is not yet initialized. ⌋ ()[SWS_Can_00216]
      */
      if(CanDriveState==CAN_UNINIT)
      {
        //  CanDevolpmentErrorType=CAN_E_UNINIT;
            return CAN_NOT_OK;
      }else
      {

        /*
        7-If default error detection for the Can module is enabled: The function Can_Write shall raise the error CAN_E_PARAM_HANDLE and shall
        return CAN_NOT_OK if the parameter Hth is not a configured Hardware Transmit Handle. ⌋ ()[SWS_Can_00217]
        */
                const CanHardwareObject* CurrentHObj = Can_GetHardwareObjectOfHTH(Hth);
                if(CurrentHObj->CanObjectType==RECEIVE)
                {
                    //  CanDevolpmentErrorType=CAN_E_PARAM_HANDLE;
                        return CAN_NOT_OK;
                }else
                {
                    /*
                    1- The function Can_Write first checks if the hardware transmit object that is identified
                     by the HTH is free and if another Can_Write is ongoing for the same HTH. [SWS_Can_00212]
                    */
                    for(uint8 ObjInd = 0 ; ObjInd < CurrentHObj->CanHwObjectCount;ObjInd++)
                    {
                          if(MessageObject[CurrentHObj->CanControllerRef->CanControllerId][CurrentHObj->CanObjectId][ObjInd].confirmation==true)
                          {
                              id = ObjInd;
                              break;
                          }else
                          {
                              //is not free
                              /*
                              3- The function Can_Write shall perform no actions if the hardware transmit object is busy with another transmit request for an L-PDU:
                              [SWS_Can_00213]
                                    a. The transmission of the other L-PDU shall not be cancelled and the function Can_Write is left without any actions.
                                    b. The function Can_Write shall return CAN_BUSY.⌋ (SRS_Can_01049).
                              */
                              return CAN_BUSY;


                          }

                    }
                    //is free
                    /*
                    2- if HTH is free :
                          a. The mutex for that HTH is set to ‘signaled’
                          b.The ID, DLC and SDU are put in a format appropriate for the hardware (if necessary) and copied in the appropriate
                            hardware registers/buffers.
                          c.All necessary control operations to initiate the transmit are done
                          d.The mutex for that HTH is released
                          e.The function returns with CAN_OK⌋ (SRS_Can_01049)
                    */
                    MessageObject[CurrentHObj->CanControllerRef->CanControllerId][CurrentHObj->CanObjectId][Id].confirmation=false;
                    /*
                    9. If default error detection for CanDrv is enabled: Can_Write() shall raise CAN_E_PARAM_POINTER and shall return CAN_NOT_OK
                    if the parameter PduInfo is a null pointer. ⌋ () [SWS_CAN_00219]
                    */
                    if(PduInfo==NULL)
                    {

                        //  CanDevolpmentErrorType=CAN_E_PARAM_POINTER;
                        MessageObject[CurrentHObj->CanControllerRef->CanControllerId][CurrentHObj->CanObjectId][Id].confirmation=true;

                        return CAN_NOT_OK;

                    }else
                    {
                            if(PduInfo->sdu==NULL)
                            {
                                /*
                                12- If default error detection for CanDrv is enabled: Can_Write() shall raise CAN_E_PARAM_POINTER and shall return CAN_NOT_OK
                                if the trigger transmit API is disabled for this hardware object
                                (CanTriggerTransmitEnable = FALSE) and the SDU pointer inside PduInfo is a null pointer. ⌋ ()[SWS_CAN_00505]
                                */
                                if(CurrentHObj->CanTriggerTransmitEnable==false)
                                {
                                    MessageObject[CurrentHObj->CanControllerRef->CanControllerId][CurrentHObj->CanObjectId][Id].confirmation=true;
                                    //CanDevolpmentErrorType=CAN_E_PARAM_POINTER;
                                    return Can_NOT_OK;
                                }else
                                {
                                      /*
                                      10- Can_Write() shall accept a null pointer as SDU (Can_PduType.Can_SduPtrType = NULL) if the trigger transmit API is enabled
                                      for this hardware object (CanTriggerTransmitEnable = TRUE). ⌋ ()[SWS_CAN_00503]
                                      */
                                      /*
                                      11- If the trigger transmit API is enabled for the hardware object, Can_Write() shall interpret a null pointer as SDU
                                      (Can_PduType.Can_SduPtrType = NULL) as request for using the trigger transmit interface.
                                      If so and the hardware object is free, Can_Write() shall call CanIf_TriggerTransmit() to acquire the PDU’s data. ⌋ ()[SWS_CAN_00504]
                                      */
                                      if(MessageObject[CurrentHObj->CanControllerRef->CanControllerId][CurrentHObj->CanObjectId][Id].confirmation==true)
                                      {
                                        //nadyi 3ala el api mn canif bta3t el transmitapi

                                        /*
                                        13- If default error detection for CanDrv is enabled:Can_Write() shall raise CAN_E_PARAM_POINTER and shall return CAN_NOT_OK
                                        if the trigger transmit API (CanIf_TriggerTransmit()) returns E_NOT_OK. ⌋ ()[SWS_CAN_00506]
                                        */

                                      }else
                                      {
                                        //ab2o shkolfko 7aga wla ayi btangan hna

                                      }


                                }

                            }else
                            {

                                  /*
                                  8- The function Can_Write shall return CAN_NOT_OK and if default error detection for the CAN module is enabled shall raise the error
                                  CAN_E_PARAM_DLC: [SWS_Can_00218]

                                        a. If the length is more than 64 byte.
                                        b. If the length is more than 8 byte and the CAN controller is not in CAN FD mode (no CanControllerFdBaudrateConfig).
                                        c. If the length is more than 8 byte and the CAN controller is in CAN FD mode (valid CanControllerFdBaudrateConfig),
                                            but the CAN FD flag in Can_PduType->id is not set (refer Can_IdType).⌋ ( SRS_Can_01005)
                                  */
                                  // full
                                  if(PduInfo->length<=8u)
                                  {
                                          // send data
                                          MessageObject[CurrentHObj->CanControllerRef->CanControllerId][CurrentHObj->CanObjectId][Id].PduId=PduInfo->swPduHandle;
                                          sCANMessage.ui32MsgID = PduInfo->id;
                                          sCANMessage.ui32MsgLen = PduInfo->length;
                                                      /*if(PduInfo->length<1U)
                                                      {
                                                            sCANMessage.ui32MsgLen = 1U;
                                                      }

                                                      else{
                                                          sCANMessage.ui32MsgLen=PduInfo->length;

                                                        }*/
                                          sCANMessage.pui8MsgData =PduInfo->sdu;
                                          sCANMessage.ui32MsgIDMask = 0U;
                                          if(CurrentHObj->CanIdType==EXTENDED)
                                          {
                                              sCANMessage.ui32Flags = MSG_OBJ_TX_INT_ENABLE|MSG_OBJ_EXTENDED_ID;
                                          }else
                                          {
                                                sCANMessage.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
                                          }
                                          CANMessageSet(CurrentHObj->CanControllerRef->CanControllerBaseAddress,
                                          MessageObject[CurrentHObj->CanControllerRef->CanControllerId][CurrentHObj->CanObjectId][Id].MessageObjectNumber, &sCANMessage, MSG_OBJ_TYPE_TX);
                                          MessageObject[CurrentHObj->CanControllerRef->CanControllerId][CurrentHObj->CanObjectId][Id].confirmation=true;
                                          return  CAN_OK;

                                  }else
                                  {
                                          // CanDevolpmentErrorType=CAN_E_PARAM_DLC
                                          MessageObject[CurrentHObj->CanControllerRef->CanControllerId][CurrentHObj->CanObjectId][Id].confirmation=true;
                                          return CAN_NOT_OK
                                  }

                            }

                      }










              }


      }














  /*
  14- The CAN Frame has to be sent according to the two most significant bits of Can_PduType->id. The CAN FD frame bit is only evaluated if CAN Controller
   is in CAN FD mode (valid CanControllerFdBaudrateConfig). ⌋ ()[SWS_CAN_00486]
  */

  /*
  15- If PduInfo->SduLength does not match possible DLC values CanDrv shall use the next higher valid DLC for transmission with initialization of unused
  bytes to the value of the corresponding CanFdPaddingValue (see ECUC_Can_00485). ⌋ ( SRS_Can_01160)[SWS_CAN_00502]
  */
>>>>>>> Stashed changes






<<<<<<< Updated upstream
/*********************************************************************************************************************************
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


=======
>>>>>>> Stashed changes
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
