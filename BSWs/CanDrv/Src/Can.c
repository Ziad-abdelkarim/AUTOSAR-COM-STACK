/********************************************************************************************************************************
 **
 FileName:                   Can.c
 AUTOSAR Version:            4.2.2
 **
*********************************************************************************************************************************
 **                                                          Includes                                                          **
*********************************************************************************************************************************/
#include "Can.h"
/********************************************************************************************************************************
 **                                                       Global Variables                                                     **
 *******************************************************************************************************************************/
static uint8 InterruptDisableCount[NUMBER_OF_CONTROLLERS] = { 0, 0 };
static uint8 InterruptEnableCount[NUMBER_OF_CONTROLLERS] = { 0, 0 };
static CanDriverStateType CanDriverState = CAN_UNINIT;
static MessageObjectType MessageObject[NUMBER_OF_CONTROLLERS][NUMBER_OF_HOH][MAX_NUMBER_OF_HANDLERS];
static CanControllerStateType CanControllerState[NUMBER_OF_CONTROLLERS] = {CAN_CS_UNINIT, CAN_CS_UNINIT};
static boolean LogicalSleep[NUMBER_OF_CONTROLLERS];
/********************************************************************************************************************************
 **                                                        Local Functions                                                      **
 *********************************************************************************************************************************/
const CanHardwareObject* Can_GetHardwareObjectOfHTH(Can_HwHandleType HTH);

const CanHardwareObject* Can_GetHardwareObjectOfHTH(Can_HwHandleType HTH)
{
    const CanHardwareObject *ADDRESS;
    uint8 HOI;
    for (HOI = 0; HOI < NUMBER_OF_HOH ; HOI++)
    {
        if (Can.CanConfigSet.CanHardwareObject[HOI].CanObjectId == HTH)
        {
            ADDRESS = &Can.CanConfigSet.CanHardwareObject[HOI];
            break;
        }
        else
        {

        }
    }
    return ADDRESS;
}

/***********************************************************************************************
 Service name:                            Can_ISRHandler
 Sync/Async:                                 Synchronous
 Parameters (in):                            Controller
 Parameters (inout):                            None
 Parameters (out):                              None
 Return value:                                  None
 Description:                      Can Interrupt service routine
 ***********************************************************************************************/
void Can_ISRHandler(uint8 Controller)
{
    uint32 ui32Status;
    uint8 ui8NumberOfObjectHandler;
    uint8 ui8NumberOfObject;
    uint32 ui32ErrorFlag = INDEX_ZERO;

    tCANMsgObject ReceivedMessage;
    uint8 ReceivedMsgData[8U];
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
    ui32Status = CANIntStatus(Can.CanConfigSet.CanController[Controller].CanControllerBaseAddress, CAN_INT_STS_CAUSE);
    /*
     If this was a status interrupt acknowledge it by reading the CAN
     controller status register.
     */

    if(ui32Status == CAN_INT_INTID_STATUS)
    {
         /*
         Read the controller status.  This will return a field of status
         error bits that can indicate various errors. Refer to the
         API documentation for details about the error status bits.
         The act of reading this status will clear the interrupt.
         */
        CANIntClear(Can.CanConfigSet.CanController[Controller].CanControllerBaseAddress, ui32Status); /* Clear the message object interrupt*/
        ui32Status = CANStatusGet(Can.CanConfigSet.CanController[Controller].CanControllerBaseAddress, CAN_STS_CONTROL);
        ui32ErrorFlag |= ui32Status; /*Add ERROR flags to list of current errors To be handled*/
        if (ui32ErrorFlag & CAN_STATUS_BUS_OFF) /*check if CAN controller has entered a Bus Off state.*/
        {
            if(Can.CanConfigSet.CanController[Controller].CanBusOffProcessing == INTERRUPT)
            {
                Can_SetControllerMode(Controller, CAN_T_STOP); /* [SWS_Can_00259] The function Can_Init shall set all CAN controllers in the state STOPPED.*/
                /*
                 The CanIf module is notified with the function CanIf_ControllerBusOff after
                 STOPPED state is reached referring to the corresponding CAN controller with the abstract CanIf ControllerId.⌋(SRS_Can_01055)
                 */
                //CanIf_ControllerBusOff(Controller);
            }
            else
            {

            }
        }
        else
        {
            /*MISRA*/
        }
    }
    else if((ui32Status >= MIN_OBJECT_HANDLER) && (ui32Status <= MAX_OBJECT_HANDLER)) /*check if Object Handler in range 1-->32 */
    {
        CANIntClear(Can.CanConfigSet.CanController[Controller].CanControllerBaseAddress, ui32Status); /* Clear the message object interrupt*/
        for (ui8NumberOfObjectHandler = INDEX_ZERO; ui8NumberOfObjectHandler < NUMBER_OF_HOH; ui8NumberOfObjectHandler++) /*determine Of Object Handler which we need*/
        {
            if (Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanControllerRef == &Can.CanConfigSet.CanController[Controller]) /*check which Can Controller we use*/
            {
                if (Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanObjectType == TRANSMIT) /*check if CanbjectType is TRANSMIT*/
                {
                    if(Can.CanConfigSet.CanController[Controller].CanTxProcessing == INTERRUPT)
                    {
                        /*determine all Of Object Handler TRANSMIT*/
                        for (ui8NumberOfObject = INDEX_ZERO; ui8NumberOfObject < Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanHwObjectCount; ui8NumberOfObject++)
                        {
                            if(MessageObject[Controller][ui8NumberOfObjectHandler][ui8NumberOfObject].MessageObjectNumber == ui32Status) /*check which Message Object Number we need to TRANSMIT form it*/
                            {
                                /*confirms a previously successfully processed transmission*/
                                MessageObject[Controller][ui8NumberOfObjectHandler][ui8NumberOfObject].Confirmation = true;
                                /*
                                 (SRS_Can_01009)
                                 Note: The service CanIf_TxConfirmation() is implemented in CanIf and called
                                 by the CanDrv after the CAN L-PDU has been TRANSMITted on the CAN network.
                                 */
                                //CanIf_TxConfirmation(MessageObject[Controller][ui8NumberOfObjectHandler][ui8NumberOfObject].PduId);
                                UARTprintf("CAN_ISRHandler Message Sent\n");

                            }
                            else
                            {
                                /*MISRA*/
                            }
                        }
                    }
                    else
                    {

                    }
                }
                else if (Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanObjectType == RECEIVE) /*check if Can ObjectType is receive*/
                {
                    if(Can.CanConfigSet.CanController[Controller].CanRxProcessing == INTERRUPT)
                    {
                        /*determine all OfObject Handler receive*/
                        for (ui8NumberOfObject = INDEX_ZERO; ui8NumberOfObject < Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanHwObjectCount; ui8NumberOfObject++)
                        {
                            if (MessageObject[Controller][ui8NumberOfObjectHandler][ui8NumberOfObject].MessageObjectNumber == ui32Status) /*check which Message Object Number we need to receive in it*/
                            {
                                ReceivedMessage.pui8MsgData = ReceivedMsgData;
                                CANMessageGet(
                                        Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanControllerRef->CanControllerBaseAddress,
                                        MessageObject[Controller][ui8NumberOfObjectHandler][ui8NumberOfObject].MessageObjectNumber,
                                        &ReceivedMessage, INDEX_ONE);

                                ReceiverMailBox.Hoh = Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanObjectId;
                                ReceiverMailBox.ControllerId = Can.CanConfigSet.CanHardwareObject[ui8NumberOfObjectHandler].CanControllerRef->CanControllerId;
                                ReceiverMailBox.CanId = ReceivedMessage.ui32MsgID;
                                ReceiverPduInfo.SduDataPtr = ReceivedMessage.pui8MsgData;
                                ReceiverPduInfo.SduLength = ReceivedMessage.ui32MsgLen;
                                //CanIf_RxIndication(&ReceiverMailBox, &ReceiverPduInfo);
                                UARTprintf("CAN_ISRHandler Message Received %d\n", *(ReceivedMessage.pui8MsgData));
                            }
                            else
                            {
                                /*MISRA*/
                            }
                        }
                    }
                    else
                    {

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
    if (Can.CanConfigSet.CanController[0U].CanRxProcessing == INTERRUPT || Can.CanConfigSet.CanController[0U].CanTxProcessing == INTERRUPT || Can.CanConfigSet.CanController[0U].CanBusOffProcessing == INTERRUPT)
    {
        Can_ISRHandler((uint8)0);
    }
    else
    {

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
    if (Can.CanConfigSet.CanController[1U].CanRxProcessing == INTERRUPT || Can.CanConfigSet.CanController[1U].CanTxProcessing == INTERRUPT || Can.CanConfigSet.CanController[1U].CanBusOffProcessing == INTERRUPT)
    {
        Can_ISRHandler((uint8)1);
    }
    else
    {

    }
}

/********************************************************************************************************************************/

/********************************************************************************************************************************
 **                                                      Functions  definition                                                                        **
 *********************************************************************************************************************************/

/*********************************************************************************************************************************
 Service name:                                       Can_Init
 Service ID[hex]:                                    0x00
 Sync/Async:                                         Synchronous
 Reentrancy:                                         Non Reentrant
 Parameters (in):                                    Config ---> Pointer to driver configuration.
 Parameters (inout):                                 None
 Parameters (out):                                   None
 Return value:                                       None
 Description:                                       This function initializes the module.
 *********************************************************************************************************************************/
void Can_Init(const Can_ConfigType* Config)
{
    uint32 CanSysCtlPeripheral[2] = { SYSCTL_PERIPH_CAN0, SYSCTL_PERIPH_CAN1 };
    uint8 ControllerIndex, HardwareObjectIndex, ObjectIndex;
    uint8 MessageObjectNumber[NUMBER_OF_CONTROLLERS ] = { 1, 1 };
    bool UninitCheck = true;
    tCANBitClkParms CANBitClkParms;
    tCANMsgObject CANMsgObject;
    CanObjectType HardwareObjectType;

    /* [SWS_Can_00174] If default error detection for the Can module is enabled: The
     function Can_Init shall raise the error CAN_E_TRANSITION if the driver is not in
     state CAN_UNINIT. */
    if (CanDriverState != CAN_UNINIT)
    {
        #if(CanDevErrorDetection == true)
        Det_ReportError(MODULE_ID, INSTANCE_ID, 0x00, CAN_E_TRANSITION);
        #endif
    }
    else
    {
        /* [SWS_Can_00408] If default error detection for the Can module is enabled: The
         function Can_Init shall raise the error CAN_E_TRANSITION if the CAN controllers
         are not in state UNINIT. */
        for (ControllerIndex = 0U; ControllerIndex < NUMBER_OF_CONTROLLERS ; ControllerIndex++)
        {
            if (CanControllerState[ControllerIndex] != CAN_CS_UNINIT)
            {
                UninitCheck = false;
            }
            else
            {
                /*MISRA*/
            }
        }

        if (UninitCheck == true)
        {
            /* [SWS_Can_00245] The function Can_Init shall initialize all CAN controllers
             according to their configuration. (SRS_SPAL_12057, SRS_Can_01041) */

            /* Loop over all controllers*/
            for (ControllerIndex = 0U; ControllerIndex < NUMBER_OF_CONTROLLERS ; ControllerIndex++)
            {
                /* Check if controller needs to be activated*/
                if (Config->CanConfigSet.CanController[ControllerIndex].CanControllerActivation)
                {
                    /* Enable Can peripheral clock*/
                    SysCtlPeripheralEnable(CanSysCtlPeripheral[ControllerIndex]);
                    /* Wait for clock to be enabled*/
                    while (!SysCtlPeripheralReady(CanSysCtlPeripheral[ControllerIndex]));

                    /* Initialize Can controller*/
                    CANInit(Config->CanConfigSet.CanController[ControllerIndex].CanControllerBaseAddress);

                    /* Set bit timing values*/
                    CANBitClkParms.ui32SyncPropPhase1Seg = (Config->CanConfigSet.CanController[ControllerIndex].CanControllerDefaultBaudrate->CanControllerPropSeg + Config->CanConfigSet.CanController[ControllerIndex].CanControllerDefaultBaudrate->CanControllerSeg1 + 1U);
                    CANBitClkParms.ui32Phase2Seg = Config->CanConfigSet.CanController[ControllerIndex].CanControllerDefaultBaudrate->CanControllerSeg2;
                    CANBitClkParms.ui32SJW = Config->CanConfigSet.CanController[ControllerIndex].CanControllerDefaultBaudrate->CanControllerSyncJumpWidth;
                    CANBitClkParms.ui32QuantumPrescaler = SysCtlClockGet() / ((CANBitClkParms.ui32SyncPropPhase1Seg + CANBitClkParms.ui32Phase2Seg) * Config->CanConfigSet.CanController[ControllerIndex].CanControllerDefaultBaudrate->CanControllerBaudRate * 1000U);
                    /* Set bit timing*/
                    CANBitTimingSet(Config->CanConfigSet.CanController[ControllerIndex].CanControllerBaseAddress, &CANBitClkParms);
                    /* [SWS_Can_00259] The function Can_Init shall set all CAN controllers in the state STOPPED. */
                    CanControllerState[ControllerIndex] = CAN_CS_STOPPED;
                }
                else
                {
                    /*MISRA*/
                }
            }

            /* Loop over all HOH*/
            for (HardwareObjectIndex = 0; HardwareObjectIndex < NUMBER_OF_HOH ; HardwareObjectIndex++)
            {
                /* Get HOH's Controller ID*/
                ControllerIndex = Config->CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanControllerRef->CanControllerId;
                /* Get HOH's Object Type*/
                HardwareObjectType = Config->CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanObjectType;

                /* Loop over all HTH or HRH*/
                for (ObjectIndex = 0; ObjectIndex < Config->CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanHwObjectCount; ObjectIndex++)
                {
                    /* Assign a message object to the HTH or HRH*/
                    MessageObject[ControllerIndex][HardwareObjectIndex][ObjectIndex].MessageObjectNumber = MessageObjectNumber[ControllerIndex];
                    /* Set Message Objects as Free*/
                    MessageObject[ControllerIndex][HardwareObjectIndex][ObjectIndex].Confirmation = true;
                    /* If the HOH type was receive then additional operations must be done*/
                    if (HardwareObjectType == RECEIVE)
                    {
                        /* Set HRH values*/
                        CANMsgObject.ui32MsgID =
                                Config->CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanHwFilter.CanHwFilterCode;
                        CANMsgObject.ui32MsgIDMask =
                                Config->CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanHwFilter.CanHwFilterMask;
                        CANMsgObject.ui32MsgLen = 8U;

                        if (Config->CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanIdType == STANDARD)
                        {
                            CANMsgObject.ui32Flags = MSG_OBJ_USE_ID_FILTER | MSG_OBJ_FIFO;
                        }
                        else if (Config->CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanIdType == EXTENDED)
                        {
                            CANMsgObject.ui32Flags = MSG_OBJ_USE_ID_FILTER | MSG_OBJ_EXTENDED_ID | MSG_OBJ_USE_EXT_FILTER| MSG_OBJ_FIFO;
                        }
                        else
                        {
                            /*MISRA*/
                        }

                        if (Can.CanConfigSet.CanController[ControllerIndex].CanRxProcessing == INTERRUPT)
                        {
                            CANMsgObject.ui32Flags |= MSG_OBJ_RX_INT_ENABLE;
                        }
                        else
                        {

                        }

                        /* Make controller receive on this message object*/
                        CANMessageSet(
                                Config->CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanControllerRef->CanControllerBaseAddress,
                                MessageObjectNumber[ControllerIndex], &CANMsgObject,
                                MSG_OBJ_TYPE_RX);
                    }
                    else
                    {
                        /*MISRA*/
                    }

                    /* Increment message object number for next HTH or HRH*/
                    MessageObjectNumber[ControllerIndex]++;
                }
            }

            /* [SWS_Can_00246] The function Can_Init shall change the module state to
             CAN_READY, after initializing all controllers inside the HW
             Unit.(SRS_SPAL_12057, SRS_Can_01041) */
            CanDriverState = CAN_READY;
        }
        else
        {
            #if(CanDevErrorDetection == true)
            Det_ReportError(MODULE_ID, INSTANCE_ID, 0x00, CAN_E_TRANSITION);
            #endif
        }
    }
}

/******************************************************************************************************************************************************

 Service name:                                             Can_MainFunction_Write
 Service ID[hex]:                                                   0x01
 Description:                    This function performs the polling of TX indications when CAN_TX_PROCESSING is set to POLLING.

 **********************************************************************************************************************************************************/
void Can_MainFunction_Write()
{
    if (CanDriverState != CAN_READY) /*check if can driver is not ready*/
    {
        /* raise the error CAN_E_UNINIT if the driver not yet initialized*/
        #if(CanDevErrorDetection == true)
            Det_ReportError(MODULE_ID, INSTANCE_ID, 0x01, CAN_E_UNINIT);
        #endif
    }
    else
    {
        uint8 ControllerIndex;
        uint8 HardwareObjectIndex;
        uint8 ObjectIndex;
        for(ControllerIndex = 0; ControllerIndex < NUMBER_OF_CONTROLLERS; ControllerIndex++)
        {
            if(Can.CanConfigSet.CanController[ControllerIndex].CanControllerActivation)
            {
                if(Can.CanConfigSet.CanController[ControllerIndex].CanTxProcessing == POLLING)
                {
                    for(HardwareObjectIndex = 0; HardwareObjectIndex < NUMBER_OF_HOH; HardwareObjectIndex++)
                    {
                        if(Can.CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanControllerRef== &Can.CanConfigSet.CanController[ControllerIndex])
                        {
                            if(Can.CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanObjectType == TRANSMIT)
                            {
                                for(ObjectIndex = 0; ObjectIndex < Can.CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanHwObjectCount; ObjectIndex++)
                                {
                                    if(MessageObject[ControllerIndex][HardwareObjectIndex][ObjectIndex].Confirmation == false)
                                    {
                                        if(MessageObject[ControllerIndex][HardwareObjectIndex][ObjectIndex].MessageObjectNumber <= 16)
                                        {
                                            if(!(GetBit(Can.CanConfigSet.CanController[ControllerIndex].CanControllerBaseAddress + CAN_O_TXRQ1, (MessageObject[ControllerIndex][HardwareObjectIndex][ObjectIndex].MessageObjectNumber-1))))
                                            {
                                                /*[SWS_Can_00016] ⌈The Can module shall call CanIf_TxConfirmation to indicate a
                                                 successful transmission.It shall either called by the TX-interrupt service routine
                                                 of the corresponding HW resource or inside the Can_MainFunction_Write in case of
                                                 polling mode.*/

                                                MessageObject[ControllerIndex][HardwareObjectIndex][ObjectIndex].Confirmation=true;
                                                //CanIf_TxConfirmation(MessageObject[ControllerIndex][HardwareObjectIndex][ObjectIndex].PduId);
                                                UARTprintf("Can_MainFunction_Write Transmission Confirmed\n");
                                            }
                                            else
                                            {

                                            }
                                        }
                                        else
                                        {
                                            if(!(GetBit(Can.CanConfigSet.CanController[ControllerIndex].CanControllerBaseAddress +CAN_O_TXRQ2,(MessageObject[ControllerIndex][HardwareObjectIndex][ObjectIndex].MessageObjectNumber)-1)))
                                            {
                                                /*[SWS_Can_00016] The Can module shall call CanIf_TxConfirmation to indicate a
                                                 successful transmission.It shall either called by the TX-interrupt service routine
                                                 of the corresponding HW resource or inside the Can_MainFunction_Write in case of
                                                 polling mode.*/

                                                MessageObject[ControllerIndex][HardwareObjectIndex][ObjectIndex].Confirmation=true;
                                                //CanIf_TxConfirmation(MessageObject[ControllerIndex][HardwareObjectIndex][ObjectIndex].PduId);
                                                UARTprintf("Can_MainFunction_Write Transmission Confirmed\n");
                                            }
                                            else
                                            {

                                            }
                                        }
                                    }
                                    else
                                    {

                                    }
                                }
                            }
                            else
                            {

                            }
                        }
                        else
                        {

                        }
                    }
                }
                else
                {

                }
            }
            else
            {

            }
        }
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
Can_ReturnType Can_SetControllerMode(uint8 Controller,
                                     Can_StateTransitionType Transition)
{
    uint32 CanControllerBaseAddress;
    uint32 HardwareObjectIndex = 0;
    uint32 ObjectIndex = 0;
    uint32 CanTimeoutDuration = CONTROLLER_MODE_TIMEOUT;
    CanControllerBaseAddress = Can.CanConfigSet.CanController[Controller].CanControllerBaseAddress;
    /*[SWS_Can_00198] If default error detection for the Can module is enabled: if the
     module is not yet initialized, the function Can_SetControllerMode shall raise default
     error CAN_E_UNINIT and return CAN_NOT_OK.*/
    if (CanDriverState == CAN_UNINIT)
    {
        #if(CanDevErrorDetection == true)
        Det_ReportError(MODULE_ID, INSTANCE_ID, 0x03, CAN_E_UNINIT);
        #endif
        return CAN_NOT_OK;
    }
    else
    {
        /*[SWS_Can_00199] If default error detection for the Can module is enabled: if the
         parameter Controller is out of range, the function Can_SetControllerMode shall
         raise default error CAN_E_PARAM_CONTROLLER and return CAN_NOT_OK.*/
        if (Controller >= NUMBER_OF_CONTROLLERS)
        {
            #if(CanDevErrorDetection == true)
            Det_ReportError(MODULE_ID, INSTANCE_ID, 0x03, CAN_E_PARAM_CONTROLLER);
            #endif
            return CAN_NOT_OK;
        }
        else
        {
            /*[SWS_Can_00261] [ The function Can_SetControllerMode(CAN_T_START) shall
             set the hardware registers in a way that makes the CAN controller participating on
             the network.]*/
            if (Transition == CAN_T_START)
            {
                if (CanControllerState[Controller] == CAN_CS_STOPPED)
                {
                    /*[SWS_Can_00262] [ The function Can_SetControllerMode(CAN_T_START) shall
                     wait for limited time until the CAN controller is fully operational. Compare to
                     SWS_Can_00398.]*/
                    CANEnable(CanControllerBaseAddress);
                    while (CanTimeoutDuration && status_Initialization)
                    {
                        CanTimeoutDuration--;

                    }
                    if (CanTimeoutDuration == 0)
                    {
                        #if(CanDevErrorDetection == true)
                        Det_ReportError(MODULE_ID, INSTANCE_ID, 0x03, CAN_E_TRANSITION);
                        #endif
                        return CAN_NOT_OK;
                    }
                    else if (CanTimeoutDuration != 0)
                    {
                        if(Can.CanConfigSet.CanController[Controller].CanRxProcessing == INTERRUPT || Can.CanConfigSet.CanController[Controller].CanTxProcessing == INTERRUPT || Can.CanConfigSet.CanController[Controller].CanBusOffProcessing == INTERRUPT)
                        {
                            Can_EnableControllerInterrupts(Controller);
                        }
                        else
                        {

                        }
                        CanControllerState[Controller] = CAN_CS_STARTED;
                        return CAN_OK;
                    }
                    else
                    {

                    }
                }
                /*[SWS_Can_00409]  When the function Can_SetControllerMode(CAN_T_START)
                 is entered and the CAN controller is not in state STOPPED it shall detect a invalid
                 state transition (Compare to SWS_Can_00200).*/
                else
                {
                    #if(CanDevErrorDetection == true)
                    Det_ReportError(MODULE_ID, INSTANCE_ID, 0x03, CAN_E_TRANSITION);
                    #endif
                    return CAN_NOT_OK;
                }
            }
            /* [SWS_Can_00263] [ The function Can_SetControllerMode(CAN_T_STOP) shall set
             the bits inside the CAN hardware such that the CAN controller stops participating on
             the network.]*/
            else if (Transition == CAN_T_STOP)
            {
                if(CanControllerState[Controller] == CAN_CS_STARTED)
                {
                    /*[SWS_Can_00264] [ The function Can_SetControllerMode(CAN_T_STOP) shall
                     wait for a limited time until the CAN controller is really switched off. Compare to
                     SWS_Can_00398.]*/
                    CANDisable(CanControllerBaseAddress);
                    while(CanTimeoutDuration && (!status_Initialization))
                    {
                        CanTimeoutDuration--;
                        /*[SWS_Can_00282] [ The function Can_SetControllerMode(CAN_T_STOP) shall
                         cancel pending messages. ]*/
                    }
                    if(CanTimeoutDuration == 0)
                    {
                        #if(CanDevErrorDetection == true)
                        Det_ReportError(MODULE_ID, INSTANCE_ID, 0x03, CAN_E_TRANSITION);
                        #endif
                        return CAN_NOT_OK;
                    }
                    else if(CanTimeoutDuration != 0)
                    {
                        for(HardwareObjectIndex = 0; HardwareObjectIndex < NUMBER_OF_HOH ; HardwareObjectIndex++)
                        {
                            if (Can.CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanObjectType == TRANSMIT && Can.CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanControllerRef->CanControllerId == Controller)
                            {
                                for (ObjectIndex = 0; ObjectIndex < Can.CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanHwObjectCount; ObjectIndex++)
                                {
                                    CLEAR_BIT(CanControllerBaseAddress, CANIF1CMSK, 7);
                                    CLEAR_BIT(CanControllerBaseAddress, CANIF1CMSK, 2);
                                    CLEAR_BIT(CanControllerBaseAddress, CANIF1MCTL, 8);
                                    GET_ADDRESS_VAL(CanControllerBaseAddress, CANIF1CRQ) = MessageObject[Controller][HardwareObjectIndex][ObjectIndex].MessageObjectNumber;
                                }
                            }
                            else
                            {

                            }
                        }
                        if(Can.CanConfigSet.CanController[Controller].CanRxProcessing == INTERRUPT || Can.CanConfigSet.CanController[Controller].CanTxProcessing == INTERRUPT || Can.CanConfigSet.CanController[Controller].CanBusOffProcessing == INTERRUPT)
                        {
                            Can_DisableControllerInterrupts(Controller);
                        }
                        else
                        {

                        }

                        CanControllerState[Controller] = CAN_CS_STOPPED;
                        return CAN_OK;
                    }
                    else
                    {

                    }

                }
                /*[SWS_Can_00410] [ When the function Can_SetControllerMode(CAN_T_STOP) is
                 entered and the CAN controller is neither in state STARTED nor in state STOPPED,
                 it shall detect a invalid state transition (Compare to SWS_Can_00200).] */
                else
                {
                    #if(CanDevErrorDetection == true)
                    Det_ReportError(MODULE_ID, INSTANCE_ID, 0x03, CAN_E_TRANSITION);
                    #endif
                    return CAN_NOT_OK;
                }
            }
            /*[SWS_Can_00265] [ The function Can_SetControllerMode(CAN_T_SLEEP) shall
             set the controller into sleep mode.]*/
            else if (Transition == CAN_T_SLEEP)
            {
                /*[SWS_Can_00290] [ If the CAN HW does not support a sleep mode, the function
                 Can_SetControllerMode(CAN_T_SLEEP) shall set the CAN controller to the logical
                 sleep mode.]*/
                if ((CanControllerState[Controller] == CAN_CS_STOPPED))
                {
                    CANDisable(CanControllerBaseAddress);
                    while (CanTimeoutDuration && (!status_Initialization)
                            && (!LogicalSleep[Controller]))
                    {
                        CanTimeoutDuration--;
                    }
                    if (CanTimeoutDuration == 0)
                    {
                        #if(CanDevErrorDetection == true)
                        Det_ReportError(MODULE_ID, INSTANCE_ID, 0x03, CAN_E_TRANSITION);
                        #endif
                        return CAN_NOT_OK;
                    }
                    else if (CanTimeoutDuration != 0)
                    {
                        if(Can.CanConfigSet.CanController[Controller].CanRxProcessing == INTERRUPT || Can.CanConfigSet.CanController[Controller].CanTxProcessing == INTERRUPT || Can.CanConfigSet.CanController[Controller].CanBusOffProcessing == INTERRUPT)
                        {
                            Can_DisableControllerInterrupts(Controller);
                        }
                        else
                        {

                        }
                        LogicalSleep[Controller] = true;
                        CanControllerState[Controller] = CAN_CS_SLEEP;
                        return CAN_OK;
                    }
                    /*[SWS_Can_00411] [ When the function Can_SetControllerMode(CAN_T_SLEEP)
                     is entered and the CAN controller is neither in state STOPPED nor in state SLEEP, it
                     shall detect a invalid state transition]*/
                    else
                    {
                        #if(CanDevErrorDetection == true)
                        Det_ReportError(MODULE_ID, INSTANCE_ID, 0x03, CAN_E_TRANSITION);
                        #endif
                        return CAN_NOT_OK;
                    }
                }
            }
            /*[SWS_Can_00267] [If the CAN HW does not support a sleep mode, the function
             Can_SetControllerMode(CAN_T_WAKEUP) shall return from the logical sleep mode,but have no effect to the CAN controller state (as the controller is already in stopped
             state).]*/
            else if (Transition == CAN_T_WAKEUP)
            {
                if (CanControllerState[Controller] == CAN_CS_SLEEP)
                {
                    /*[SWS_Can_00268] [ The function Can_SetControllerMode(CAN_T_WAKEUP) shall
                     wait for a limited time until the CAN controller is in STOPPED state. Compare to
                     SWS_Can_00398.]*/
                    CANEnable(CanControllerBaseAddress);
                    while (CanTimeoutDuration && status_Initialization
                            && LogicalSleep[Controller])
                    {
                        CanTimeoutDuration--;

                    }
                    if (CanTimeoutDuration == 0)
                    {
                        #if(CanDevErrorDetection == true)
                        Det_ReportError(MODULE_ID, INSTANCE_ID, 0x03,CAN_E_TRANSITION);
                        #endif
                        return CAN_NOT_OK;
                    }
                    else if (CanTimeoutDuration != 0)
                    {
                        if(Can.CanConfigSet.CanController[Controller].CanRxProcessing == INTERRUPT || Can.CanConfigSet.CanController[Controller].CanTxProcessing == INTERRUPT || Can.CanConfigSet.CanController[Controller].CanBusOffProcessing == INTERRUPT)
                        {
                            Can_EnableControllerInterrupts(Controller);
                        }
                        LogicalSleep[Controller] = false;
                        CanControllerState[Controller] = CAN_CS_STARTED;
                        return CAN_OK;
                    }
                    else
                    {

                    }
                }
                /*[SWS_Can_00412]
                 [When the function Can_SetControllerMode(CAN_T_WAKEUP) is entered and the CAN controller is
                 neither in state SLEEP nor in state STOPPED, it shall detect a invalid state transition
                 (Compare to SWS_Can_00200).]*/
                else
                {
                    #if(CanDevErrorDetection == true)
                    Det_ReportError(MODULE_ID, INSTANCE_ID, 0x03, CAN_E_TRANSITION);
                    #endif
                    return CAN_NOT_OK;
                }
            }
            else
            {

            }
        }
    }
    return CAN_NOT_OK;
}

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
    if(CanDriverState != CAN_READY) /*check if can driver is not ready*/
    { /* raise the error CAN_E_UNINIT if the driver not yet initialized*/
        #if(CanDevErrorDetection == true)
        Det_ReportError(MODULE_ID, INSTANCE_ID, 0x04, CAN_E_UNINIT);
        #endif
    }
    else
    {
        if (Controller >= NUMBER_OF_CONTROLLERS) /*check parameter Controller is out of range*/
        {
            /*raise the error CAN_E_PARAM_CONTROLLER if the parameter Controller is out of range*/
            #if(CanDevErrorDetection == true)
            Det_ReportError(MODULE_ID, INSTANCE_ID, 0x04, CAN_E_PARAM_CONTROLLER);
            #endif
        }
        else
        {
            uint32 INT_CAN[NUMBER_OF_CONTROLLERS] = {INT_CAN0, INT_CAN1};

            if(Can.CanConfigSet.CanController[Controller].CanRxProcessing == INTERRUPT || Can.CanConfigSet.CanController[Controller].CanTxProcessing == INTERRUPT || Can.CanConfigSet.CanController[Controller].CanBusOffProcessing == INTERRUPT)
            {
                /*No errors found {the driver not yet initialized or parameter Controller is out of range}*/
                InterruptDisableCount[Controller]++; /*increase a counter to make disable Controller Interrupts*/
                /*Disable the CAN interrupt if it is enabled.*/
                if (IntIsEnabled(INT_CAN[Controller])) /*Returns TURE if a peripheral interrupt is enabled.*/
                {
                    /*When Can_EnableControllerInterrupts has been called several times,
                     *When Can_DisableControllerInterrupts must be called as many times before the interrupts are disabled. */
                    if (InterruptEnableCount[Controller] >= INDEX_ONE) /*check how many times you make to disable Controller Interrupts**/
                    {
                        InterruptEnableCount[Controller]--; /*incremental enable count until reach to zero*/
                    }
                    else
                    {
                        /*MISRA*/
                    }
                    if (InterruptEnableCount[Controller] == INDEX_ZERO)
                    { /*Disables the specified CAN controller interrupt sources. Only enabled interrupt sources can cause a processor interrupt.*/
                        CANIntDisable(
                                Can.CanConfigSet.CanController[Controller].CanControllerBaseAddress,
                                CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
                        /*disable interrupts for CAN0*/
                        IntDisable(INT_CAN[Controller]);
                    }
                    else
                    {
                        /*must call this function until make InterruptEnableCount[Controller] =ZERO to make disable Controller Interrupts*/
                    }
                }
                else
                {

                }
            }
            else
            {

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
    if (CanDriverState != CAN_READY) /*check if can driver is not ready*/
    {
        /* raise the error CAN_E_UNINIT if the driver not yet initialized*/
        #if(CanDevErrorDetection == true)
        Det_ReportError(MODULE_ID, INSTANCE_ID, 0x05, CAN_E_UNINIT);
        #endif
    }
    else
    {
        if (Controller >= NUMBER_OF_CONTROLLERS) /*check parameter Controller is out of range*/
        {
            /*raise the error CAN_E_PARAM_CONTROLLER if the parameter Controller is out of range*/
            #if(CanDevErrorDetection == true)
            Det_ReportError(MODULE_ID, INSTANCE_ID, 0x05, CAN_E_PARAM_CONTROLLER);
            #endif
        }
        else
        {
            uint32 INT_CAN[NUMBER_OF_CONTROLLERS] = {INT_CAN0, INT_CAN1};
            /* No errors found {the driver not yet initialized or parameter Controller is out of range}*/
            InterruptEnableCount[Controller]++; /* increase a counter to make enable*/
            if(Can.CanConfigSet.CanController[Controller].CanRxProcessing == INTERRUPT || Can.CanConfigSet.CanController[Controller].CanTxProcessing == INTERRUPT || Can.CanConfigSet.CanController[Controller].CanBusOffProcessing == INTERRUPT)
            {
                if (Controller == CONTROLLER_ZERO) /*check which controllers (Controller0 or Controller1)*/
                {
                    /*enable the CAN interrupt if it is disabled.*/
                    if(!IntIsEnabled(INT_CAN[Controller])) /*Returns TURE if a peripheral interrupt is enabled*/
                    {
                        /*When Can_DisableControllerInterrupts has been called several times,
                         *When  Can_EnableControllerInterrupts must be called as many times before the interrupts are re-enabled. */
                        if (InterruptDisableCount[Controller] >= INDEX_ONE) /*check how many times you make to enable Controller Interrupts*/
                        {
                            InterruptDisableCount[Controller]--; /*incremental disable count until reach to zero*/
                        }
                        else
                        {
                            /*MISRA*/
                        }
                        if (InterruptDisableCount[Controller] == INDEX_ZERO)
                        { /*This function enables specific interrupt sources of the CAN controller. Only enabled sources cause a processor interrupt.*/
                            CANIntEnable(
                                    Can.CanConfigSet.CanController[Controller].CanControllerBaseAddress,
                                    CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
                            /* enable interrupts for CAN0*/
                            IntEnable(INT_CAN[Controller]);
                        }
                        else
                        {
                            /*must call this function until make InterruptDisableCount[Controller]=ZERO to make enable Controller Interrupts*/
                        }
                    }
                    else
                    {

                    }
                }
                else
                {
                    /*it is enabled*/
                }
            }
            else
            {

            }
        }
    }
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

Can_ReturnType Can_Write(Can_HwHandleType Hth, const Can_PduType* PduInfo)
{
    tCANMsgObject sCANMessage;
    uint8 Id = 0;
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
    if (CanDriverState == CAN_UNINIT)
    {
        #if(CanDevErrorDetection == true)
        Det_ReportError(MODULE_ID, INSTANCE_ID, 0x06, CAN_E_UNINIT);
        #endif

        return CAN_NOT_OK;
    }
    else
    {
        /*
         7-If default error detection for the Can module is enabled: The function Can_Write shall raise the error CAN_E_PARAM_HANDLE and shall
         return CAN_NOT_OK if the parameter Hth is not a configured Hardware Transmit Handle. ⌋ ()[SWS_Can_00217]
         */
        const CanHardwareObject* CurrentHObj = Can_GetHardwareObjectOfHTH(Hth);
        if (CurrentHObj->CanObjectType == RECEIVE)
        {
            #if(CanDevErrorDetection == true)
            Det_ReportError(MODULE_ID, INSTANCE_ID, 0x06, CAN_E_PARAM_HANDLE);
            #endif
            return CAN_NOT_OK;
        }
        else
        {
            /*
             1- The function Can_Write first checks if the hardware TRANSMIT object that is identified
             by the HTH is free and if another Can_Write is ongoing for the same HTH. [SWS_Can_00212]
             */
            for (ObjInd = 0; ObjInd < CurrentHObj->CanHwObjectCount; ObjInd++)
            {
                if (MessageObject[CurrentHObj->CanControllerRef->CanControllerId][CurrentHObj->CanObjectId][ObjInd].Confirmation == true)
                {
                    Id = ObjInd;
                    break;
                }
                else
                {
                    /*is not free */
                    /*
                     3- The function Can_Write shall perform no actions if the hardware TRANSMIT object is busy with another TRANSMIT request for an L-PDU:
                     [SWS_Can_00213]
                     a. The transmission of the other L-PDU shall not be cancelled and the function Can_Write is left without any actions.
                     b. The function Can_Write shall return CAN_BUSY.⌋ (SRS_Can_01049).
                     */
                    return CAN_BUSY;
                }

            }
            /* is free */
            /*
             2- if HTH is free :
             a. The mutex for that HTH is set to ‘signaled’
             b.The ID, DLC and SDU are put in a format appropriate for the hardware (if necessary) and copied in the appropriate
             hardware registers/buffers.
             c.All necessary control operations to initiate the TRANSMIT are done
             d.The mutex for that HTH is released
             e.The function returns with CAN_OK⌋ (SRS_Can_01049)
             */
            MessageObject[CurrentHObj->CanControllerRef->CanControllerId][CurrentHObj->CanObjectId][Id].Confirmation = false;
            /*
             9. If default error detection for CanDrv is enabled: Can_Write() shall raise CAN_E_PARAM_POINTER and shall return CAN_NOT_OK
             if the parameter PduInfo is a null pointer. ⌋ () [SWS_CAN_00219]
             */
            if (PduInfo == NULL)
            {
                #if(CanDevErrorDetection == true)
                Det_ReportError(MODULE_ID, INSTANCE_ID, 0x06, CAN_E_PARAM_POINTER);
                #endif
                MessageObject[CurrentHObj->CanControllerRef->CanControllerId][CurrentHObj->CanObjectId][Id].Confirmation = true;
                return CAN_NOT_OK;
            }
            else
            {
                if (PduInfo->sdu == NULL)
                {
                    /*
                     12- If default error detection for CanDrv is enabled: Can_Write() shall raise CAN_E_PARAM_POINTER and shall return CAN_NOT_OK
                     if the trigger TRANSMIT API is disabled for this hardware object
                     (CanTriggerTransmitEnable = FALSE) and the SDU pointer inside PduInfo is a null pointer. ⌋ ()[SWS_CAN_00505]
                     */
                    if (CurrentHObj->CanTriggerTransmitEnable == false)
                    {
                        MessageObject[CurrentHObj->CanControllerRef->CanControllerId][CurrentHObj->CanObjectId][Id].Confirmation = true;
                        #if(CanDevErrorDetection == true)
                        Det_ReportError(MODULE_ID, INSTANCE_ID, 0x06, CAN_E_PARAM_POINTER);
                        #endif

                        return CAN_NOT_OK;
                    }
                    else
                    {
                        /*
                         10- Can_Write() shall accept a null pointer as SDU (Can_PduType.Can_SduPtrType = NULL) if the trigger TRANSMIT API is enabled
                         for this hardware object (CanTriggerTransmitEnable = TRUE). ()[SWS_CAN_00503]
                         */
                        /*
                         11- If the trigger TRANSMIT API is enabled for the hardware object, Can_Write() shall interpret a null pointer as SDU
                         (Can_PduType.Can_SduPtrType = NULL) as request for using the trigger TRANSMIT interface.
                         If so and the hardware object is free, Can_Write() shall call CanIf_TriggerTransmit() to acquire the PDUs data. ()[SWS_CAN_00504]
                         */
                        if (MessageObject[CurrentHObj->CanControllerRef->CanControllerId][CurrentHObj->CanObjectId][Id].Confirmation == true)
                        {
                            /*
                             13- If default error detection for CanDrv is enabled:Can_Write() shall raise CAN_E_PARAM_POINTER and shall return CAN_NOT_OK
                             if the trigger TRANSMIT API (CanIf_TriggerTransmit()) returns E_NOT_OK. ⌋ ()[SWS_CAN_00506]
                             */
                        }
                        else
                        {

                        }
                    }

                }
                else
                {
                    /*
                     8- The function Can_Write shall return CAN_NOT_OK and if default error detection for the CAN module is enabled shall raise the error
                     CAN_E_PARAM_DLC: [SWS_Can_00218]

                     a. If the length is more than 64 byte.
                     b. If the length is more than 8 byte and the CAN controller is not in CAN FD mode (no CanControllerFdBaudrateConfig).
                     c. If the length is more than 8 byte and the CAN controller is in CAN FD mode (valid CanControllerFdBaudrateConfig),
                     but the CAN FD flag in Can_PduType->id is not set (refer Can_IdType).⌋ ( SRS_Can_01005)
                     */
                    /* full*/
                    if (PduInfo->length <= 8u)
                    {
                        /* send data*/
                        MessageObject[CurrentHObj->CanControllerRef->CanControllerId][CurrentHObj->CanObjectId][Id].PduId = PduInfo->swPduHandle;
                        sCANMessage.ui32MsgID = PduInfo->id;
                        sCANMessage.ui32MsgLen = PduInfo->length;
                        sCANMessage.pui8MsgData = PduInfo->sdu;
                        sCANMessage.ui32MsgIDMask = 0U;
                        if (CurrentHObj->CanIdType == EXTENDED)
                        {
                            sCANMessage.ui32Flags = MSG_OBJ_TX_INT_ENABLE | MSG_OBJ_EXTENDED_ID;
                        }
                        else
                        {
                            sCANMessage.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
                        }
                        CANMessageSet(
                                CurrentHObj->CanControllerRef->CanControllerBaseAddress,
                                MessageObject[CurrentHObj->CanControllerRef->CanControllerId][CurrentHObj->CanObjectId][Id].MessageObjectNumber,
                                &sCANMessage, MSG_OBJ_TYPE_TX);
                        UARTprintf("Can_Write Message Sent\n");
                        return CAN_OK;
                    }
                    else
                    {
                        #if(CanDevErrorDetection == true)
                        Det_ReportError(MODULE_ID, INSTANCE_ID, 0x06, CAN_E_PARAM_DLC);
                        #endif
                        MessageObject[CurrentHObj->CanControllerRef->CanControllerId][CurrentHObj->CanObjectId][Id].Confirmation = true;
                        return CAN_NOT_OK;
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
    return CAN_NOT_OK;
}

/*********************************************************************************************************************************
 Service name:                                       Can_MainFunction_Read
 Service ID[hex]:                                           0x08

 Parameters (in):                                           void
 Parameters (inout):                                        None
 Parameters (out):                                          void
 Return value:                                              void
 Description:          This function performs the polling of RX indications when CAN_RX_PROCESSING is set to POLLING.
 *******************************************************************************************************************************/
void Can_MainFunction_Read(void)
{
    uint8 ControllerIndex;
    for(ControllerIndex = 0U; ControllerIndex < NUMBER_OF_CONTROLLERS; ControllerIndex++)
    {
        /*
         *check if if the CAN controller is used in the configuration.
         */
        if(Can.CanConfigSet.CanController[ControllerIndex].CanControllerActivation)
        {
            if (Can.CanConfigSet.CanController[ControllerIndex].CanRxProcessing == POLLING)
            {
                /*
                * [SWS_Can_00108] The function Can_MainFunction_Read shall perform the polling of RX indications when CanRxProcessing
                *             is set to POLLING or mixed.In case of mixed processing only the hardware objects for which CanHardwareObjectUses
                *              Polling is set to TRUE shall be polled.(SRS_BSW_00432, SRS_SPAL_00157)
                */
                uint8 HardwareObjectIndex = 0;
                uint8 ObjectIndex = 0;
                uint8 ReceivedMsgArray[8U];
                tCANMsgObject CanMsgReceived; /* The structure used for encapsulating all the items associated with
                a CAN message object in the CAN controller.*/

                Can_HwType ReceiverMailBox; /*A data structure(MailBox) which clearly provides an Hardware Object Handle
                including its corresponding CAN Controller and therefore CanDrv
                as well as the specific CanId.*/
                PduInfoType ReceiverPduInfo; /*used to store the basic information about a PDU of any type,
                namely a pointer variable pointing to its SDU (Payload),
                and the corresponding length of the SDU in bytes.*/

                CanMsgReceived.pui8MsgData = ReceivedMsgArray; /*pointer of Data in a message is pointing to an Array where data will be stored*/


                for (HardwareObjectIndex = 0U; HardwareObjectIndex < NUMBER_OF_HOH; HardwareObjectIndex++)
                {
                    /*
                     * check if status of the HardwareObject is used as Receive object.
                     */
                    if (Can.CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanObjectType == RECEIVE)
                    {
                        /*
                         * check if the Reference of CAN Controller 0 to which the HOH is associated to is the same as
                         * Configurations of can controller 0.
                         */

                        if (Can.CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanControllerRef == &Can.CanConfigSet.CanController[ControllerIndex])
                        {
                            /*
                             *iterate on number of Message objects  in each hardware object
                             */

                            for (ObjectIndex = 0U; ObjectIndex < Can.CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanHwObjectCount; ObjectIndex++)
                            {
                                CANMessageGet(
                                        Can.CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanControllerRef->CanControllerBaseAddress,
                                        MessageObject[ControllerIndex][HardwareObjectIndex][ObjectIndex].MessageObjectNumber,
                                        &CanMsgReceived, STD_OFF);
                                if ((CanMsgReceived.ui32Flags) & MSG_OBJ_NEW_DATA)
                                {
                                    ReceiverMailBox.CanId = CanMsgReceived.ui32MsgID;
                                    ReceiverMailBox.ControllerId =
                                    Can.CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanControllerRef->CanControllerId;
                                    ReceiverMailBox.Hoh =
                                    Can.CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanObjectId;
                                    ReceiverPduInfo.SduDataPtr = CanMsgReceived.pui8MsgData;
                                    ReceiverPduInfo.SduLength = CanMsgReceived.ui32MsgLen;

                                    //CanIf_RxIndication(&ReceiverMailBox, &ReceiverPduInfo);
                                    UARTprintf("Can_MainFunction_Read Message Received %d\n", *(CanMsgReceived.pui8MsgData));


                                }
                                else
                                {
                                    /*MISRA*/
                                }
                            }
                        }
                        else
                        {

                        }
                    }
                    else
                    {

                    }
                }
            }
            else
            {

            }
        }
        else
        {

        }
    }
}




/*********************************************************************************************************************************
 Service name:                                       Can_MainFunction_BusOff
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

void Can_MainFunction_BusOff(void)
{
    /*
     * The Can module may implement the function Can_MainFunction_BusOff as empty define  in case no polling at
     * all is used.
     */
    if(Can.CanConfigSet.CanController[0U].CanBusOffProcessing == INTERRUPT || Can.CanConfigSet.CanController[1U].CanBusOffProcessing == INTERRUPT)
    {
        uint8 Iteration; /* Used In for loops to specify which CAN controller we dealing with */

        /* An Array consists of Controllers BASE ADDRESSES To Allow us with
         the help of the offsets and The GET_ADDRESS_Val To Get The Value of
         Specific Registers */

        uint32 temp_address_val;

        boolean BOFF; /*BussOff Indication variable*/

        for (Iteration = 0; Iteration < NUMBER_OF_CONTROLLERS ; Iteration++)
        {
            temp_address_val = GET_ADDRESS_VAL(Can.CanConfigSet.CanController[Iteration].CanControllerBaseAddress, CAN_O_STS);
            temp_address_val &= CAN_STS_BOFF;
            if (temp_address_val)
            {
                BOFF = true;
            }
            else
            {
                BOFF = false;
            }
            if (BOFF == true)
            {

                /*The CAN controller is in bus-off state
                 * Notify  */

                //CanIf_ControllerBusOff(Iteration);
                /* SetController Mode (Optional)*/
            }
            else
            {
                /*The CAN controller is not in bus-off state.*/
            }

        }
    }
    else
    {

    }
}

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
    boolean Init; /* boolean Variable used To store The Value Of INIT Bit*/
    CanIf_ControllerModeType temp_case;
    if (CanDriverState == CAN_READY)
    {
        for (Iteration = 0; Iteration < NUMBER_OF_CONTROLLERS ; Iteration++)
        {
            if (Can.CanConfigSet.CanController[Iteration].CanControllerActivation)
            {
                    temp_address_val = GET_ADDRESS_VAL((Can.CanConfigSet.CanController[Iteration].CanControllerBaseAddress),(CAN_O_CTL));
                    temp_address_val &= CAN_CTL_INIT;
                    if (temp_address_val)
                    {
                        Init = true;
                    }
                    else
                    {
                        Init = false;
                    }

                    if (GET_ADDRESS_VAL(
                            Can.CanConfigSet.CanController[Iteration].CanControllerBaseAddress,
                            CAN_O_CTL) == RST_CANCTL
                            && GET_ADDRESS_VAL(
                                    Can.CanConfigSet.CanController[Iteration].CanControllerBaseAddress,
                                    CAN_O_ERR) == RST_CANERR
                            && GET_ADDRESS_VAL(
                                    Can.CanConfigSet.CanController[Iteration].CanControllerBaseAddress,
                                    CAN_O_BIT) == RST_CANBIT
                            && GET_ADDRESS_VAL(
                                    Can.CanConfigSet.CanController[Iteration].CanControllerBaseAddress,
                                    CAN_O_TST) == RST_CANTST
                            && GET_ADDRESS_VAL(
                                    Can.CanConfigSet.CanController[Iteration].CanControllerBaseAddress,
                                    CAN_O_BRPE) == RST_CANBRPE)
                    {

                        /*
                         * CAN controller state UNINIT
                         * The CAN controller is not initialized.
                         * All registers belonging to the CAN module are in reset state, CAN interrupts are disabled.
                         * The CAN Controller is not participating on the CAN bus.
                         */

                        temp_case = CANIF_CS_UNINIT;

                    }
                    else if (Init == true && LogicalSleep[Iteration] == true)
                    {
                        /*
                         * CAN controller state SLEEP
                         * The hardware settings only differ from state STOPPED for CAN hardware that support a sleep mode
                         * The(wake-up over CAN bus directly supported by CAN hardware).
                         */
                        temp_case = CANIF_CS_SLEEP;

                    }
                    else if (Init == true && LogicalSleep[Iteration] == false)
                    {
                        /*
                         * CAN controller state STOPPED
                         * In this state the CAN Controller is initialized but does not participate on the bus.
                         * In addition, error frames and acknowledges must not be sent.
                         */
                        temp_case = CANIF_CS_STOPPED;

                    }
                    else if (Init == false)
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

                    }

                    if (CurrentCanState[Iteration] == temp_case)
                    {

                    }
                    else
                    {
                        /*
                         * [SWS_Can_00373] ?The function Can_Mainfunction_Mode shall call the function
                         CanIf_ControllerModeIndication to notify the upper layer about a successful state transition
                         of the corresponding CAN controller referred by abstract CanIf ControllerId,
                         in case the state transition was triggered by function Can_SetControllerMode.
                         */

                        //CanIf_ControllerModeIndication(Iteration, temp_case);
                        switch(temp_case)
                        {
                        case CANIF_CS_STARTED:
                            UARTprintf("Can_MainFunction_Mode Started\n");
                            break;
                        case CANIF_CS_STOPPED:
                            UARTprintf("Can_MainFunction_Mode Stopped\n");
                            break;
                        case CANIF_CS_SLEEP:
                            UARTprintf("Can_MainFunction_Mode Sleep\n");
                            break;
                        default:
                           UARTprintf("Can_MainFunction_Mode Error\n");
                           break;
                        }
                        CurrentCanState[Iteration] = temp_case;
                    }
            }
            else
            {

            }
        }
    }
    else
    {
        #if(CanDevErrorDetection == true)
        Det_ReportError(MODULE_ID, INSTANCE_ID, 0x0c, CAN_E_UNINIT);
        #endif
    }
}

/*****************************************************************************************************************************
 Service name:                                       Can_SetBaudrate
 Service ID[hex]:                                          0x0f
 Sync/Async:                                            Synchronous
 Reentrancy:                  Reentrant for different Controllers. Non reentrant for the same Controller.

 Parameters (in):                   Controller CAN controller, whose baud rate shall be set.
 BaudRateConfigID references a baud rate configuration by ID (see CanControllerBaudRateConfigID)

 Parameters (out):                                          None

 Return value:                  Std_ReturnType E_OK: Service request accepted, setting of (new) baud rate started
 E_NOT_OK: Service request not accepted

 Description:                       This service shall set the baud rate configuration of the CAN controller.
 Depending on necessary baud rate modifications the controller might have to reset.

 *******************************************************************************************************************************/
Std_ReturnType Can_SetBaudrate(uint8 Controller, uint16 BaudRateConfigID)
{
    uint8 BaudRateIndex;
    tCANBitClkParms CanBitTimingParameters;

    if(CanDriverState == CAN_READY)
    {
        if (Controller >= NUMBER_OF_CONTROLLERS)
        {
            /*[SWS_CAN_00494]? If default error detection for the Can module is enabled
             the function Can_SetBaudrate shall raise the error CAN_E_PARAM_CONTROLLER
             and return E_NOT_OK if the parameter Controller is out of range.?()*/

            #if(CanDevErrorDetection == true)
                Det_ReportError(MODULE_ID, INSTANCE_ID, 0x0f, CAN_E_PARAM_CONTROLLER);
            #endif

            return E_NOT_OK;
        }
        else
        {
            if(CanControllerState[Controller] == CAN_CS_STOPPED)
            {
                if(BaudRateConfigID < NUMBER_OF_BAUDRATES)
                {
                    for(BaudRateIndex = 0; BaudRateIndex < NUMBER_OF_BAUDRATES ; BaudRateIndex++)
                    {
                        if (Can.CanConfigSet.CanController[Controller].CanControllerBaudrateConfig[BaudRateIndex].CanControllerBaudRateConfigId == BaudRateConfigID)
                        {

                            CanBitTimingParameters.ui32SyncPropPhase1Seg =
                                    Can.CanConfigSet.CanController[Controller].CanControllerBaudrateConfig[BaudRateIndex].CanControllerPropSeg
                                    +\
                                    Can.CanConfigSet.CanController[Controller].CanControllerBaudrateConfig[BaudRateIndex].CanControllerSeg1 + 1U;

                            CanBitTimingParameters.ui32Phase2Seg =
                                    Can.CanConfigSet.CanController[Controller].CanControllerBaudrateConfig[BaudRateIndex].CanControllerSeg2;

                            CanBitTimingParameters.ui32SJW =
                                    Can.CanConfigSet.CanController[Controller].CanControllerBaudrateConfig[BaudRateIndex].CanControllerSyncJumpWidth;

                            CanBitTimingParameters.ui32QuantumPrescaler = SysCtlClockGet() / ((CanBitTimingParameters.ui32SyncPropPhase1Seg + CanBitTimingParameters.ui32Phase2Seg) * (Can.CanConfigSet.CanController[Controller].CanControllerDefaultBaudrate->CanControllerBaudRate * 1000U));

                            CANBitTimingSet(Can.CanConfigSet.CanController[Controller].CanControllerBaseAddress, &CanBitTimingParameters);

                            return E_OK;
                        }
                        else
                        {
                            /*error not BaudRateConfigID not in range*/
                            /*MISRA*/
                        }
                    }
                }
                else
                {
                    /*[SWS_CAN_00493]? If default error detection for the Can module is enabled:
                     The function Can_SetBaudrate shall raise the error CAN_E_PARAM_BAUDRATE
                     and return E_NOT_OK if the parameter BaudRateConfigID has an invalid value.?()*/
                    #if(CanDevErrorDetection == true)
                    Det_ReportError(MODULE_ID, INSTANCE_ID, 0x0f, CAN_E_PARAM_BAUDRATE);
                    #endif
                    return E_NOT_OK;
                }
            }
            else
            {
                /*error not stopped*/
                return E_NOT_OK;
            }
        }
    }
    else
    {
        /*comment!
         [SWS_CAN_00492]? If default error detection for the Can module is enabled:
         The function Can_SetBaudrate shall raise the error CAN_E_UNINIT and return
         E_NOT_OK if the driver is not yet initialized.?()*/
        #if(CanDevErrorDetection == true)
        Det_ReportError(MODULE_ID, INSTANCE_ID, 0x0f, CAN_E_UNINIT);
        #endif
        return E_NOT_OK;
    }
    return E_NOT_OK;
}



