/******************************************************************************************************************************
Service name:                                             Can_MainFunction_Write
Service ID[hex]:                                                   0x01
Description:                    This function performs the polling of TX indications when CAN_TX_PROCESSING is set to POLLING.
*******************************************************************************************************************************/
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

                                    /*[SWS_Can_00016] ⌈The Can module shall call CanIf_TxConfirmation to indicate a
                                   successful transmission.It shall either called by the TX-interrupt service routine
                                   of the corresponding HW resource or inside the Can_MainFunction_Write in case of
                                   polling mode.*/

static uint8 InterruptDisableCount[NUMBER_OF_CONTROLLERS];
static uint8 InterruptEnableCount[NUMBER_OF_CONTROLLERS];
static CanDriverStateType CanDriverState=CAN_UNINIT;
static MessageObjectType MessageObject[NUMBER_OF_CONTROLLERS][NUMBER_OF_HOH][MAX_NUMBER_OF_HANDLERS];
static ControllerStateType ControllerState[NUMBER_OF_CONTROLLERS] = {CAN_CS_UNINIT, CAN_CS_UNINIT};



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
Parameters (in):                                             Hth ---> information which HW-TRANSMIT handle shall be used for TRANSMIT.
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
                    1- The function Can_Write first checks if the hardware TRANSMIT object that is identified
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
                              3- The function Can_Write shall perform no actions if the hardware TRANSMIT object is busy with another TRANSMIT request for an L-PDU:
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
                          c.All necessary control operations to initiate the TRANSMIT are done
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
                                if the trigger TRANSMIT API is disabled for this hardware object
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
                                      10- Can_Write() shall accept a null pointer as SDU (Can_PduType.Can_SduPtrType = NULL) if the trigger TRANSMIT API is enabled
                                      for this hardware object (CanTriggerTransmitEnable = TRUE). ⌋ ()[SWS_CAN_00503]
                         */
                        /*
                                      11- If the trigger TRANSMIT API is enabled for the hardware object, Can_Write() shall interpret a null pointer as SDU
                                      (Can_PduType.Can_SduPtrType = NULL) as request for using the trigger TRANSMIT interface.
                                      If so and the hardware object is free, Can_Write() shall call CanIf_TriggerTransmit() to acquire the PDU’s data. ⌋ ()[SWS_CAN_00504]
                         */
                        if(MessageObject[CurrentHObj->CanControllerRef->CanControllerId][CurrentHObj->CanObjectId][Id].Confirmation==true)
                        {
                            //nadyi 3ala el api mn canif bta3t el TRANSMITapi

                            /*
                                        13- If default error detection for CanDrv is enabled:Can_Write() shall raise CAN_E_PARAM_POINTER and shall return CAN_NOT_OK
                                        if the trigger TRANSMIT API (CanIf_TriggerTransmit()) returns E_NOT_OK. ⌋ ()[SWS_CAN_00506]
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
                        == TRANSMIT)  //check if CanObjectType is TRANSMIT
                {
                    //determine all OfObjectHandler TRANSMIT
                    for (ui8NumberOfObject = INDEX_ZERO;
                            ui8NumberOfObject
                                    < Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanHwObjectCount;
                            ui8NumberOfObject++)
                    {
                        if (MessageObject[CONTROLLER_ZERO][ui8NumberOfObjectHandler][ui8NumberOfObject].MessageObjectNumber
                                == ui32Status) //check which MessageObjectNumber we need to TRANSMIT form it
                        {
                            //confirms a previously successfully processed transmission
                            MessageObject[CONTROLLER_ZERO][ui8NumberOfObjectHandler][ui8NumberOfObject].Confirmation =
                                    true;
                            /*
                             (SRS_Can_01009)
                             Note: The service CanIf_TxConfirmation() is implemented in CanIf and called
                             by the CanDrv after the CAN L-PDU has been TRANSMITted on the CAN network.
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
                        == TRANSMIT)  //check if CanObjectType is TRANSMIT
                {
                    //determine all OfObjectHandler TRANSMIT
                    for (ui8NumberOfObject = INDEX_ZERO;
                            ui8NumberOfObject
                                    < Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanHwObjectCount;
                            ui8NumberOfObject++)
                    {
                        if (MessageObject[CONTROLLER_ONE][ui8NumberOfObjectHandler][ui8NumberOfObject].MessageObjectNumber
                                == ui32Status) //check which MessageObjectNumber we need to TRANSMIT form it
                        {
                            //confirms a previously successfully processed transmission
                            MessageObject[CONTROLLER_ONE][ui8NumberOfObjectHandler][ui8NumberOfObject].Confirmation =
                                    true;
                            /*
                             (SRS_Can_01009)
                             Note: The service CanIf_TxConfirmation() is implemented in CanIf and called
                             by the CanDrv after the CAN L-PDU has been TRANSMITted on the CAN network.
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




/*
 * [SWS_Can_00108] The function Can_MainFunction_Read shall perform the polling of RX indications when CanRxProcessing
 *             is set to POLLING or mixed.In case of mixed processing only the hardware objects for which CanHardwareObjectUses
 *              Polling is set to TRUE shall be polled.(SRS_BSW_00432, SRS_SPAL_00157)
 */
 
 
/*********************************************************************************************************************************       
Service name:                                       Can_MainFunction_Read0
Service ID[hex]:                                                   0x08

Parameters (in):                                    				void
Parameters (inout):                                               None
Parameters (out):                                                  void
Return value:                                                		void
Description:   
                                  This function performs the polling of RX indications when CAN_RX_PROCESSING is set to POLLING.
*******************************************************************************************************************************/




#if ( Can0RxProcessing == POLLING )
void Can_MainFunction_Read0(void)
{

    /*************************************************************************************
     *                                    Local Variables                                *
     *************************************************************************************/

    uint8 HOH_Num_Iterator = 0;
    uint8 HW_Obj_Counter   = 0;
    uint8 ReceivedMsgArray0[16U];
    tCANMsgObject CanMsgReceived ;                          /* The structure used for encapsulating all the items associated with
                                                               a CAN message object in the CAN controller.*/

    Can_HwType ReceiverMailBox ;                            /*A data structure(MailBox) which clearly provides an Hardware Object Handle
                                                              including its corresponding CAN Controller and therefore CanDrv
                                                              as well as the specific CanId.*/
    PduInfoType ReceiverPduInfo;                            /*used to store the basic information about a PDU of any type,
                                                              namely a pointer variable pointing to its SDU (Payload),
                                                              and the corresponding length of the SDU in bytes.*/

    CanMsgReceived.pui8MsgData=ReceivedMsgArray0;           /*pointer of Data in a message is pointing to an Array where data will be stored*/
    uint32 cancontrollerbaseadress  =Can.CanConfigSet.CanController[0].CanControllerBaseAddress;

    /*
     *check if if the CAN controller 0 is used in the configuration.
     */
    if(Can.CanConfigSet.CanController[CONTROLLER_ZERO].CanControllerActivation)
    {

        /*
         * Reset 7th bit in CANIF1CMSK to Transfer the data in the CAN message object specified by
           the MNUM field in the CANIFnCRQ register into the CANIFn registers.
         */
        CLEAR_BIT(cancontrollerbaseadress,CAN_O_IF1CMSK,WRNRD_Bit);
        CLEAR_BIT(cancontrollerbaseadress,CAN_O_IF1CMSK,CANIF1CMSK_TXRQST_Bit);
        /*
         * loop for the number of hardware objects which are 32 in TIVA C
         */

        for(HOH_Num_Iterator= 0U ; HOH_Num_Iterator < NUMBER_OF_HOH ; HOH_Num_Iterator++)
        {
            /*
             * check if status of the HardwareObject is used as Receive object.
             */

            if(Can.CanConfigSet.CanHardwareObject[HOH_Num_Iterator].CanObjectType == RECEIVE)
            {
                /*
                 * check if the Reference of CAN Controller 0 to which the HOH is associated to is the same as
                 * Configurations of can controller 0.
                 */

                if(Can.CanConfigSet.CanHardwareObject[HOH_Num_Iterator].CanControllerRef == & Can.CanConfigSet.CanController[CONTROLLER_ZERO])
                {
                    /*
                     *iterate on number of Message objects  in each hardware object
                     */

                    for(HW_Obj_Counter = 0U ; HW_Obj_Counter < Can.CanConfigSet.CanHardwareObject[HOH_Num_Iterator].CanHwObjectCount;HW_Obj_Counter++)
                    {
                        /*
                         *Selects one of the 32 message objects in the message RAM for data transfer. The message objects are numbered from 1 to 32.
                         */
                        (CAN0_IF1CRQ_R)= MessageObject[CONTROLLER_ZERO][HOH_Num_Iterator][HW_Obj_Counter].MessageObjectNumber ;

                        /*
                         * check if NEWDAT bit may have two values :
                         * bit == 0 :No new data has been written into the data portion of this message object by the message handler,
                         *  since the last time this flag was cleared by the CPU.
                         * bit == 1 :The message handler or the CPU has written new data into the data portion of this message object.
                         */
                        if(GetBit((CAN0_IF1MCTL_R),NEWDAT_Bit) == STD_HIGH)
                        {
                            /*
                             * [SWS_Can_00279] On L-PDU reception, the Can module shall call the RX indication
                             * callback function CanIf_RxIndication with ID, HOH, abstract CanIf ControllerId in parameter Mailbox,
                             * and the DLC and pointer to the L-SDU buffer in parameter PduInfoPtr.(SRS_Can_01045)
                             */


                            /*******************************************************************************************************|
                                 |* Name:                                 CanMessageGet                                                *|
                                 |* Description:                                                                                       *|
                                 |* Reads a CAN message from one of the message object buffers.                                        *|
                                 |* Prototype:                                                                                         *|
                                 |*                   void CANMessageGet(uint32_t ui32Base,uint32_t ui32ObjID,                         *|
                                 |*                   tCANMsgObject *psMsgObject,bool bClrPendingInt)                                  *|
                                 |*                                                                                                    *|
                                 |* INPUT Parameters:                                                                                  *|
                                 |*                  ui32Base:           is the base address of the CAN controller.                    *|
                                 |*                  ui32ObjID:          is the object number to read (1-32).                          *|
                                 |*                  psMsgObject:        points to a structure containing message object fields.       *|
                                 |*                  bClrPendingInt:     indicates whether an associated interrupt should be cleared.  *|
                                 |*                                                                                                    *|
                                 |******************************************************************************************************/
                            CANMessageGet(Can.CanConfigSet.CanHardwareObject[HOH_Num_Iterator].CanControllerRef ->CanControllerBaseAddress,
                                          MessageObject[CONTROLLER_ZERO][HOH_Num_Iterator][HW_Obj_Counter].MessageObjectNumber,
                                          &CanMsgReceived, STD_OFF);

                            ReceiverMailBox.CanId= CanMsgReceived.ui32MsgID;
                            ReceiverMailBox.ControllerId=Can.CanConfigSet.CanHardwareObject[HOH_Num_Iterator].CanControllerRef -> CanControllerId;
                            ReceiverMailBox.Hoh=Can.CanConfigSet.CanHardwareObject[HOH_Num_Iterator].CanObjectId;
                            ReceiverPduInfo.SduDataPtr=CanMsgReceived.pui8MsgData;
                            ReceiverPduInfo.SduLength=CanMsgReceived.ui32MsgLen;

                            CanIf_RxIndication(&ReceiverMailBox, &ReceiverPduInfo);

                        }
                        else
                        {
                            /*MISRA*/
                        }
                    }
                }
                else
                {
                    /*MISRA*/
                }
            }
            else
            {
                /*MISRA*/
            }
        }
    }
    else
    {
        /*MISRA*/
    }
}

#endif






/*
 * Description:[SWS_Can_00108] The function Can_MainFunction_Read shall perform the polling of RX indications when CanRxProcessing
                is set to POLLING or mixed.In case of mixed processing only the hardware objects for which CanHardwareObjectUses
                Polling is set to TRUE shall be polled.(SRS_BSW_00432, SRS_SPAL_00157)
 * 
 */

/*********************************************************************************************************************************       
Service name:                                       Can_MainFunction_Read1
Service ID[hex]:                                                   0x08

Parameters (in):                                    				void
Parameters (inout):                                               None
Parameters (out):                                                  void
Return value:                                                		void
Description:   
                                  This function performs the polling of RX indications when CAN_RX_PROCESSING is set to POLLING.
*******************************************************************************************************************************/



#if ( Can1RxProcessing == POLLING )

void Can_MainFunction_Read1(void)
{
    /*************************************************************************************
     **                                    Local Variables                                       *
     *************************************************************************************/

    uint8 HOH_Num_Iterator = 0;
    uint8 HW_Obj_Counter   = 0;
    uint8 ReceivedMsgArray0[16U];
    tCANMsgObject CanMsgReceived ;                          /* The structure used for encapsulating all the items associated with
                                                               a CAN message object in the CAN controller.*/

    Can_HwType ReceiverMailBox ;                            /*A data structure(MailBox) which clearly provides an Hardware Object Handle
                                                              including its corresponding CAN Controller and therefore CanDrv
                                                              as well as the specific CanId.*/
    PduInfoType ReceiverPduInfo;                            /*used to store the basic information about a PDU of any type,
                                                              namely a pointer variable pointing to its SDU (Payload),
                                                              and the corresponding length of the SDU in bytes.*/

    CanMsgReceived.pui8MsgData=ReceivedMsgArray0;           /*pointer of Data in a message is pointing to an Array where data will be stored*/
    uint32 cancontrollerbaseadress  =Can.CanConfigSet.CanController[1].CanControllerBaseAddress;



    /*
     *check if if the CAN controller 1 is used in the configuration.
     */
    if(Can.CanConfigSet.CanController[CONTROLLER_ONE].CanControllerActivation)
    {

        /*
         * Reset 7th bit in CANIF1CMSK to Transfer the data in the CAN message object specified by
           the MNUM field in the CANIFnCRQ register into the CANIFn registers.
         */
        CLEAR_BIT(cancontrollerbaseadress,CAN_O_IF1CMSK,WRNRD_Bit);
        CLEAR_BIT(cancontrollerbaseadress,CAN_O_IF1CMSK,CANIF1CMSK_TXRQST_Bit);
        /*
         * loop for the number of hardware objects which are 32 in TIVA C
         */

        for(HOH_Num_Iterator= 0U ; HOH_Num_Iterator < NUMBER_OF_HOH ; HOH_Num_Iterator++)
        {
            /*
             * check if status of the HardwareObject is used as Receive object.
             */

            if(Can.CanConfigSet.CanHardwareObject[HOH_Num_Iterator].CanObjectType == RECEIVE)
            {
                /*
                 * check if the Reference of CAN Controller 0 to which the HOH is associated to is the same as
                 * Configurations of can controller 1.
                 */

                if(Can.CanConfigSet.CanHardwareObject[HOH_Num_Iterator].CanControllerRef == & Can.CanConfigSet.CanController[CONTROLLER_ONE])
                {
                    /*
                     *iterate on number of Message objects  in each hardware object
                     */

                    for(HW_Obj_Counter = 0U ; HW_Obj_Counter < Can.CanConfigSet.CanHardwareObject[HOH_Num_Iterator].CanHwObjectCount;HW_Obj_Counter++)
                    {
                        /*
                         *Selects one of the 32 message objects in the message RAM for data transfer. The message objects are numbered from 1 to 32.
                         */
                        (CAN0_IF1CRQ_R)= MessageObject[CONTROLLER_ONE][HOH_Num_Iterator][HW_Obj_Counter].MessageObjectNumber ;

                        /*
                         * check if NEWDAT bit may have two values :
                         * bit == 0 :No new data has been written into the data portion of this message object by the message handler,
                         *           since the last time this flag was cleared by the CPU.
                         * bit == 1 :The message handler or the CPU has written new data into the data portion of this message object.
                         */
                        if(GetBit((CAN0_IF1MCTL_R),NEWDAT_Bit) == STD_HIGH)
                        {
                            /*
                             * [SWS_Can_00279] On L-PDU reception, the Can module shall call the RX indication
                             * callback function CanIf_RxIndication with ID, HOH, abstract CanIf ControllerId in parameter Mailbox,
                             * and the DLC and pointer to the L-SDU buffer in parameter PduInfoPtr.(SRS_Can_01045)
                             */


                            /*******************************************************************************************************|
                                 |* Name:                                 CanMessageGet                                                *|
                                 |* Description:                                                                                       *|
                                 |* Reads a CAN message from one of the message object buffers.                                        *|
                                 |* Prototype:                                                                                         *|
                                 |*                   void CANMessageGet(uint32_t ui32Base,uint32_t ui32ObjID,                         *|
                                 |*                   tCANMsgObject *psMsgObject,bool bClrPendingInt)                                  *|
                                 |*                                                                                                    *|
                                 |* INPUT Parameters:                                                                                  *|
                                 |*                  ui32Base:           is the base address of the CAN controller.                    *|
                                 |*                  ui32ObjID:          is the object number to read (1-32).                          *|
                                 |*                  psMsgObject:        points to a structure containing message object fields.       *|
                                 |*                  bClrPendingInt:     indicates whether an associated interrupt should be cleared.  *|
                                 |*                                                                                                    *|
                                 |******************************************************************************************************/
                            CANMessageGet(Can.CanConfigSet.CanHardwareObject[HOH_Num_Iterator].CanControllerRef ->CanControllerBaseAddress,
                                          MessageObject[CONTROLLER_ONE][HOH_Num_Iterator][HW_Obj_Counter].MessageObjectNumber,
                                          &CanMsgReceived, STD_OFF);

                            ReceiverMailBox.CanId= CanMsgReceived.ui32MsgID;
                            ReceiverMailBox.ControllerId=Can.CanConfigSet.CanHardwareObject[HOH_Num_Iterator].CanControllerRef -> CanControllerId;
                            ReceiverMailBox.Hoh=Can.CanConfigSet.CanHardwareObject[HOH_Num_Iterator].CanObjectId;
                            ReceiverPduInfo.SduDataPtr=CanMsgReceived.pui8MsgData;
                            ReceiverPduInfo.SduLength=CanMsgReceived.ui32MsgLen;

                            CanIf_RxIndication(&ReceiverMailBox, &ReceiverPduInfo);

                        }
                        else
                        {
                            /*MISRA*/
                        }
                    }
                }
                else
                {
                    /*MISRA*/
                }
            }
            else
            {
                /*MISRA*/
            }
        }
    }
    else
    {
        /*MISRA*/
    }


}

#endif












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
            if(ControllerState[Controller]==CAN_CS_STOPPED)
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
                    ControllerState[Controller]=CAN_CS_STARTED;
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
            if(ControllerState[Controller]==CAN_CS_STARTED)
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
                    ControllerState[Controller]=CAN_CS_STOPPED;
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
            if( (ControllerState[Controller]==CAN_CS_STARTED) )
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
                    ControllerState[Controller]=CAN_CS_SLEEP;
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

                if(ControllerState[Controller]==CAN_CS_SLEEP)
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
                        ControllerState[Controller]=CAN_CS_STARTED;
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



/*********************************************************************************************************************************       
Service name:                                       Can_Init
Service ID[hex]:                                    0x00
Sync/Async:                                         Synchronous
Reentrancy:                                         Non Reentrant
Parameters (in):                                    Config ---> Pointer to driver configuration.                                            
Parameters (inout):                                 None
Parameters (out):                                   None
Return value:                                       None
Description:										This function initializes the module.
*********************************************************************************************************************************/
void Can_Init(const Can_ConfigType* Config)
{
	uint32_t CanSysCtlPeripheral[2] = {
		SYSCTL_PERIPH_CAN0,
		SYSCTL_PERIPH_CAN1
	};
	uint8_t ControllerIndex, HardwareObjectIndex, ObjectIndex;
	uint8_t MessageObjectNumber[NUMBER_OF_CONTROLLERS] = {1,1};
	bool UninitCheck = true;
	tCANBitClkParms CANBitClkParms;
	tCANMsgObject CANMsgObject;
	CanObjectType HardwareObjectType;
	
	/* [SWS_Can_00174] If default error detection for the Can module is enabled: The
		function Can_Init shall raise the error CAN_E_TRANSITION if the driver is not in
		state CAN_UNINIT. */
	if(CanDriverState != CAN_UNINIT)
	{
		CanDevelopmentError = CAN_E_TRANSITION;
	}
	else
	{
		/* [SWS_Can_00408] If default error detection for the Can module is enabled: The
			function Can_Init shall raise the error CAN_E_TRANSITION if the CAN controllers
			are not in state UNINIT. */
		for(ControllerIndex=0U; ControllerIndex < NUMBER_OF_CONTROLLERS; ControllerIndex++)
		{
			if(ControllerState[ControllerIndex] != CAN_CS_UNINIT)
			{
				UninitCheck = false;
			}
			else
			{
				//MISRA
			}
		}


		if(UninitCheck == true)
		{
			/* [SWS_Can_00245] The function Can_Init shall initialize all CAN controllers
				according to their configuration. (SRS_SPAL_12057, SRS_Can_01041) */
				
				
			// Loop over all controllers
			for(ControllerIndex=0U; ControllerIndex < NUMBER_OF_CONTROLLERS; ControllerIndex++)
			{
				// Check if controller needs to be activated
				if(Config->CanConfigSet.CanController[ControllerIndex].CanControllerActivation)
				{	
					// Enable Can peripheral clock
					SysCtlPeripheralEnable(CanSysCtlPeripheral[ControllerIndex]);
					// Wait for clock to be enabled
					while(!SysCtlPeripheralReady(CanSysCtlPeripheral[ControllerIndex]));
					
					// Initialize Can controller
					CANInit(Config->CanConfigSet.CanController[ControllerIndex].CanControllerBaseAddress);
					
					// Set bit timing values
					CANBitClkParms.ui32SyncPropPhase1Seg = Config->CanConfigSet.CanController[ControllerIndex].CanControllerDefaultBaudrate->CanControllerPropSeg +\
																Config->CanConfigSet.CanController[ControllerIndex].CanControllerDefaultBaudrate->CanControllerSeg1 + (uint32_t)1;
					CANBitClkParms.ui32Phase2Seg = Config->CanConfigSet.CanController[ControllerIndex].CanControllerDefaultBaudrate->CanControllerSeg2;
					CANBitClkParms.ui32SJW = Config->CanConfigSet.CanController[ControllerIndex].CanControllerDefaultBaudrate->CanControllerSyncJumpWidth;
					CANBitClkParms.ui32QuantumPrescaler =  SysCtlClockGet() / (CANBitClkParms.ui32SyncPropPhase1Seg + CANBitClkParms.ui32Phase2Seg) *\
																	(Config->CanConfigSet.CanController[ControllerIndex].CanControllerDefaultBaudrate->CanControllerBaudRate * 1000U);
					// Set bit timing
					CANBitTimingSet(Config->CanConfigSet.CanController[ControllerIndex].CanControllerBaseAddress, &CANBitClkParms);
					
					/* [SWS_Can_00259] The function Can_Init shall set all CAN controllers in the state STOPPED. */
					ControllerState[ControllerIndex] = CAN_CS_STOPPED;
				}
				else
				{
					//MISRA
				}
				
			}
			
			// Loop over all HOH
			for(HardwareObjectIndex = 0; HardwareObjectIndex < NUMBER_OF_HOH ; HardwareObjectIndex++)
			{
				// Get HOH's Controller ID
				ControllerIndex = Config->CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanControllerRef->CanControllerId;
				// Get HOH's Object Type
				HardwareObjectType = Config->CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanObjectType;
				
				// Loop over all HTH or HRH
				for(ObjectIndex = 0; ObjectIndex < Config->CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanHwObjectCount; ObjectIndex++)
				{
					// Assign a message object to the HTH or HRH
					MessageObject[ControllerIndex][HardwareObjectIndex][ObjectIndex].MessageObjectNumber = MessageObjectNumber[ControllerIndex];
					// Set Message Objects as Free
					MessageObject[ControllerIndex][HardwareObjectIndex][ObjectIndex].Confirmation = true;
					// If the HOH type was receive then additional operations must be done
					if(HardwareObjectType == RECEIVE)
					{
						// Set HRH values
						CANMsgObject.ui32MsgID = Config->CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanHwFilter.CanHwFilterCode;
						CANMsgObject.ui32MsgIDMask = Config->CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanHwFilter.CanHwFilterMask;
						CANMsgObject.ui32MsgLen = 8U;
						
						if(Config->CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanIdType == STANDARD)
						{
							CANMsgObject.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER | MSG_OBJ_FIFO;
						}
						else if(Config->CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanIdType == EXTENDED)
						{
							CANMsgObject.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER | MSG_OBJ_EXTENDED_ID | MSG_OBJ_USE_EXT_FILTER | MSG_OBJ_FIFO;
						}
						else
						{
							//MISRA
						}
						
						// Make controller receive on this message object
						CANMessageSet(Config->CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanControllerRef->CanControllerBaseAddress,
									MessageObjectNumber[ControllerIndex],
									&CANMsgObject, MSG_OBJ_TYPE_RX);	
					}
					else
					{
						//MISRA
					}
					
					// Increment message object number for next HTH or HRH
					MessageObjectNumber[ControllerIndex] ++;
				}
			}
			
			/* [SWS_Can_00246] The function Can_Init shall change the module state to
				CAN_READY, after initializing all controllers inside the HW
				Unit.(SRS_SPAL_12057, SRS_Can_01041) */
			CanDriverState = CAN_READY;
		}
		else
		{
			CanDevelopmentError = CAN_E_TRANSITION;
		}
	}
}



/******************************************************************************************************************************************************

Service name:                                             Can_MainFunction_Write
Service ID[hex]:                                                   0x01
Description:                    This function performs the polling of TX indications when CAN_TX_PROCESSING is set to POLLING.

**********************************************************************************************************************************************************/
#if(Can0TxProcessing==INTERRUPT && Can1TxProcessing==INTERRUPT)
               void Can_MainFunction_Write(){
                    //empty define function in interrupt mode
                }
#elif(Can0TxProcessing==POLLING || Can1TxProcessing==POLLING)

    void Can_MainFunction_Write(){


        uint8 INDEX_HOH;
        uint8 INDEX_MESGOBJ;

        #if(Can0TxProcessing==POLLING)

           if(Can.CanConfigSet.CanController[0].CanControllerActivation){
                for(INDEX_HOH=0;INDEX_HOH<NUMBER_OF_HOH;INDEX_HOH++){
                    if(Can.CanConfigSet.CanHardwareObject[INDEX_HOH].CanControllerRef== &Can.CanConfigSet.CanController[0]){
                        if(Can.CanConfigSet.CanHardwareObject[INDEX_HOH].CanObjectType==TRANSMIT){
                          for(INDEX_MESGOBJ=0;INDEX_MESGOBJ<=Can.CanConfigSet.CanHardwareObject[INDEX_HOH].CanHwObjectCount;INDEX_MESGOBJ++){
                             if(MessageObject[0][INDEX_HOH][INDEX_MESGOBJ].MessageObjectNumber <= 16){
                                if(!(GetBit(CAN0_BASE+CAN_O_TXRQ1,(MessageObject[0][INDEX_HOH][INDEX_MESGOBJ].MessageObjectNumber-1)))){

                                    /*[SWS_Can_00016] ⌈The Can module shall call CanIf_TxConfirmation to indicate a
                                   successful transmission.It shall either called by the TX-interrupt service routine
                                   of the corresponding HW resource or inside the Can_MainFunction_Write in case of
                                   polling mode.*/

                                    MessageObject[0][INDEX_HOH][INDEX_MESGOBJ].Confirmation=true;
                                    //CanIf_TxConfirmation()

                                }else{

                                    /*7.8 Notification concept*/

                                    /*CanDrv must store the all in HTHs pending L-PDU Ids in an array
                                    organized perTH to avoid new search of the L-PDU ID for call of

                                     */

                                    //CanIf_TxConfirmation(MessageObject[0][INDEX_HOH][INDEX_MESGOBJ].PduId)
                                    MessageObject[0][INDEX_HOH][INDEX_MESGOBJ].Confirmation=true;
                                }


                            }else{

                                if(!(GetBit(CAN0_BASE+CAN_O_TXRQ2,(MessageObject[0][INDEX_HOH][INDEX_MESGOBJ].MessageObjectNumber)-1))){

                                    /*[SWS_Can_00016] ⌈The Can module shall call CanIf_TxConfirmation to indicate a
                                   successful transmission.It shall either called by the TX-interrupt service routine
                                   of the corresponding HW resource or inside the Can_MainFunction_Write in case of
                                   polling mode.*/

                                    MessageObject[0][INDEX_HOH][INDEX_MESGOBJ].Confirmation=true;
                                    //CanIf_TxConfirmation().

                                }else{

                                    /*7.8 Notification concept*/

                                    /*CanDrv must store the all in HTHs pending L-PDU Ids in an array
                                    organized perTH to avoid new search of the L-PDU ID for call of

                                     */

                                    //CanIf_TxConfirmation(MessageObject[0][INDEX_HOH][INDEX_MESGOBJ].PduId)
                                    MessageObject[0][INDEX_HOH][INDEX_MESGOBJ].Confirmation=true;
                                }

                            }

                          }

                        }else{/*misra error*/}
                    }else{/*misra error*/}
                  }
               }else{/*misra error*/}

               #else //misra error
               #endif


        #if(Can1TxProcessing==POLLING)

           if(Can.CanConfigSet.CanController[1].CanControllerActivation){

                for(INDEX_HOH=0;INDEX_HOH<NUMBER_OF_HOH;INDEX_HOH++){
                    if(Can.CanConfigSet.CanHardwareObject[INDEX_HOH].CanControllerRef== &Can.CanConfigSet.CanController[1]){
                        if(Can.CanConfigSet.CanHardwareObject[INDEX_HOH].CanObjectType==TRANSMIT){
                          for(INDEX_MESGOBJ=0;INDEX_MESGOBJ<=Can.CanConfigSet.CanHardwareObject[INDEX_HOH].CanHwObjectCount;INDEX_MESGOBJ++){
                           //if(MessageObject[0][INDEX_HOH][INDEX_MESGOBJ].MessageObjectNumber){}
                             if(MessageObject[1][INDEX_HOH][INDEX_MESGOBJ].MessageObjectNumber <= 16){
                                if(!(GetBit(CAN1_BASE+CAN_O_TXRQ1,(MessageObject[1][INDEX_HOH][INDEX_MESGOBJ].MessageObjectNumber-1)))){

                                    /*[SWS_Can_00016] ⌈The Can module shall call CanIf_TxConfirmation to indicate a
                                   successful transmission.It shall either called by the TX-interrupt service routine
                                   of the corresponding HW resource or inside the Can_MainFunction_Write in case of
                                   polling mode.*/

                                    MessageObject[1][INDEX_HOH][INDEX_MESGOBJ].Confirmation=true;
                                    //CanIf_TxConfirmation().

                                }else{

                                    /*7.8 Notification concept*/

                                    /*CanDrv must store the all in HTHs pending L-PDU Ids in an array
                                    organized perTH to avoid new search of the L-PDU ID for call of

                                     */

                                    //CanIf_TxConfirmation(MessageObject[1][INDEX_HOH][INDEX_MESGOBJ].PduId)
                                    MessageObject[0][INDEX_HOH][INDEX_MESGOBJ].Confirmation=true;
                                }


                            }else{

                                if(!(GetBit(CAN1_BASE+CAN_O_TXRQ2,(MessageObject[1][INDEX_HOH][INDEX_MESGOBJ].MessageObjectNumber)-1))){

                                    /*[SWS_Can_00016] ⌈The Can module shall call CanIf_TxConfirmation to indicate a
                                   successful transmission.It shall either called by the TX-interrupt service routine
                                   of the corresponding HW resource or inside the Can_MainFunction_Write in case of
                                   polling mode.*/

                                    MessageObject[1][INDEX_HOH][INDEX_MESGOBJ].Confirmation=true;
                                    //CanIf_TxConfirmation().

                                }else{

                                    /*7.8 Notification concept*/

                                    /*CanDrv must store the all in HTHs pending L-PDU Ids in an array
                                    organized perTH to avoid new search of the L-PDU ID for call of

                                     */


                                    //CanIf_TxConfirmation(MessageObject[1][INDEX_HOH][INDEX_MESGOBJ].PduId)
                                    MessageObject[0][INDEX_HOH][INDEX_MESGOBJ].Confirmation=true;
                                }
                              }
                          }

                        }else{/*misra error*/}
                    }else{/*misra error*/}
                  }
               }else{/*misra error*/}

        #else //misra error
        #endif

        }

#else //misra error
#endif


    void main(void)
    {

    }

