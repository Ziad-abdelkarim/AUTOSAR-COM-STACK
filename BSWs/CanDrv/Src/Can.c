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






/********************************************************************************************************************************
 **                                                                       Global Variables                                                                                         **
 ********************************************************************************************************************************/


static uint8 InterruptDisableCount[NUMBER_OF_CONTROLLERS];
static uint8 InterruptEnableCount[NUMBER_OF_CONTROLLERS];
static CanDriverStateType CanDriverState=CAN_UNINIT;
static MessageObjectType MessageObject[NUMBER_OF_CONTROLLERS][NUMBER_OF_HOH][MAX_NUMBER_OF_HANDLERS];




const CanHardwareObject* Can_GetHardwareObjectOfHTH(Can_HwHandleType HTH){
  const CanHardwareObject *ADDRESS;
  uint8 HOI;
  for( HOI=0;HOI<NUMBER_OF_HOH;HOI++){
    if(Can.CanConfigSet.CanHardwareObject[HOI].CanObjectId==HTH){
      ADDRESS=&Can.CanConfigSet.CanHardwareObject[HOI];
      break;
    }
  }
  return ADDRESS;
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
    uint8 Id =0;
    uint8 ObjInd;
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
    if(CanDriverState==CAN_UNINIT)
    {
        //  CanDevelopmentError=CAN_E_UNINIT;
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
            //  CanDevelopmentError=CAN_E_PARAM_HANDLE;
            return CAN_NOT_OK;
        }else
        {
            /*
                    1- The function Can_Write first checks if the hardware transmit object that is identified
                     by the HTH is free and if another Can_Write is ongoing for the same HTH. [SWS_Can_00212]
             */
            for(ObjInd = 0 ; ObjInd < CurrentHObj->CanHwObjectCount;ObjInd++)
            {
                if(MessageObject[CurrentHObj->CanControllerRef->CanControllerId][CurrentHObj->CanObjectId][ObjInd].Confirmation==true)
                {
                    Id = ObjInd;
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
            MessageObject[CurrentHObj->CanControllerRef->CanControllerId][CurrentHObj->CanObjectId][Id].Confirmation=false;
            /*
                    9. If default error detection for CanDrv is enabled: Can_Write() shall raise CAN_E_PARAM_POINTER and shall return CAN_NOT_OK
                    if the parameter PduInfo is a null pointer. ⌋ () [SWS_CAN_00219]
             */
            if(PduInfo==NULL)
            {

                //  CanDevelopmentError=CAN_E_PARAM_POINTER;
                MessageObject[CurrentHObj->CanControllerRef->CanControllerId][CurrentHObj->CanObjectId][Id].Confirmation=true;

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
                        MessageObject[CurrentHObj->CanControllerRef->CanControllerId][CurrentHObj->CanObjectId][Id].Confirmation=true;
                        //CanDevelopmentError=CAN_E_PARAM_POINTER;
                        return CAN_NOT_OK;
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
                        if(MessageObject[CurrentHObj->CanControllerRef->CanControllerId][CurrentHObj->CanObjectId][Id].Confirmation==true)
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
                        MessageObject[CurrentHObj->CanControllerRef->CanControllerId][CurrentHObj->CanObjectId][Id].Confirmation=true;
                        return  CAN_OK;

                    }else
                    {
                        // CanDevelopmentError=CAN_E_PARAM_DLC
                        MessageObject[CurrentHObj->CanControllerRef->CanControllerId][CurrentHObj->CanObjectId][Id].Confirmation=true;
                        return CAN_NOT_OK;
                    }

                }

            }










        }


    }
    return CAN_NOT_OK;
}

    /*
  14- The CAN Frame has to be sent according to the two most significant bits of Can_PduType->id. The CAN FD frame bit is only evaluated if CAN Controller
   is in CAN FD mode (valid CanControllerFdBaudrateConfig). ⌋ ()[SWS_CAN_00486]
     */

    /*
  15- If PduInfo->SduLength does not match possible DLC values CanDrv shall use the next higher valid DLC for transmission with initialization of unused
  bytes to the value of the corresponding CanFdPaddingValue (see ECUC_Can_00485). ⌋ ( SRS_Can_01160)[SWS_CAN_00502]
     */




/***********************************************************************************************
Service name:                     Can_DisableControllerInterrupts
Service ID[hex]:                               0x04
Sync/Async:                                 Synchronous
Reentrancy:                                  Reentrant
Parameters (in):           Controller --> CAN controller for which interrupts shall be disabled.
Parameters (inout):                            None
Parameters (out):                              None
Return value:                                  None
Description:               This function disables all interrupts for this CAN controller.
***********************************************************************************************/
void Can_DisableControllerInterrupts(uint8 Controller)
{
    if (CanDriverState!= CAN_READY) //check if can driver is not ready
    {
        CanDevelopmentError = CAN_E_UNINIT; // raise the error CAN_E_UNINIT if the driver not yet initialized
    }
    else
    {
        if (Controller >= NUMBER_OF_CONTROLLERS) //check parameter Controller is out of range
        {
            CanDevelopmentError = CAN_E_PARAM_CONTROLLER; //raise the error CAN_E_PARAM_CONTROLLER if the parameter Controller is out of range
        }
        else
        { //not found any error {the driver not yet initialized or parameter Controller is out of range}
            InterruptDisableCount[Controller]++; // increase a counter to make disable Controller Interrupts
            if (Controller == CONTROLLER_ZERO) //check which controllers (Controller0 or Controller1)
            { //Disable the CAN0 interrupt if it is enabled.
                if (IntIsEnabled(INT_CAN0)) //Returns TURE if a peripheral interrupt is enabled.
                {
                    /*When Can_EnableControllerInterrupts has been called several times,
                     *When Can_DisableControllerInterrupts must be called as many times before the interrupts are disabled. */
                    if (InterruptEnableCount[Controller] >= INDEX_ONE) //check how many times you make to disable Controller Interrupts
                    {
                        InterruptEnableCount[Controller]--; //incremental enable count until reach to zero
                    }
                    else
                    {
                        //MISRA
                    }
                    if (InterruptEnableCount[Controller] == INDEX_ZERO)
                    { //Disables the specified CAN controller interrupt sources. Only enabled interrupt sources can cause a processor interrupt.
                        CANIntDisable(
                                Can.CanConfigSet.CanController[Controller].CanControllerBaseAddress,
                                CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
                        // disable interrupts for CAN0
                        IntDisable(INT_CAN0);
                    }
                    else
                    {
                        //must call this function until make InterruptEnableCount[Controller] =ZERO to make disable Controller Interrupts
                    }
                }
                else
                {
                    //it is disabled
                }
            }
            else if(Controller == CONTROLLER_ONE)//check which controllers (Controller0 or Controller1)
            {
              //Disable the CAN1 interrupt if it is enabled.
                if (IntIsEnabled(INT_CAN1)) //Returns TURE if a peripheral interrupt is enabled.
                {
                    /*When Can_EnableControllerInterrupts has been called several times,
                     *When Can_DisableControllerInterrupts must be called as many times before the interrupts are disabled. */
                    if (InterruptEnableCount[Controller] >= INDEX_ONE) //check how many times you to make disable Controller Interrupts
                    {
                        InterruptEnableCount[Controller]--; //incremental enable count until reach to zero
                    }
                    else
                    {
                        //MISRA
                    }
                    if (InterruptEnableCount[Controller] == INDEX_ZERO)
                    { //Disables the specified CAN controller interrupt sources. Only enabled interrupt sources can cause a processor interrupt.
                        CANIntDisable(
                                Can.CanConfigSet.CanController[Controller].CanControllerBaseAddress,
                                CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
                        // enable to disable interrupts for CAN1
                        IntDisable(INT_CAN1);
                    }
                    else
                    {
                        //must call this function until make InterruptEnableCount[Controller] =ZERO to make disable
                    }
                }
                else
                {
                    //it is disabled
                }
            }
             else
                {
                //MISRA
                }
        }
    }
}

/***********************************************************************************************
Service name:                     Can_EnableControllerInterrupts
Service ID[hex]:                               0x05
Sync/Async:                                 Synchronous
Reentrancy:                                  Reentrant
Parameters (in):          Controller --> CAN controller for which interrupts shall be re-enabled
Parameters (inout):                            None
Parameters (out):                              None
Return value:                                  None
Description:               This function enables all interrupts for this CAN controller.
***********************************************************************************************/
void Can_EnableControllerInterrupts(uint8 Controller)
{
    if (CanDriverState!= CAN_READY) //check if can driver is not ready
    {
        CanDevelopmentError = CAN_E_UNINIT; // raise the error CAN_E_UNINIT if the driver not yet initialized
    }
    else
    {
        if (Controller >= NUMBER_OF_CONTROLLERS) //check parameter Controller is out of range
        {
            CanDevelopmentError = CAN_E_PARAM_CONTROLLER; //raise the error CAN_E_PARAM_CONTROLLER if the parameter Controller is out of range
        }
        else
        { //not found any error {the driver not yet initialized or parameter Controller is out of range}
            InterruptEnableCount[Controller]++; // increase a counter to make enable
            if (Controller == CONTROLLER_ZERO) //check which controllers (Controller0 or Controller1)
            {
                //enable the CAN0 interrupt if it is disabled.
                if (!IntIsEnabled(INT_CAN0)) //Returns TURE if a peripheral interrupt is enabled.
                {
                    /*When Can_DisableControllerInterrupts has been called several times,
                     *When  Can_EnableControllerInterrupts must be called as many times before the interrupts are re-enabled. */
                    if (InterruptDisableCount[Controller] >= INDEX_ONE) //check how many times you make to enable Controller Interrupts
                    {
                        InterruptDisableCount[Controller]--; //incremental disable count until reach to zero
                    }
                    else
                    {
                        //MISRA
                    }
                    if (InterruptDisableCount[Controller] == INDEX_ZERO)
                    { //This function enables specific interrupt sources of the CAN controller. Only enabled sources cause a processor interrupt.
                        CANIntEnable(
                                Can.CanConfigSet.CanController[Controller].CanControllerBaseAddress,
                                CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
                        // enable interrupts for CAN0
                        IntEnable(INT_CAN0);
                    }
                    else
                    {
                        //must call this function until make InterruptDisableCount[Controller]=ZERO to make enable Controller Interrupts
                    }
                }
                else
                {
                    //it is enabled
                }
            }
            else if(Controller == CONTROLLER_ONE)//check which controllers (Controller0 or Controller1)
            {
              //enable the CAN1 interrupt if it is disabled.
                if (!IntIsEnabled(INT_CAN1)) //Returns TURE if a peripheral interrupt is enabled.
                {
                    /*When Can_DisableControllerInterrupts has been called several times,
                     *When  Can_EnableControllerInterrupts must be called as many times before the interrupts are re-enabled. */
                    if (InterruptDisableCount[Controller] >= INDEX_ONE) //check how many times you make to enable Controller Interrupts
                    {
                        InterruptDisableCount[Controller]--; //incremental disable count  until reach to zero
                    }
                    else
                    {
                        //MISRA
                    }
                    if (InterruptDisableCount[Controller] == INDEX_ZERO)
                    { //This function enables specific interrupt sources of the CAN controller. Only enabled sources cause a processor interrupt.
                        CANIntEnable(
                                Can.CanConfigSet.CanController[Controller].CanControllerBaseAddress,
                                CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
                        // enable  interrupts for CAN1
                        IntEnable(INT_CAN1);
                    }
                    else
                    {
                        //must call this function until make InterruptDisableCount[Controller] =ZERO to make enable Controller Interrupts
                    }
                }
                else
                {
                    //it is enabled
                }
            }
            else
            {
            //MISRA
            }
        }
    }
}

/***********************************************************************************************
 Service name:                         Can0_InterruptHandler
 Sync/Async:                                 Synchronous
 Parameters (in):                               None
 Parameters (inout):                            None
 Parameters (out):                              None
 Return value:                                  None
 Description:                      Can0 Interrupt service routine
 ***********************************************************************************************/
void Can0_InterruptHandler(void)
{
    uint32 ui32Status;
    uint8 ui8NumberOfObjectHandler;
    uint8 ui8NumberOfObject;
    uint32 ui32ErrorFlag = INDEX_ZERO;

    tCANMsgObject ReceivedMessage;
    Can_HwType ReceiverMailBox;
    PduInfoType ReceiverPduInfo;
    /*
     Read the CAN interrupt status to find the cause of the interrupt
     CAN_INT_STS_CAUSE register values
     0x0000        = No Interrupt Pending
     0x0001-0x0020 = Number of message object that caused the interrupt
     0x8000        = Status interrupt
     all other numbers are reserved and have no meaning in this system
     */
    ui32Status = CANIntStatus(
            Can.CanConfigSet.CanController[CONTROLLER_ZERO].CanControllerBaseAddress,
            CAN_INT_STS_CAUSE);
    /*
     If this was a status interrupt acknowledge it by reading the CAN
     controller status register.
     */
    if (ui32Status == CAN_INT_INTID_STATUS)
    {
        CANIntClear(Can.CanConfigSet.CanController[CONTROLLER_ZERO].CanControllerBaseAddress,
                    ui32Status); // Clear the message object interrupt
        /*
         Read the controller status.  This will return a field of status
         error bits that can indicate various errors. Refer to the
         API documentation for details about the error status bits.
         The act of reading this status will clear the interrupt.
         */
        ui32Status = CANStatusGet(
                Can.CanConfigSet.CanController[CONTROLLER_ZERO].CanControllerBaseAddress,
                CAN_STS_CONTROL);
        ui32ErrorFlag |= ui32Status; //Add ERROR flags to list of current errors To be handled
        if (ui32ErrorFlag & CAN_STATUS_BUS_OFF) //check if CAN controller has entered a Bus Off state.
        {
            Can_SetControllerMode(CONTROLLER_ZERO, CAN_T_STOP); // [SWS_Can_00259] The function Can_Init shall set all CAN controllers in the state STOPPED.
            /*
             The CanIf module is notified with the function CanIf_ControllerBusOff after
             STOPPED state is reached referring to the corresponding CAN controller with the abstract CanIf ControllerId.⌋(SRS_Can_01055)
             */
            CanIf_ControllerBusOff(CONTROLLER_ZERO);
        }
        else
        {
            //MISRA
        }
    }
    else if ((ui32Status >= MIN_OBJECT_HANDLER) && (ui32Status <= MAX_OBJECT_HANDLER)) //check if ObjectHandler in range 1-->32 .
    {
        CANIntClear(Can.CanConfigSet.CanController[CONTROLLER_ZERO].CanControllerBaseAddress,
                    ui32Status);  // Clear the message object interrupt
        for (ui8NumberOfObjectHandler = INDEX_ZERO;
                ui8NumberOfObjectHandler < NUMBER_OF_HOH;
                ui8NumberOfObjectHandler++) //determine OfObjectHandler which we need
        {
            if (Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanControllerRef
                    == &Can.CanConfigSet.CanController[CONTROLLER_ZERO]) //check which CanController we use
            {
                if (Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanObjectType
                        == TRANSMIT)  //check if CanObjectType is transmit
                {
                    //determine all OfObjectHandler transmit
                    for (ui8NumberOfObject = INDEX_ZERO;
                            ui8NumberOfObject
                                    < Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanHwObjectCount;
                            ui8NumberOfObject++)
                    {
                        if (MessageObject[CONTROLLER_ZERO][ui8NumberOfObjectHandler][ui8NumberOfObject].MessageObjectNumber
                                == ui32Status) //check which MessageObjectNumber we need to transmit form it
                        {
                            //confirms a previously successfully processed transmission
                            MessageObject[CONTROLLER_ZERO][ui8NumberOfObjectHandler][ui8NumberOfObject].Confirmation =
                                    true;
                            /*
                             (SRS_Can_01009)
                             Note: The service CanIf_TxConfirmation() is implemented in CanIf and called
                             by the CanDrv after the CAN L-PDU has been transmitted on the CAN network.
                             */
                            CanIf_TxConfirmation(MessageObject[CONTROLLER_ZERO][ui8NumberOfObjectHandler][ui8NumberOfObject].PduId);
                        }
                        else
                        {
                            //MISRA
                        }
                    }
                }
                else if (Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanObjectType
                        == RECEIVE)  //check if CanObjectType is receive
                {
                    //determine all OfObjectHandler receive
                    for (ui8NumberOfObject = INDEX_ZERO;
                            ui8NumberOfObject
                                    < Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanHwObjectCount;
                            ui8NumberOfObject++)
                    {
                        if (MessageObject[CONTROLLER_ZERO][ui8NumberOfObjectHandler][ui8NumberOfObject].MessageObjectNumber
                                == ui32Status) //check which MessageObjectNumber we need to receive in it
                        {
                            CANMessageGet(
                                    Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanControllerRef->CanControllerBaseAddress,
                                    MessageObject[CONTROLLER_ZERO][ui8NumberOfObjectHandler][ui8NumberOfObject].MessageObjectNumber,
                                    &ReceivedMessage, INDEX_ONE);
                            ReceiverMailBox.Hoh = Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanObjectId;
                            ReceiverMailBox.ControllerId =
                                    Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanControllerRef->CanControllerId;
                            ReceiverMailBox.CanId = ReceivedMessage.ui32MsgID;
                            ReceiverPduInfo.SduDataPtr =
                                    ReceivedMessage.pui8MsgData;
                            ReceiverPduInfo.SduLength =
                                    ReceivedMessage.ui32MsgLen;
                            CanIf_RxIndication(&ReceiverMailBox, &ReceiverPduInfo);
                        }
                        else
                        {
                            //MISRA
                        }
                    }
                }
                else
                {
                    //MISRA
                }
            }
            else
            {
                //MISRA
            }
        }
    }
    else
    {
        //MISRA
    }
}

/***********************************************************************************************
 Service name:                         Can1_InterruptHandler
 Sync/Async:                                 Synchronous
 Parameters (in):                               None
 Parameters (inout):                            None
 Parameters (out):                              None
 Return value:                                  None
 Description:                      Can1 Interrupt service routine
 ***********************************************************************************************/
void Can1_InterruptHandler(void)
{

    uint32 ui32Status;
    uint8 ui8NumberOfObjectHandler;
    uint8 ui8NumberOfObject;
    uint32 ui32ErrorFlag = INDEX_ZERO;

    tCANMsgObject ReceivedMessage;
    Can_HwType ReceiverMailBox;
    PduInfoType ReceiverPduInfo;
    /*
     Read the CAN interrupt status to find the cause of the interrupt
     CAN_INT_STS_CAUSE register values
     0x0000        = No Interrupt Pending
     0x0001-0x0020 = Number of message object that caused the interrupt
     0x8000        = Status interrupt
     all other numbers are reserved and have no meaning in this system
     */
    ui32Status = CANIntStatus(
            Can.CanConfigSet.CanController[CONTROLLER_ONE].CanControllerBaseAddress,
            CAN_INT_STS_CAUSE);
    /*
     If this was a status interrupt acknowledge it by reading the CAN
     controller status register.
     */
    if (ui32Status == CAN_INT_INTID_STATUS)
    {
        CANIntClear(Can.CanConfigSet.CanController[CONTROLLER_ONE].CanControllerBaseAddress,
                    ui32Status); // Clear the message object interrupt
        /*
         Read the controller status.  This will return a field of status
         error bits that can indicate various errors. Refer to the
         API documentation for details about the error status bits.
         The act of reading this status will clear the interrupt.
         */
        ui32Status = CANStatusGet(
                Can.CanConfigSet.CanController[CONTROLLER_ONE].CanControllerBaseAddress,
                CAN_STS_CONTROL);
        ui32ErrorFlag |= ui32Status; //Add ERROR flags to list of current errors To be handled
        if (ui32ErrorFlag & CAN_STATUS_BUS_OFF) //check if CAN controller has entered a Bus Off state.
        {
            Can_SetControllerMode(CONTROLLER_ONE, CAN_T_STOP); // [SWS_Can_00259] The function Can_Init shall set all CAN controllers in the state STOPPED.
            /*
             The CanIf module is notified with the function CanIf_ControllerBusOff after
             STOPPED state is reached referring to the corresponding CAN controller with the abstract CanIf ControllerId.⌋(SRS_Can_01055)
             */
            CanIf_ControllerBusOff(CONTROLLER_ONE);
        }
        else
        {
            //MISRA
        }
    }
    else if ((ui32Status >= MIN_OBJECT_HANDLER) && (ui32Status <= MAX_OBJECT_HANDLER)) //check if ObjectHandler in range 1-->32 .
    {
        CANIntClear(Can.CanConfigSet.CanController[CONTROLLER_ONE].CanControllerBaseAddress,
                    ui32Status);  // Clear the message object interrupt
        for (ui8NumberOfObjectHandler = INDEX_ZERO;
                ui8NumberOfObjectHandler < NUMBER_OF_HOH;
                ui8NumberOfObjectHandler++) //determine OfObjectHandler which we need
        {
            if (Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanControllerRef
                    == &Can.CanConfigSet.CanController[CONTROLLER_ONE]) //check which CanController we use
            {
                if (Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanObjectType
                        == TRANSMIT)  //check if CanObjectType is transmit
                {
                    //determine all OfObjectHandler transmit
                    for (ui8NumberOfObject = INDEX_ZERO;
                            ui8NumberOfObject
                                    < Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanHwObjectCount;
                            ui8NumberOfObject++)
                    {
                        if (MessageObject[CONTROLLER_ONE][ui8NumberOfObjectHandler][ui8NumberOfObject].MessageObjectNumber
                                == ui32Status) //check which MessageObjectNumber we need to transmit form it
                        {
                            //confirms a previously successfully processed transmission
                            MessageObject[CONTROLLER_ONE][ui8NumberOfObjectHandler][ui8NumberOfObject].Confirmation =
                                    true;
                            /*
                             (SRS_Can_01009)
                             Note: The service CanIf_TxConfirmation() is implemented in CanIf and called
                             by the CanDrv after the CAN L-PDU has been transmitted on the CAN network.
                             */
                            CanIf_TxConfirmation(MessageObject[CONTROLLER_ONE][ui8NumberOfObjectHandler][ui8NumberOfObject].PduId);
                        }
                        else
                        {
                            //MISRA
                        }
                    }
                }
                else if (Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanObjectType
                        == RECEIVE)  //check if CanObjectType is receive
                {
                    //determine all OfObjectHandler receive
                    for (ui8NumberOfObject = INDEX_ZERO;
                            ui8NumberOfObject
                                    < Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanHwObjectCount;
                            ui8NumberOfObject++)
                    {
                        if (MessageObject[CONTROLLER_ONE][ui8NumberOfObjectHandler][ui8NumberOfObject].MessageObjectNumber
                                == ui32Status) //check which MessageObjectNumber we need to receive in it
                        {
                            CANMessageGet(
                                    Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanControllerRef->CanControllerBaseAddress,
                                    MessageObject[CONTROLLER_ONE][ui8NumberOfObjectHandler][ui8NumberOfObject].MessageObjectNumber,
                                    &ReceivedMessage, INDEX_ONE);
                            ReceiverMailBox.Hoh = Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanObjectId;
                            ReceiverMailBox.ControllerId =
                                    Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanControllerRef->CanControllerId;
                            ReceiverMailBox.CanId = ReceivedMessage.ui32MsgID;
                            ReceiverPduInfo.SduDataPtr =
                                    ReceivedMessage.pui8MsgData;
                            ReceiverPduInfo.SduLength =
                                    ReceivedMessage.ui32MsgLen;
                            CanIf_RxIndication(&ReceiverMailBox, &ReceiverPduInfo);
                        }
                        else
                        {
                            //MISRA
                        }
                    }
                }
                else
                {
                    //MISRA
                }
            }
            else
            {
                //MISRA
            }
        }
    }
    else
    {
        //MISRA
    }
}




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
    Can_ReturnType    Can_SetControllerMode (uint8 Controller,Can_StateTransitionType Transition)
    {
        uint32 cancontrollerbaseadress;
        uint32 HardwareObjectIndex=0 ;
        uint32 ObjectIndex=0;
        cancontrollerbaseadress=Can.CanConfigSet.CanController[Controller].CanControllerBaseAddress;
        if(CanDriverState == CAN_UNINIT)
        {
            CanDevelopmentError=CAN_E_UNINIT;
            return CAN_NOT_OK;

        }
                            /**[SWS_Can_00261] [ The function Can_SetControllerMode(CAN_T_START) shall
                    set the hardware registers in a way that makes the CAN controller participating on
                    the network.]**/
        else	if ( Transition ==CAN_T_START)
        {
            if(status_Initialization)
            {
                                    /**[SWS_Can_00262] [ The function Can_SetControllerMode(CAN_T_START) shall
                    wait for limited time until the CAN controller is fully operational. Compare to
                    SWS_Can_00398.]**/
                CANEnable(cancontrollerbaseadress);
                while( CanTimeoutDuration && status_Initialization)
                {
                    CanTimeoutDuration--;

                }
                if(CanTimeoutDuration==0)
                {
                    CanDevelopmentError=CAN_E_TRANSITION;
                    return CAN_NOT_OK;
                }
                else if (CanTimeoutDuration != 0)
                {
                    Can_EnableControllerInterrupts(Controller);
                    ControllerState[Controller]=true;
                    return CAN_OK;
                }
            }
                                /**[SWS_Can_00409]  When the function Can_SetControllerMode(CAN_T_START)
                    is entered and the CAN controller is not in state STOPPED it shall detect a invalid
                    state transition (Compare to SWS_Can_00200).**/
            else
            {
                CanDevelopmentError=CAN_E_TRANSITION;
                return CAN_NOT_OK;
            }
        }

                        /**	[SWS_Can_00263] [ The function Can_SetControllerMode(CAN_T_STOP) shall set
                the bits inside the CAN hardware such that the CAN controller stops participating on
                the network.]**/
        else if (Transition ==CAN_T_STOP)
        {
            if(!status_Initialization)
            {
                                /**[SWS_Can_00264] [ The function Can_SetControllerMode(CAN_T_STOP) shall
                wait for a limited time until the CAN controller is really switched off. Compare to
                SWS_Can_00398.]**/
                CANDisable(cancontrollerbaseadress);
                while( CanTimeoutDuration && (!status_Initialization))
                {
                    CanTimeoutDuration--;
                                        /**[SWS_Can_00282] [ The function Can_SetControllerMode(CAN_T_STOP) shall
                    cancel pending messages. ]*/
                }

                if(CanTimeoutDuration==0)
                {
                    CanDevelopmentError=CAN_E_TRANSITION;
                    return CAN_NOT_OK;
                }
                else if (CanTimeoutDuration != 0)
                {


                    for(HardwareObjectIndex =0; HardwareObjectIndex < NUMBER_OF_HOH ; HardwareObjectIndex++){
                        if(Can.CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanObjectType == TRANSMIT && Can.CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanControllerRef -> CanControllerId == Controller)
                        {
                            for(ObjectIndex = 0; HardwareObjectIndex < Can.CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanHwObjectCount ; ObjectIndex ++)
                            {
                                CLEAR_BIT(cancontrollerbaseadress,CANIF1CMSK,7);
                                CLEAR_BIT(cancontrollerbaseadress,CANIF1CMSK,2);
                                CLEAR_BIT(cancontrollerbaseadress,CANIF1MCTL,8);
                                GET_ADDRESS_VAL(cancontrollerbaseadress,CANIF1CRQ) = MessageObject[Controller][HardwareObjectIndex][ObjectIndex].MessageObjectNumber;
                            }
                        }
                    }
                    Can_DisableControllerInterrupts(Controller);
                    ControllerState[Controller]=false;
                    return CAN_OK;
                }

            }
                                /**[SWS_Can_00410] [ When the function Can_SetControllerMode(CAN_T_STOP) is
                    entered and the CAN controller is neither in state STARTED nor in state STOPPED,
                    it shall detect a invalid state transition (Compare to SWS_Can_00200).] */
            else
            {
                CanDevelopmentError=CAN_E_TRANSITION;
                return CAN_NOT_OK;
            }
        }

                        /**[SWS_Can_00265] [ The function Can_SetControllerMode(CAN_T_SLEEP) shall
                set the controller into sleep mode.]**/
        else if (Transition ==CAN_T_SLEEP)
        {
                            /**[SWS_Can_00290] [ If the CAN HW does not support a sleep mode, the function
                Can_SetControllerMode(CAN_T_SLEEP) shall set the CAN controller to the logical
                sleep mode.]**/
            if( (!status_Initialization) && (! LogicalSleep[Controller]) )
            {

                CANDisable(cancontrollerbaseadress);
                while( CanTimeoutDuration && (!status_Initialization) && (!LogicalSleep[Controller]) )
                {
                    CanTimeoutDuration--;
                }
                if(CanTimeoutDuration==0)
                {
                    CanDevelopmentError=CAN_E_TRANSITION;
                    return CAN_NOT_OK;
                }
                else if (CanTimeoutDuration != 0)
                {
                    Can_DisableControllerInterrupts(Controller);
                    LogicalSleep[Controller]=true;
                    return CAN_OK;
                }
                                    /**[SWS_Can_00411] [ When the function Can_SetControllerMode(CAN_T_SLEEP)
                    is entered and the CAN controller is neither in state STOPPED nor in state SLEEP, it
                    shall detect a invalid state transition]**/
                else
                {
                    CanDevelopmentError=CAN_E_TRANSITION;
                    return CAN_NOT_OK;
                }
            }

                                    /**[SWS_Can_00267] [If the CAN HW does not support a sleep mode, the function
                        Can_SetControllerMode(CAN_T_WAKEUP) shall return from the logical sleep mode,but have no effect to the CAN controller state (as the controller is already in stopped
                        state).]**/
            else if (Transition ==CAN_T_WAKEUP)
            {

                if(status_Initialization &&LogicalSleep[Controller])
                {
                                            /**[SWS_Can_00268] [ The function Can_SetControllerMode(CAN_T_WAKEUP) shall
                        wait for a limited time until the CAN controller is in STOPPED state. Compare to
                        SWS_Can_00398.]**/
                    CANEnable(cancontrollerbaseadress);
                    while( CanTimeoutDuration && status_Initialization && LogicalSleep[Controller])
                    {
                        CanTimeoutDuration--;

                    }
                    if(CanTimeoutDuration==0)
                    {
                        CanDevelopmentError=CAN_E_TRANSITION;
                        return CAN_NOT_OK;
                    }
                    else if (CanTimeoutDuration != 0)
                    {
                        Can_EnableControllerInterrupts(Controller);
                        LogicalSleep[Controller]=false;
                        return CAN_OK;
                    }

                }
                                        /**[SWS_Can_00412]
                        [When the function Can_SetControllerMode(CAN_T_WAKEUP) is entered and the CAN controller is
                        neither in state SLEEP nor in state STOPPED, it shall detect a invalid state transition
                        (Compare to SWS_Can_00200).]**/
                else
                {
                    CanDevelopmentError=CAN_E_TRANSITION;
                    return CAN_NOT_OK;
                }

            }
            else
            {
                CanDevelopmentError=CAN_E_PARAM_CONTROLLER;
                return CAN_NOT_OK;
            }

        }

   return CAN_NOT_OK;
    }





    void main(void)
    {

    }

