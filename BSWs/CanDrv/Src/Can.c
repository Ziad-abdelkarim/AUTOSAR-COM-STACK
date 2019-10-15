

uint8 CanDevolpmentErrorType;
static uint8 CanStateType= CAN_UNINIT;
static uint8 InterruptDisableCount[NUMBER_OF_CAN_CONTROLLER ];
static uint8 InterruptEnableCount[NUMBER_OF_CAN_CONTROLLER ];

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
    if (CanStateType!= CAN_READY) //check if can driver is not ready
    {
        CanDevolpmentErrorType = CAN_E_UNINIT; // raise the error CAN_E_UNINIT if the driver not yet initialized
    }
    else
    {
        if (Controller >= NUMBER_OF_CAN_CONTROLLER) //check parameter Controller is out of range
        {
            //CanDevolpmentErrorType = CAN_E_PARAM_CONTROLLER; //raise the error CAN_E_PARAM_CONTROLLER if the parameter Controller is out of range
        }
        else
        { //not found any error {the driver not yet initialized or parameter Controller is out of range}
            InterruptDisableCount[Controller]++; // increase a counter to make disable Controller Interrupts
            if (Controller == CAN_CONTROLLERS_ONE) //check which controllers (Controller0 or Controller1)
            { //Disable the CAN0 interrupt if it is enabled.
                if (IntIsEnabled(INT_CAN0)) //Returns TURE if a peripheral interrupt is enabled.
                {
                    /*When Can_EnableControllerInterrupts has been called several times,
                     *When Can_DisableControllerInterrupts must be called as many times before the interrupts are disabled. */
                    if (InterruptEnableCount[Controller] >= ONE) //check how many times you make to disable Controller Interrupts
                    {
                        InterruptEnableCount[Controller]--; //incremental enable count until reach to zero
                    }
                    else
                    {
                        //MISRA
                    }
                    if (InterruptEnableCount[Controller] == ZERO)
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
            else
            { //we use here controller1
              //Disable the CAN1 interrupt if it is enabled.
                if (IntIsEnabled(INT_CAN1)) //Returns TURE if a peripheral interrupt is enabled.
                {
                    /*When Can_EnableControllerInterrupts has been called several times,
                     *When Can_DisableControllerInterrupts must be called as many times before the interrupts are disabled. */
                    if (InterruptEnableCount[Controller] >= ONE) //check how many times you to make disable Controller Interrupts
                    {
                        InterruptEnableCount[Controller]--; //incremental enable count until reach to zero
                    }
                    else
                    {
                        //MISRA
                    }
                    if (InterruptEnableCount[Controller] == ZERO)
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
    if (CanStateType!= CAN_READY) //check if can driver is not ready
    {
        CanDevolpmentErrorType = CAN_E_UNINIT; // raise the error CAN_E_UNINIT if the driver not yet initialized
    }
    else
    {
        if (Controller >= NUMBER_OF_CAN_CONTROLLER) //check parameter Controller is out of range
        {
            //CanDevolpmentErrorType = CAN_E_PARAM_CONTROLLER; //raise the error CAN_E_PARAM_CONTROLLER if the parameter Controller is out of range
        }
        else
        { //not found any error {the driver not yet initialized or parameter Controller is out of range}
            InterruptEnableCount[Controller]++; // increase a counter to make enable
            if (Controller == CAN_CONTROLLERS_ONE) //check which controllers (Controller0 or Controller1)
            {
                //enable the CAN0 interrupt if it is disabled.
                if (!IntIsEnabled(INT_CAN0)) //Returns TURE if a peripheral interrupt is enabled.
                {
                    /*When Can_DisableControllerInterrupts has been called several times,
                     *When  Can_EnableControllerInterrupts must be called as many times before the interrupts are re-enabled. */
                    if (InterruptDisableCount[Controller] >= ONE) //check how many times you make to enable Controller Interrupts
                    {
                        InterruptDisableCount[Controller]--; //incremental disable count until reach to zero
                    }
                    else
                    {
                        //MISRA
                    }
                    if (InterruptDisableCount[Controller] == ZERO)
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
            else
            { //we use here controller1
              //enable the CAN1 interrupt if it is disabled.
                if (!IntIsEnabled(INT_CAN1)) //Returns TURE if a peripheral interrupt is enabled.
                {
                    /*When Can_DisableControllerInterrupts has been called several times,
                     *When  Can_EnableControllerInterrupts must be called as many times before the interrupts are re-enabled. */
                    if (InterruptDisableCount[Controller] >= ONE) //check how many times you make to enable Controller Interrupts
                    {
                        InterruptDisableCount[Controller]--; //incremental disable count  until reach to zero
                    }
                    else
                    {
                        //MISRA
                    }
                    if (InterruptDisableCount[Controller] == ZERO)
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
    uint32 ui32ErrorFlag = 0;

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
            Can.CanConfigSet.CanController[0U].CanControllerBaseAddress,
            CAN_INT_STS_CAUSE);
    /*
     If this was a status interrupt acknowledge it by reading the CAN
     controller status register.
     */
    if (ui32Status == CAN_INT_INTID_STATUS)
    {
        CANIntClear(Can.CanConfigSet.CanController[0U].CanControllerBaseAddress,
                    ui32Status); // Clear the message object interrupt
        /*
         Read the controller status.  This will return a field of status
         error bits that can indicate various errors. Refer to the
         API documentation for details about the error status bits.
         The act of reading this status will clear the interrupt.
         */
        ui32Status = CANStatusGet(
                Can.CanConfigSet.CanController[1U].CanControllerBaseAddress,
                CAN_STS_CONTROL);
        ui32ErrorFlag |= ui32Status; //Add ERROR flags to list of current errors To be handled
        if (ui32ErrorFlag & CAN_STATUS_BUS_OFF) //check if CAN controller has entered a Bus Off state.
        {
            Can_SetControllerMode(ControllerIndex, CAN_T_STOP); // [SWS_Can_00259] The function Can_Init shall set all CAN controllers in the state STOPPED. 
            /*     
             The CanIf module is notified with the function CanIf_ControllerBusOff after 
             STOPPED state is reached referring to the corresponding CAN controller with the abstract CanIf ControllerId.⌋(SRS_Can_01055)
             */
            //CanIf_ControllerBusOff(1U);           
        }
        else
        {
            //MISRA
        }
    }
    else if ((ui32Status >= 0x01U) && (ui32Status <= 0x20U)) //check if ObjectHandler in range 1-->32 .
    {
        CANIntClear(Can.CanConfigSet.CanController[0U].CanControllerBaseAddress,
                    ui32Status);  // Clear the message object interrupt
        for (ui8NumberOfObjectHandler = 0U;
                ui8NumberOfObjectHandler < NUM_OF_HOH;
                ui8NumberOfObjectHandler++) //determine OfObjectHandler which we need 
        {
            if (Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanControllerRef
                    == &CanContainer.CanConfigSet.CanController[1]) //check which CanController we use
            {
                if (Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanObjectType
                        == TRANSMIT)  //check if CanObjectType is transmit
                {
                    //determine all OfObjectHandler transmit  
                    for (ui8NumberOfObject = 0U;
                            ui8NumberOfObject
                                    < Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanHwObjectCount;
                            ui8NumberOfObject++)
                    {
                        if (MessageObject[0U][ui8NumberOfObjectHandler][ui8NumberOfObject].MessageObjectNumber
                                == ui32Status) //check which MessageObjectNumber we need to transmit form it
                        {
                            //confirms a previously successfully processed transmission
                            Message_Confirmation[ui8NumberOfObjectHandler][ui8NumberOfObject].Confirmation =
                                    true;
                            /*
                             (SRS_Can_01009)
                             Note: The service CanIf_TxConfirmation() is implemented in CanIf and called
                             by the CanDrv after the CAN L-PDU has been transmitted on the CAN network.
                             */
                            //CanIf_TxConfirmation(Message_Confirmation[ui8NumberOfObjectHandler][ui8NumberOfObject].PduId);
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
                    for (ui8NumberOfObject = 0U;
                            ui8NumberOfObject
                                    < Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanHwObjectCount;
                            ui8NumberOfObject++)
                    {
                        if (MessageObject[0U][ui8NumberOfObjectHandler][ui8NumberOfObjectHandler2]
                                == ui32Status) //check which MessageObjectNumber we need to receive in it 
                        {
                            CANMessageGet(
                                    Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanControllerRef->CanControllerBaseAddress,
                                    MessageObject[0U][ui8NumberOfObjectHandler][ui8NumberOfObject].MessageObjectNumber,
                                    &ReceivedMessage, 0U);
                            ReceiverMailBox.Hoh = ui8NumberOfObjectHandler;
                            ReceiverMailBox.ControllerId =
                                    Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanControllerRef->CanControllerId;
                            ReceiverMailBox.CanId = ReceivedMessage.ui32MsgId;
                            ReceiverPduInfo.SduDataPtr =
                                    ReceivedMessage.pui8MsgData;
                            ReceiverPduInfo.SduLength =
                                    ReceivedMessage.ui32MsgLen;
                            //CanIf_RxIndication(&ReceiverMailBox, &ReceiverPduInfo);                      
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
    uint32 ui32ErrorFlag = 0;

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
            Can.CanConfigSet.CanController[1U].CanControllerBaseAddress,
            CAN_INT_STS_CAUSE);
    /*
     If this was a status interrupt acknowledge it by reading the CAN
     controller status register.
     */
    if (ui32Status == CAN_INT_INTID_STATUS)
    {
        CANIntClear(Can.CanConfigSet.CanController[1U].CanControllerBaseAddress,
                    ui32Status); // Clear the message object interrupt
        /*
         Read the controller status.  This will return a field of status
         error bits that can indicate various errors. Refer to the
         API documentation for details about the error status bits.
         The act of reading this status will clear the interrupt.
         */
        ui32Status = CANStatusGet(
                Can.CanConfigSet.CanController[1U].CanControllerBaseAddress,
                CAN_STS_CONTROL);
        ui32ErrorFlag |= ui32Status; //Add ERROR flags to list of current errors To be handled
        if (ui32ErrorFlag & CAN_STATUS_BUS_OFF) //check if CAN controller has entered a Bus Off state.
        {
            Can_SetControllerMode(ControllerIndex, CAN_T_STOP); // [SWS_Can_00259] The function Can_Init shall set all CAN controllers in the state STOPPED. 
            /*     
             The CanIf module is notified with the function CanIf_ControllerBusOff after 
             STOPPED state is reached referring to the corresponding CAN controller with the abstract CanIf ControllerId.⌋(SRS_Can_01055)
             */
            //CanIf_ControllerBusOff(1U);           
        }
        else
        {
            //MISRA
        }
    }
    else if ((ui32Status >= 0x01U) && (ui32Status <= 0x20U)) //check if ObjectHandler in range 1-->32 .
    {
        CANIntClear(Can.CanConfigSet.CanController[1U].CanControllerBaseAddress,
                    ui32Status);  // Clear the message object interrupt
        for (ui8NumberOfObjectHandler = 0U;
                ui8NumberOfObjectHandler < NUM_OF_HOH;
                ui8NumberOfObjectHandler++) //determine OfObjectHandler which we need 
        {
            if (Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanControllerRef
                    == &CanContainer.CanConfigSet.CanController[1]) //check which CanController we use
            {
                if (Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanObjectType
                        == TRANSMIT)  //check if CanObjectType is transmit
                {
                    //determine all OfObjectHandler transmit  
                    for (ui8NumberOfObject = 0U;
                            ui8NumberOfObject
                                    < Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanHwObjectCount;
                            ui8NumberOfObject++)
                    {
                        if (MessageObject[1U][ui8NumberOfObjectHandler][ui8NumberOfObject].MessageObjectNumber
                                == ui32Status) //check which MessageObjectNumber we need to transmit form it
                        {
                            //confirms a previously successfully processed transmission
                            Message_Confirmation[ui8NumberOfObjectHandler][ui8NumberOfObject].Confirmation =
                                    true;
                            /*
                             (SRS_Can_01009)
                             Note: The service CanIf_TxConfirmation() is implemented in CanIf and called
                             by the CanDrv after the CAN L-PDU has been transmitted on the CAN network.
                             */
                            //CanIf_TxConfirmation(Message_Confirmation[ui8NumberOfObjectHandler][ui8NumberOfObject].PduId);
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
                    for (ui8NumberOfObject = 0U;
                            ui8NumberOfObject
                                    < Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanHwObjectCount;
                            ui8NumberOfObject++)
                    {
                        if (MessageObject[1U][ui8NumberOfObjectHandler][ui8NumberOfObjectHandler2]
                                == ui32Status) //check which MessageObjectNumber we need to receive in it 
                        {
                            CANMessageGet(
                                    Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanControllerRef->CanControllerBaseAddress,
                                    MessageObject[1U][ui8NumberOfObjectHandler][ui8NumberOfObject].MessageObjectNumber,
                                    &ReceivedMessage, 1U);
                            ReceiverMailBox.Hoh = ui8NumberOfObjectHandler;
                            ReceiverMailBox.ControllerId =
                                    Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanControllerRef->CanControllerId;
                            ReceiverMailBox.CanId = ReceivedMessage.ui32MsgId;
                            ReceiverPduInfo.SduDataPtr =
                                    ReceivedMessage.pui8MsgData;
                            ReceiverPduInfo.SduLength =
                                    ReceivedMessage.ui32MsgLen;
                            //CanIf_RxIndication(&ReceiverMailBox, &ReceiverPduInfo);                      
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
