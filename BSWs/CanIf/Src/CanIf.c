/********************************************************************************************************************************
 **
 FileName:                   CanIf.c
 AUTOSAR Version:            4.2.2
 **
 ********************************************************************************************************************************/
/********************************************************************************************************************************
 **                                                          Includes                                                                                                **
 ********************************************************************************************************************************/
#include "Can.h"
#include "CanIf.h"
#include "CanSM.h"
#include "Det.h"
/********************************************************************************************************************************
 **                                                       Global Variables                                                                                       **
 ********************************************************************************************************************************/
  CanIf_NotifStatusType TxPduState  [CanIfMaxTxPduCfg];
 CanIf_NotifStatusType RxPduState[CanIfMaxRxPduCfg];
 bool CanIfState=false;
 
 
 
 
 
 
 
 
 
extern CanIf_ConfigType CanIf;

static enum CanIfStateType
{
    CANIF_UNINIT, CANIF_READY
} CanIfState = CANIF_UNINIT;
static CanIf_ControllerModeType CanIfControllerMode[NUMBER_OF_CONTROLLERS ] = {
        CANIF_CS_UNINIT, CANIF_CS_UNINIT };
static CanIf_PduModeType CanIfPduMode[NUMBER_OF_CONTROLLERS ];
static const PduInfoType* RxBuffer[CanIfMaxRxPduCfg ];

#if(CanIfPublicReadRxPduNotifyStatusApi == true)
static CanIf_NotifStatusType RxPduState[CanIfMaxRxPduCfg ];
#endif

#if(CanIfPublicTxBuffering == true)
static CanIf_TxBufferType CanIfTxBuffer[NUMBER_OF_BUFFERS] = { {
        .CanIfBufferCfgRef = &CanIf.CanIfInitCfg.CanIfBufferCfg[0U],
        .CanIfTxBufferFront = -1, .CanIfTxBufferRear = -1, .CanIfTxBufferSize =
                -1,
        .CanIfTxBufferPduAvailable = { false } } };
#endif
uint8 CanIfDevelopmentError;

/********************************************************************************************************************************
 **                                                        Local Functions                                                                                        **
 *********************************************************************************************************************************/
#if(CanIfPublicTxBuffering == true)
static Std_ReturnType CanIf_TxBufferGet(
        CanIfBufferCfg* CanIfTxPduBufferRefLocal,
        CanIf_TxBufferType *CanIfTxBufferLocal);
static Std_ReturnType CanIf_TxBufferNotEmpty(
        CanIf_TxBufferType *CanIfTxBufferLocal);
static Std_ReturnType CanIf_TxBufferNotFull(
        CanIf_TxBufferType *CanIfTxBufferLocal);
static Std_ReturnType CanIf_TxBufferDequeue(CanIfTxPduCfg* TxPdu,
                                            const Can_PduType* PduInfoPtr);
static Std_ReturnType CanIf_TxBufferPeek(CanIfTxPduCfg* TxPdu,
                                         const Can_PduType* PduInfoPtr);
static Std_ReturnType CanIf_TxBufferEnqueue(CanIfTxPduCfg* TxPdu,
                                            const Can_PduType* PduInfoPtr);
static Std_ReturnType CanIf_TxBufferClear(uint8 ControllerId);
#endif
static Std_ReturnType CanIf_GetTxPdu(PduIdType CanIfTxSduId,
                                     CanIfTxPduCfg* TxPduPtr);
static Std_ReturnType CanIf_GetRxPdu(PduIdType CanIfRxSduId,
                                     CanIfRxPduCfg* RxPduPtr);

/*********************************************************************************************************************************
 Service name:                                       CanIf_TxBufferGet
 Parameters (in):                      CanIfTxPduBufferRefLocal-->Tx PDU reference to a CanIf buffer configuration.
 CanIfBufferRefLocal -->Reference to the Tx Buffer
 Parameters (inout):                                          None
 Parameters (out):                                            None
 Return value:                                            Std_ReturnType
 Description:							This service searches for the CanIf buffer referenced by the available
 CanIf buffers and the Tx Pdu
 *******************************************************************************************************************************/
#if(CanIfPublicTxBuffering == true)
Std_ReturnType CanIf_TxBufferGet(CanIfBufferCfg *CanIfTxPduBufferRefLocal,
                                 CanIf_TxBufferType *CanIfBufferRefLocal)
{
    uint8 BufferIndex;

    /* Check if the reference to the buffer configurations is a valid reference */
    if (CanIfTxPduBufferRefLocal == NULL)
    {
        return E_NOT_OK;
    }
    else
    {
        /* Loop over the available buffers and return the one with the same buffer configuration reference as the Tx PDU */
        for (BufferIndex = 0; BufferIndex < NUMBER_OF_BUFFERS; BufferIndex++)
        {
            if (CanIfTxBuffer[BufferIndex].CanIfBufferCfgRef
                    == CanIfTxPduBufferRefLocal)
            {
                CanIfBufferRefLocal = &CanIfTxBuffer[BufferIndex];
                return E_OK;
            }
            else
            {

            }
        }
    }

    return E_NOT_OK;
}
#endif

/*********************************************************************************************************************************
 Service name:                                       CanIf_TxBufferNotEmpty
 Parameters (in):                        CanIfTxBufferRefLocal -->Reference to the Tx Buffer
 Parameters (inout):                                          None
 Parameters (out):                                            None
 Return value:                                            Std_ReturnType
 Description:							This service checks if the buffer is empty
 *******************************************************************************************************************************/
#if(CanIfPublicTxBuffering == true)
Std_ReturnType CanIf_TxBufferNotEmpty(CanIf_TxBufferType *CanIfTxBufferRefLocal)
{
    /* Check if the reference to the buffer is a valid reference */
    if (CanIfTxBufferRefLocal == NULL)
    {
        return E_NOT_OK;
    }
    else
    {
        if (CanIfTxBufferRefLocal->CanIfTxBufferSize < 0)
        {
            return E_NOT_OK;
        }
        else
        {
            return E_OK;
        }
    }

}
#endif

/*********************************************************************************************************************************
 Service name:                                       CanIf_TxBufferNotFull
 Parameters (in):                        CanIfTxBufferRefLocal -->Reference to the Tx Buffer
 Parameters (inout):                                          None
 Parameters (out):                                            None
 Return value:                                            Std_ReturnType
 Description:							This service checks if the buffer is full
 *******************************************************************************************************************************/
#if(CanIfPublicTxBuffering == true)
Std_ReturnType CanIf_TxBufferNotFull(CanIf_TxBufferType *CanIfTxBufferRefLocal)
{
    /* Check if the reference to the buffer is a valid reference */
    if (CanIfTxBufferRefLocal == NULL)
    {
        return E_NOT_OK;
    }
    else
    {
        if (CanIfTxBufferRefLocal->CanIfTxBufferSize
                < CanIfTxBufferRefLocal->CanIfBufferCfgRef->CanIfBufferSize)
        {
            return E_OK;
        }
        else
        {
            return E_NOT_OK;
        }
    }

}
#endif

/*********************************************************************************************************************************
 Service name:                                       CanIf_TxBufferDequeue
 Parameters (in):                        		TxPdu -->Reference to the Tx PDU
 PduInfoPtr --> Reference to the PDU
 Parameters (inout):                                          None
 Parameters (out):                                            None
 Return value:                                            Std_ReturnType
 Description:							This service returns the next PDU in the FIFO buffer
 *******************************************************************************************************************************/
#if(CanIfPublicTxBuffering == true)
Std_ReturnType CanIf_TxBufferDequeue(CanIfTxPduCfg* TxPdu,
                                     const Can_PduType* PduInfoPtr)
{
    CanIf_TxBufferType* CanIfTxBufferRefLocal = NULL;

    /* Check if the reference to the TxPdu is a valid reference */
    if (TxPdu == NULL)
    {
        return E_NOT_OK;
    }
    else
    {
        /* Get the buffer of the Tx PDU */
        if (CanIf_TxBufferGet(TxPdu->CanIfTxPduBufferRef,
                              CanIfTxBufferRefLocal) == E_NOT_OK)
        {
            return E_NOT_OK;
        }
        else
        {
            /* Check if the buffer is empty */
            if (CanIf_TxBufferNotEmpty(CanIfTxBufferRefLocal) == E_NOT_OK)
            {
                return E_NOT_OK;
            }
            else
            {
                /* Set the PDU pointer to the next PDU in the buffer */
                PduInfoPtr =
                        &CanIfTxBufferRefLocal->CanIfTxBufferPduData[CanIfTxBufferRefLocal->CanIfTxBufferFront];
                /* Set the PDU state as not available in the buffer */
                CanIfTxBufferRefLocal->CanIfTxBufferPduAvailable[TxPdu->CanIfTxPduId] =
                false;
                /* Decrease the current available PDUs */
                CanIfTxBufferRefLocal->CanIfTxBufferSize--;
                CanIfTxBufferRefLocal->CanIfTxBufferFront++;
                return E_OK;
            }
        }
    }

}
#endif

/*********************************************************************************************************************************
 Service name:                                       CanIf_TxBufferPeek
 Parameters (in):                               TxPdu -->Reference to the Tx PDU
 PduInfoPtr --> Reference to the PDU
 Parameters (inout):                                          None
 Parameters (out):                                            None
 Return value:                                            Std_ReturnType
 Description:                           This service only looks at the next PDU in the buffer without dequeuing
 *******************************************************************************************************************************/
#if(CanIfPublicTxBuffering == true)
Std_ReturnType CanIf_TxBufferPeek(CanIfTxPduCfg* TxPdu,
                                  const Can_PduType* PduInfoPtr)
{
    CanIf_TxBufferType* CanIfTxBufferRefLocal = NULL;

    /* Check if the reference to the TxPdu is a valid reference */
    if (TxPdu == NULL)
    {
        return E_NOT_OK;
    }
    else
    {
        /* Get the buffer of the Tx PDU */
        if (CanIf_TxBufferGet(TxPdu->CanIfTxPduBufferRef,
                              CanIfTxBufferRefLocal) == E_NOT_OK)
        {
            return E_NOT_OK;
        }
        else
        {
            /* Check if the buffer is empty */
            if (CanIf_TxBufferNotEmpty(CanIfTxBufferRefLocal) == E_NOT_OK)
            {
                return E_NOT_OK;
            }
            else
            {
                /* Set the PDU pointer to the next PDU in the buffer */
                PduInfoPtr =
                        &CanIfTxBufferRefLocal->CanIfTxBufferPduData[CanIfTxBufferRefLocal->CanIfTxBufferFront];
                return E_OK;
            }
        }
    }

}
#endif

/*********************************************************************************************************************************
 Service name:                                       CanIf_TxBufferEnqueue
 Parameters (in):                        		TxPdu -->Reference to the Tx PDU
 PduInfoPtr --> Reference to the PDU
 Parameters (inout):                                          None
 Parameters (out):                                            None
 Return value:                                            Std_ReturnType
 Description:								This service adds a PDU to the FIFO buffer
 *******************************************************************************************************************************/
#if(CanIfPublicTxBuffering == true)
Std_ReturnType CanIf_TxBufferEnqueue(CanIfTxPduCfg* TxPdu,
                                     const Can_PduType* PduInfoPtr)
{
    CanIf_TxBufferType* CanIfTxBufferRefLocal = NULL;

    /* Check if the reference to the PDUs is a valid reference */
    if (TxPdu == NULL || PduInfoPtr == NULL)
    {
        return E_NOT_OK;
    }
    else
    {
        /* Get the buffer of the Tx PDU */
        if (CanIf_TxBufferGet(TxPdu->CanIfTxPduBufferRef,
                              CanIfTxBufferRefLocal) == E_NOT_OK)
        {
            return E_NOT_OK;
        }
        else
        {
            /* Check if the buffer is full */
            if (CanIf_TxBufferNotFull(CanIfTxBufferRefLocal) == E_NOT_OK)
            {
                return E_NOT_OK;
            }
            else
            {
                /* Check if the PDU is already stored in the buffer */
                if (CanIfTxBufferRefLocal->CanIfTxBufferPduAvailable[TxPdu->CanIfTxPduId]
                        == true)
                {
                    return E_NOT_OK;
                }
                else
                {
                    /* The buffer is not initialized yet. We will initialize the buffer by making both front and rear 0 and size 1
                     and give a value to the first PDU */
                    if (CanIfTxBufferRefLocal->CanIfTxBufferSize < 0)
                    {
                        CanIfTxBufferRefLocal->CanIfTxBufferPduData[0] =
                                *PduInfoPtr;
                        CanIfTxBufferRefLocal->CanIfTxBufferRear = 0;
                        CanIfTxBufferRefLocal->CanIfTxBufferFront = 0;
                        CanIfTxBufferRefLocal->CanIfTxBufferSize = 1;
                    }
                    /* This is the case when the rear is the last PDU in the buffer. We will add a new PDU at the index 0 */
                    else if (CanIfTxBufferRefLocal->CanIfTxBufferRear
                            == CanIfTxBufferRefLocal->CanIfBufferCfgRef->CanIfBufferSize
                                    - 1)
                    {
                        CanIfTxBufferRefLocal->CanIfTxBufferPduData[0] =
                                *PduInfoPtr;
                        CanIfTxBufferRefLocal->CanIfTxBufferRear = 0;
                        CanIfTxBufferRefLocal->CanIfTxBufferSize++;
                    }
                    /* This is the normal case*/
                    else
                    {
                        CanIfTxBufferRefLocal->CanIfTxBufferPduData[CanIfTxBufferRefLocal->CanIfTxBufferRear
                                + 1] = *PduInfoPtr;
                        CanIfTxBufferRefLocal->CanIfTxBufferRear++;
                        CanIfTxBufferRefLocal->CanIfTxBufferSize++;
                    }

                    /* Set the PDU as currently available in the buffer */
                    CanIfTxBufferRefLocal->CanIfTxBufferPduAvailable[TxPdu->CanIfTxPduId] =
                    true;
                    return E_OK;
                }
            }
        }
    }

}
#endif

/*********************************************************************************************************************************
 Service name:                                       CanIf_TxBufferClear
 Parameters (in):                             ControllerId -->Controller Id
 Parameters (inout):                                          None
 Parameters (out):                                            None
 Return value:                                            Std_ReturnType
 Description:                               This service clear the FIFO buffer
 *******************************************************************************************************************************/
Std_ReturnType CanIf_TxBufferClear(uint8 ControllerId)
{
    uint8 BufferIndex;
    uint8 PduIndex;
    if (ControllerId < NUMBER_OF_CONTROLLERS)
    {
        for (BufferIndex = (uint8) 0; BufferIndex < NUMBER_OF_BUFFERS;
                BufferIndex++)
        {
            if (CanIfTxBuffer[BufferIndex].CanIfBufferCfgRef->CanIfBufferHthRef->CanIfHthCanCtrlIdRef->CanIfCtrlId
                    == ControllerId)
            {
                CanIfTxBuffer[BufferIndex].CanIfTxBufferFront = -1;
                CanIfTxBuffer[BufferIndex].CanIfTxBufferRear = -1;
                CanIfTxBuffer[BufferIndex].CanIfTxBufferSize = -1;

                for (PduIndex = 0; PduIndex < CanIfMaxTxPduCfg ; PduIndex++)
                {
                    CanIfTxBuffer[BufferIndex].CanIfTxBufferPduAvailable[PduIndex] =
                    false;
                }
            }
            else
            {

            }
        }
    }
    else
    {
        return E_NOT_OK;
    }

    return E_OK;

}

/*********************************************************************************************************************************
 Service name:                                       CanIf_GetTxPdu
 Parameters (in):                        		CanIfTxSduId -->Id of the Tx PDU
 TxPduPtr --> Reference to the Tx PDU
 Parameters (inout):                                          None
 Parameters (out):                                            None
 Return value:                                            Std_ReturnType
 Description:								This service returns the Tx PDU with the desired ID
 *******************************************************************************************************************************/
Std_ReturnType CanIf_GetTxPdu(PduIdType CanIfTxSduId, CanIfTxPduCfg* TxPduPtr)
{
    uint32 TxPduIndex;
    if (CanIfTxSduId < CanIfMaxTxPduCfg)
    {
        for (TxPduIndex = 0; TxPduIndex < CanIfMaxTxPduCfg ; TxPduIndex++)
        {
            if (CanIf.CanIfInitCfg.CanIfTxPduCfg[TxPduIndex].CanIfTxPduId
                    == CanIfTxSduId)
            {
                TxPduPtr = &CanIf.CanIfInitCfg.CanIfTxPduCfg[TxPduIndex];
                return E_OK;
            }
            else
            {

            }
        }
    }
    else
    {
        return E_NOT_OK;
    }

    return E_NOT_OK;
}

/*********************************************************************************************************************************
 Service name:                                       CanIf_GetRxPdu
 Parameters (in):                               CanIfRxSduId -->Id of the Rx PDU
 RxPduPtr --> Reference to the Rx PDU
 Parameters (inout):                                          None
 Parameters (out):                                            None
 Return value:                                            Std_ReturnType
 Description:                               This service returns the Rx PDU with the desired ID
 *******************************************************************************************************************************/
Std_ReturnType CanIf_GetRxPdu(PduIdType CanIfRxSduId, CanIfRxPduCfg* RxPduPtr)
{
    uint32 RxPduIndex;
    if (CanIfRxSduId < CanIfMaxRxPduCfg)
    {
        for (RxPduIndex = 0; RxPduIndex < CanIfMaxRxPduCfg ; RxPduIndex++)
        {
            if (CanIf.CanIfInitCfg.CanIfRxPduCfg[RxPduIndex].CanIfRxPduId
                    == CanIfRxSduId)
            {
                RxPduPtr = &CanIf.CanIfInitCfg.CanIfRxPduCfg[RxPduIndex];
                return E_OK;
            }
            else
            {

            }
        }
    }
    else
    {
        return E_NOT_OK;
    }

    return E_NOT_OK;
}

/********************************************************************************************************************************
 **                                                      Functions  definition                                                                        **
 *********************************************************************************************************************************/

/*********************************************************************************************************************************
 Service name:                                              CanIf_Init
 Service ID[hex]:                                               0x01
 Sync/Async:                                               Synchronous
 Reentrancy:                                               Non Reentrant
 Parameters (in):                                           ConfigPtr
 Parameters (inout):                                          None
 Parameters (out):                                             None
 Return value:                                                   None
 Description:
 This service Initializes internal and external interfaces of the CAN Inter-
 face for the further processing.
 *******************************************************************************************************************************/
void CanIf_Init(const CanIf_ConfigType* ConfigPtr)
{

    uint8 Iterator_1 = (uint8) 0;

    if (CanIfState == CANIF_UNINIT)
    {
        /*
         The CanIf expects that the CAN Controller remains in STOPPED mode like after poweron
         reset after the initialization process has been completed. In this mode the CanIf and
         CanDrv are neither able to transmit nor receive CAN L-PDUs (see [SWS_CANIF_00001]).
         */
        for (Iterator_1 = (uint8) 0; Iterator_1 < NUMBER_OF_CONTROLLERS ;
                Iterator_1++)
        {
            CanIfControllerMode[Iterator_1] = CANIF_CS_STOPPED;
            CanIfPduMode[Iterator_1] = CANIF_OFFLINE;
        }

        CanIfState = CANIF_READY;/*Initialization is done*/
    }
    else
    {
        /* MISRA */
    }
}

/*********************************************************************************************************************************
 Service name:                                       CanIf_SetControllerMode
 Service ID[hex]:                                               0x03
 Sync/Async:                                               Asynchronous
 Reentrancy:                                               Reentrant (Not for the same controller)
 Parameters (in):                                           ControllerId      -->Abstracted CanIf ControllerId which is assigned to a
 CAN controller, which is requested for mode transition.
 ControllerMode -->Requested mode transition
 Parameters (inout):                                          None
 Parameters (out):                                             None
 Return value:                                                   Std_ReturnType
 Description:
 This service calls the corresponding CAN Driver service for changing of
 the CAN controller mode.
 *******************************************************************************************************************************/

Std_ReturnType CanIf_SetControllerMode(uint8 ControllerId,
                                       CanIf_ControllerModeType ControllerMode)
{

}

/*********************************************************************************************************************************
 Service name:                                       CanIf_GetControllerMode
 Service ID[hex]:                                               0x04
 Sync/Async:                                               Synchronous
 Reentrancy:                                               Non Reentrant
 Parameters (in):                                           ControllerId      -->Abstracted CanIf ControllerId which is assigned to a
 CAN controller, which is requested for mode transition.
 Parameters (inout):                                          None
 Parameters (out):                                     ControllerModePtr -->Pointer to a memory location, where the current
 mode of the CAN controller will be stored.
 Return value:                                                   Std_ReturnType
 Description:
 This service reports about the current status of the requested CAN controller.
 *******************************************************************************************************************************/
Std_ReturnType CanIf_GetControllerMode(
        uint8 ControllerId, CanIf_ControllerModeType* ControllerModePtr)
{

}

/*********************************************************************************************************************************
 Service name:                                       CanIf_Transmit
 Service ID[hex]:                                               0x05
 Sync/Async:                                               Synchronous
 Reentrancy:                                                 Reentrant
 Parameters (in):                                           CanIfTxSduId    -->L-SDU handle to be transmitted.This handle specifies the corresponding CAN L-
 SDU ID and implicitly the CAN Driver instance as
 well as the corresponding CAN controller device.
 CanIfTxInfoPtr   -->Pointer to a structure with CAN L-SDU related data:
 DLC and pointer to CAN L-SDU buffer including the
 MetaData of dynamic L-PDUs.
 Parameters (inout):                                          None
 Parameters (out):                                            None
 Return value:                                              Std_ReturnType
 Description:
 This service initiates a request for transmission of the CAN L-PDU specified by
 the CanTxSduId and CAN related data in the L-SDU structure.
 *******************************************************************************************************************************/

Std_ReturnType CanIf_Transmit(PduIdType CanIfTxSduId,
                              const PduInfoType* CanIfTxInfoPtr)
{
    CanIf_ControllerModeType CanIf_GetControllerModeReturn;
    CanIf_PduModeType CanIf_GetPduModeReturn;
    Can_ReturnType Can_WriteReturn;
    CanIfTxPduCfg *TxPduPtr = NULL;

    /*  CanIf must be initialized after Power ON */
    if (CanIfState == CANIF_UNINIT)
    {
        return E_NOT_OK;
    }
    else
    {
        if (CanIf_GetTxPdu(CanIfTxSduId, TxPduPtr) == E_NOT_OK)
        {
            return E_NOT_OK;
        }
        else
        {
            /* [SWS_CANIF_00317] The service CanIf_Transmit() shall not accept a transmit
             request, if the controller mode is not CANIF_CS_STARTED and the channel mode at
             least for the transmit path is not online or off-line active. */
            if (CanIf_GetControllerMode(
                    TxPduPtr->CanIfTxPduBufferRef->CanIfBufferHthRef->CanIfHthCanCtrlIdRef->CanIfCtrlId,
                    &CanIf_GetControllerModeReturn) == E_NOT_OK)
            {
                return E_NOT_OK;
            }
            else
            {
                if (CanIf_GetControllerModeReturn != CANIF_CS_STARTED)
                {
                    return E_NOT_OK;
                }
                else
                {
                    if (CanIf_GetPduMode(
                            TxPduPtr->CanIfTxPduBufferRef->CanIfBufferHthRef->CanIfHthCanCtrlIdRef->CanIfCtrlId,
                            &CanIf_GetPduModeReturn) == E_NOT_OK)
                    {
                        return E_NOT_OK;
                    }
                    else
                    {
                        if (CanIf_GetPduModeReturn == CANIF_OFFLINE
                                || CanIf_GetPduModeReturn == CANIF_TX_OFFLINE)
                        {
                            return E_NOT_OK;
                        }
                        else
                        {
                            /* [SWS_CANIF_00320] If parameter CanIfTxInfoPtr of CanIf_Transmit() has
                             an invalid value, CanIf shall report development error code CANIF_E_PARAM_POINTER
                             to the Det_ReportError service of the DET module, when CanIf_Transmit()
                             is called. */
                            if (CanIfTxInfoPtr == NULL)
                            {
#if(CanIfPublicDevErrorDetect == true)
                                Det_ReportError(MODULE_ID, INSTANCE_ID, 0x05, CANIF_E_PARAM_POINTER);
#endif
                                return E_NOT_OK;
                            }
                            else
                            {
                                /* [SWS_CANIF_00319] If parameter CanIfTxSduId of CanIf_Transmit() has an
                                 invalid value, CanIf shall report development error code CANIF_E_INVALID_TXPDUID
                                 to the Det_ReportError service of the DET, when CanIf_Transmit() is called. */
                                if (CanIfTxSduId >= CanIfMaxTxPduCfg)
                                {
#if(CanIfPublicDevErrorDetect == true)
                                    Det_ReportError(MODULE_ID, INSTANCE_ID, 0x05, CANIF_E_INVALID_TXPDUID);
#endif
                                    return E_NOT_OK;
                                }
                                else
                                {
                                    /* [SWS_CANIF_00893] When CanIf_Transmit() is called with CanIfTxInfoPtr-
                                     >SduLength exceeding the maximum length of the PDU referenced by CanIfTxSduId:
                                     • SduLength > 8 if the Can_IdType indicates a classic CAN frame
                                     • SduLength > 64 if the Can_IdType indicates a CAN FD frame

                                     CanIf shall report development error code CANIF_E_DATA_LENGTH_MISMATCH to
                                     the Det_ReportError service of the DET. */
                                    if (CanIfTxInfoPtr->SduLength > 8)
                                    {
#if(CanIfPublicDevErrorDetect == true)
                                        Det_ReportError(MODULE_ID, INSTANCE_ID, 0x05, CANIF_E_DATA_LENGTH_MISMATCH);
#endif
                                        return E_NOT_OK;
                                    }
                                    else
                                    {
                                        /* [SWS_CANIF_00318] The service CanIf_Transmit() shall map the parameters
                                         of the data structure:
                                         • the L-SDU handle (CanIfTxSduId) refers to (CanID, HTH/HRH of the CAN Controller)
                                         • and the CanIfTxInfoPtr which specifies length and data pointer of the Transmit Request
                                         to the corresponding CanDrv and call the function Can_Write(Hth, *PduInfo)
                                         Note: CanIfTxInfoPtr is a pointer to a L-SDU user memory, CAN Identifier, L-SDU
                                         handle and DLC */
                                        Can_HwHandleType Hth =
                                                TxPduPtr->CanIfTxPduBufferRef->CanIfBufferHthRef->CanIfHthIdSymRef->CanObjectId;

                                        const Can_PduType* PduInfoPtr =
                                                &(Can_PduType ) {
                                                                .swPduHandle =
                                                                        CanIfTxSduId,
                                                                .length =
                                                                        CanIfTxInfoPtr->SduLength,
                                                                .id =
                                                                        TxPduPtr->CanIfTxPduCanId,
                                                                .sdu =
                                                                        CanIfTxInfoPtr->SduDataPtr };

                                        /* [SWS_CANIF_00162] d If the call of Can_Write() returns E_OK the transmit request
                                         service CanIf_Transmit() shall return E_OK. c()
                                         Note: If the call of Can_Write() returns CAN_NOT_OK, then the transmit request
                                         service CanIf_Transmit() shall return E_NOT_OK. If the transmit request service
                                         CanIf_Transmit() returns E_NOT_OK, then the upper layer module is responsible
                                         to repeat the transmit request. */
                                        Can_WriteReturn = Can_Write(Hth,
                                                                    PduInfoPtr);

                                        if (Can_WriteReturn == CAN_OK)
                                        {
                                            return E_OK;
                                        }
                                        else if (Can_WriteReturn == CAN_BUSY)
                                        {
#if(CanIfPublicTxBuffering == true)
                                            if (CanIf_TxBufferEnqueue(
                                                    TxPduPtr,
                                                    PduInfoPtr) == E_OK)
                                            {
                                                return E_OK;
                                            }
                                            else
                                            {
                                                return E_NOT_OK;
                                            }
#else
                                            return E_NOT_OK;
#endif
                                        }
                                        else
                                        {
                                            return E_NOT_OK;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

/*********************************************************************************************************************************
 Service name:                                       CanIf_ReadRxPduData
 Service ID[hex]:                                               0x06
 Sync/Async:                                               Synchronous
 Reentrancy:                                                 Non Reentrant
 Parameters (in):                                           CanIfRxSduId    -->Receive L-SDU handle specifying the corresponding 
 CAN L-SDU ID and implicitly the CAN Driver instance
 as well as the corresponding CAN controller device.
 Parameters (inout):                                          None
 Parameters (out):                                       CanIfRxInfoPtr   -->Pointer to a structure with CAN L-SDU related data:
 DLC and pointer to CAN L-SDU buffer including the
 MetaData of dynamic L-PDUs.
 Return value:                                              Std_ReturnType
 Description:
 This service provides the CAN DLC and the received data of the requested
 CanIfRxSduId to the calling upper layer.
 *******************************************************************************************************************************/
/*[SWS_CANIF_00330]This API can be enabled or disabled at pre-compile time 
 configuration by the configuration parameter CANIF_PUBLIC_READRXPDU_DATA_API*/
#if(CanIfPublicReadRxPduDataApi)
Std_ReturnType CanIf_ReadRxPduData(PduIdType CanIfRxSduId,
                                   PduInfoType* CanIfRxInfoPtr)
{
    CanIf_ControllerModeType CanIf_GetControllerModeReturn;
    CanIfRxPduCfg *RxPduPtr = NULL;

    /*  CanIf must be initialized after Power ON */
    if (CanIfState == CANIF_UNINIT)
    {
        return E_NOT_OK;
    }
    else
    {
        if (CanIf_GetRxPdu(CanIfRxSduId, RxPduPtr) == E_NOT_OK)
        {
            return E_NOT_OK;
        }
        else
        {

            if (CanIf_GetControllerMode(
                    RxPduPtr->CanIfRxPduHrhIdRef->CanIfHrhCanCtrlIdRef->CanIfCtrlId,
                    &CanIf_GetControllerModeReturn) == E_NOT_OK)
            {
                return E_NOT_OK;
            }
            else
            {
                /*[SWS_CANIF_00324] d The function CanIf_ReadRxPduData() shall not accept a
                 request and return E_NOT_OK, if the corresponding CCMSM does not equal CANIF_CS_STARTED
                 and the channel mode is in the receive path online.*/
                if (CanIf_GetControllerModeReturn != CANIF_CS_STARTED)
                {
                    return E_NOT_OK;
                }
                else
                {
                    /*[SWS_CANIF_00326]If parameter CanIfRxInfoPtr of CanIf_ReadRxPduData()
                     has an invalid value, CanIf shall report development error code CANIF_E_PARAM_POINTER
                     to the Det_ReportError service of the DET module, when CanIf_ReadRxPduData() is called.*/
                    if ((CanIfRxInfoPtr->SduLength)
                            > 8|| (CanIfRxInfoPtr->SduDataPtr)==NULL)
                    {
#if(CanIfPublicDevErrorDetect == true)
                        Det_ReportError(MODULE_ID, INSTANCE_ID, 0x06, CANIF_E_PARAM_POINTER);
#endif
                        return E_NOT_OK;
                    }
                    else
                    {

                        CanIfRxInfoPtr = RxBuffer[CanIfRxSduId];
                        return E_OK;
                    }
                }
            }
        }
    }
}
#endif

/*********************************************************************************************************************************
 Service name:                                       CanIf_ReadTxNotifStatus
 Service ID[hex]:                                               0x07
 Sync/Async:                                               Synchronous
 Reentrancy:                                                 Non Reentrant
 Parameters (in):                                           CanIfTxSduId    -->L-SDU handle to be transmitted.
 This handle specifies the corresponding CAN L-
 SDU ID and implicitly the CAN Driver instance as
 well as the corresponding CAN controller device.
 Parameters (inout):                                          None
 Parameters (out):                                            None
 Return value:                                              CanIf_NotifStatusType
 Description:
 This service returns the confirmation status (confirmation occurred or not)
 of a specific static or dynamic CAN Tx L-PDU, requested by the
 CanIfTxSduId.
 *******************************************************************************************************************************/

/*[SWS_CANIF_00335] d Configuration of CanIf_ReadTxNotifyStatus(): This API
can be enabled or disabled at pre-compile time configuration globally by the parameter
CANIF_PUBLIC_READTXPDU_NOTIFY_STATUS_API (see ECUC_CanIf_00609)*/

#if(CanIfPublicReadTxPduNotifyStatusApi==true)

CanIf_NotifStatusType CanIf_ReadTxNotifStatus(PduIdType CanIfTxSduId){
	
	
    CanIf_NotifStatusType state_0x07;
    state_0x07 = CANIF_NO_NOTIFICATION;
    if (true == CanIfState)
    {
        /*[SWS_CANIF_00331] d If parameter CanIfTxSduId of
        CanIf_ReadTxNotifStatus() is out of range or if no status information was
        configured for this CAN Tx L-SDU, CanIf shall report development error code
        CANIF_E_INVALID_TXPDUID to the Det_ReportError service of the DET
        when CanIf_ReadTxNotifStatus() is called*/
        if (CanIfMaxTxPduCfg > CanIfTxSduId)
        {
            if (CANIF_TX_RX_NOTIFICATION == TxPduState[CanIfTxSduId])
            {
                state_0x07 = CANIF_TX_RX_NOTIFICATION;
                TxPduState[CanIfTxSduId]=CANIF_NO_NOTIFICATION;
            }
            else
            {
                ;
                /*MISRA*/
            }
        }
        else
        {
            /*THE ERROR TO BE RAISED HERE*/
            CanIfDevelopmentError = CANIF_E_INVALID_TXPDUID;
        }
    }
    else
    {
        CanIfDevelopmentError = CANIF_E_UNINIT;
        /*MISRA*/
    }
	
	
	
	















return state_0x07;
}
#endif

/*********************************************************************************************************************************
 Service name:                                       CanIf_ReadRxNotifStatus
 Service ID[hex]:                                               0x08
 Sync/Async:                                                 Synchronous
 Reentrancy:                                                 Non Reentrant
 Parameters (in):                                            CanIfRxSduId    -->Receive L-SDU handle specifying the corresponding 
 CAN L-SDU ID and implicitly the CAN Driver instance
 as well as the corresponding CAN controller device.
 Parameters (inout):                                          None
 Parameters (out):                                            None
 Return value:                                              CanIf_NotifStatusType
 Description:
 This service returns the indication status (indication occurred or not) of a
 specific CAN Rx L-PDU, requested by the CanIfRxSduId.
 *******************************************************************************************************************************/

/*[SWS_CANIF_00340] d Configuration of CanIf_ReadRxNotifStatus(): This API
can be enabled or disabled at pre-compile time configuration globally by the parameter
CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API (see ECUC_CanIf_00608).*/

#if (CanIfPublicReadRxPduNotifyStatusApi==true)

CanIf_NotifStatusType CanIf_ReadRxNotifStatus(PduIdType CanIfRxSduId){

        CanIf_NotifStatusType state_0x08;
        state_0x08 = CANIF_NO_NOTIFICATION;
        if (true == CanIfState)
        {
            /*[SWS_CANIF_00336] d If parameter CanIfRxSduId of
            CanIf_ReadRxNotifStatus() is out of range or if status for CanRxPduId
            was requested whereas CANIF_READRXPDU_DATA_API is disabled or if no status
            information was configured for this CAN Rx L-SDU, CanIf shall report development
            error code CANIF_E_INVALID_RXPDUID to the Det_ReportError service of
            the DET, when CanIf_ReadRxNotifStatus() is called. c*/
            if (CanIfMaxRxPduCfg > CanIfRxSduId)
            {
                if (CANIF_TX_RX_NOTIFICATION == RxPduState[CanIfRxSduId])
                {
                    state_0x08 = CANIF_TX_RX_NOTIFICATION;
                    RxPduState[CanIfRxSduId]=CANIF_NO_NOTIFICATION;
                }
                else
                {
                    ;
                    /*MISRA*/
                }
            }
            else
            {
                /*THE ERROR TO BE RAISED HERE*/
                CanIfDevelopmentError = CANIF_E_INVALID_RXPDUID;
            }
        }
        else
        {
            CanIfDevelopmentError = CANIF_E_UNINIT;
            /*MISRA*/
        }
	

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

	
return state_0x08;
	
}
#endif
/*********************************************************************************************************************************
 Service name:                                         CanIf_SetPduMode
 Service ID[hex]:                                               0x09
 Sync/Async:                                                 Synchronous
 Reentrancy:                                                 Non Reentrant
 Parameters (in):                                           ControllerId      -->Abstracted CanIf ControllerId which is assigned to a
 CAN controller, which is requested for mode transition.
 PduModeRequest-->Requested PDU mode change
 Parameters (inout):                                          None
 Parameters (out):                                            None
 Return value:                                              Std_ReturnType
 Description:
 This service sets the requested mode at the L-PDUs of a predefined logical PDU channel.
 *******************************************************************************************************************************/

Std_ReturnType CanIf_SetPduMode(uint8 ControllerId,
                                CanIf_PduModeType PduModeRequest)
{
    CanIf_ControllerModeType CanIfControllerModeLocal;
    /*
     [SWS_CANIF_00344] d Caveats of CanIf_SetPduMode(): CanIf must be initialized after Power ON.
     */
    if (CanIfState == CANIF_UNINIT)
    {
#if(CanIfPublicDevErrorDetect == true)
        Det_ReportError(MODULE_ID, INSTACE_ID, 0x09, CANIF_E_UNINIT);
#endif
        return E_NOT_OK;
    }
    else
    {
        /* [SWS_CANIF_00341] d If CanIf_SetPduMode() is called with invalid ControllerId
         , CanIf shall report development error code CANIF_E_PARAM_CONTROLLERID
         to the Det_ReportError service of the DET module. c(SRS_BSW_00323)
         */
        if (ControllerId >= NUMBER_OF_CONTROLLERS)
        {
#if(CanIfPublicDevErrorDetect == true)
            Det_ReportError(MODULE_ID, INSTACE_ID, 0x09, CANIF_E_PARAM_CONTROLLERID);
#endif
            return E_NOT_OK;
        }
        else
        {
            /*
             * [SWS_CANIF_00874] The service CanIf_SetPduMode() shall not accept any request and shall return E_NOT_OK,
             *  if the CCMSM referenced by ControllerId is notin state CANIF_CS_STARTED.
             */
            if (CanIf_GetControllerMode(ControllerId,
                                        &CanIfControllerModeLocal) == E_NOT_OK)
            {
                return E_NOT_OK;
            }
            else
            {
                if (CanIfControllerModeLocal != CANIF_CS_STARTED)
                {
                    return E_NOT_OK;
                }
                else
                {
                    /*
                     * [SWS_CANIF_00860] If CanIf_SetPduMode() is called with invalid PduModeRequest,
                     CanIf shall report development error code CANIF_E_PARAM_PDU_MODE to the Det_ReportError service of the DET module.
                     */
                    if (PduModeRequest != CANIF_OFFLINE
                            && PduModeRequest != CANIF_TX_OFFLINE
                            && PduModeRequest != CANIF_TX_OFFLINE_ACTIVE
                            && PduModeRequest != CANIF_ONLINE)
                    {
#if(CanIfPublicDevErrorDetect == true)
                        Det_ReportError(MODULE_ID, INSTACE_ID, 0x09, CANIF_E_PARAM_PDU_MODE);
#endif
                        return E_NOT_OK;

                    }
                    else
                    {
                        CanIfPduMode[ControllerId] = PduModeRequest;
                        return E_OK;
                    }
                }
            }
        }
    }
}


/*********************************************************************************************************************************
 Service name:                                         CanIf_GetPduMode
 Service ID[hex]:                                               0x0a
 Sync/Async:                                                 Synchronous
 Reentrancy:                                                 Reentrant (Not for the same channel)
 Parameters (in):                                           ControllerId      -->Abstracted CanIf ControllerId which is assigned to a
 CAN controller, which is requested for mode transition.
 Parameters (inout):                                          None
 Parameters (out):                                            PduModePtr  -->Pointer to a memory location, where the current
 mode of the logical PDU channel will be stored.
 Return value:                                              Std_ReturnType
 Description:
 This service reports the current mode of a requested PDU channel.
 *******************************************************************************************************************************/

Std_ReturnType CanIf_GetPduMode(uint8 ControllerId,
                                CanIf_PduModeType* PduModePtr)
{

    /*  CanIf must be initialized after Power ON */
    if (CanIfState == CANIF_UNINIT)
    {
        return E_NOT_OK;
    }
    else
    {
        /* [SWS_CANIF_00346] If CanIf_GetPduMode() is called with invalid ControllerId, CanIf shall report development error code CANIF_E_PARAM_CONTROLLERID
         to the Det_ReportError service of the DET module.  */
        if (ControllerId >= NUMBER_OF_CONTROLLERS)
        {
#if(CanIfPublicDevErrorDetect == true)
            Det_ReportError(MODULE_ID, INSTANCE_ID, 0x0A, CANIF_E_PARAM_CONTROLLERID);
#endif
            return E_NOT_OK;
        }
        else
        {
            /*[SWS_CANIF_00657] If CanIf_GetPduMode() is called with invalid PduModePtr,
             CanIf shall report development error code CANIF_E_PARAM_POINTER to the Det_ReportError
             service of the DET module.*/
            if (PduModePtr == NULL)
            {
#if(CanIfPublicDevErrorDetect == true)
                Det_ReportError(MODULE_ID, INSTANCE_ID, 0x0A, CANIF_E_PARAM_POINTER);
#endif
                return E_NOT_OK;
            }
            else
            {
                *PduModePtr = CanIfPduMode[ControllerId];
            }
        }
    }

    return E_NOT_OK;
}

/*********************************************************************************************************************************
 Service name:                                         CanIf_SetBaudrate
 Service ID[hex]:                                               0x27
 Sync/Async:                                                 Synchronous
 Reentrancy:                                                 Reentrant for different ControllerIds. Non reentrant for the same ControllerId.
 Parameters (in):                                           ControllerId      -->Abstracted CanIf ControllerId which is assigned to a
 CAN controller, which is requested for mode transition.
 BaudRateConfigID-->references a baud rate configuration by ID (see Can-
 ControllerBaudRateConfigID)
 Parameters (inout):                                          None
 Parameters (out):                                            PduModePtr  -->Pointer to a memory location, where the current
 mode of the logical PDU channel will be stored.
 Return value:                                              Std_ReturnType
 Description:
 This service shall set the baud rate configuration of the CAN controller.
 Depending on necessary baud rate modifications the controller might
 have to reset.
 *******************************************************************************************************************************/
#if(CanIfSetBaudrateApi==true)
Std_ReturnType CanIf_SetBaudrate(uint8 ControllerId, uint16 BaudRateConfigID)
{
    if (ControllerId >= NUMBER_OF_CONTROLLERS)
    {
        /*[SWS_CANIF_00869] If CanIf_SetBaudrate() is called with invalid
         ControllerId, CanIf shall report development error codeCANIF_E_PARAM_CONTROLLERID
         to the Det_ReportError service of the
         DET module */
#if(CanIfPublicDevErrorDetect == true)
        Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_API_ID,CANIF_E_PARAM_CONTROLLERID);
#endif
        return E_NOT_OK;
    }
    else
    {
        return Can_SetBaudrate(ControllerId, BaudRateConfigID);
    }
}
#endif

/*********************************************************************************************************************************
 Service name:                                         CanIf_TxConfirmation
 Service ID[hex]:                                               0x13
 Sync/Async:                                                 Synchronous
 Reentrancy:                                                 Reentrant
 Parameters (in):                                           CanTxPduId      -->L-PDU handle of CAN L-PDU successfully transmitted.
 This ID specifies the corresponding CAN L-PDU ID
 and implicitly the CAN Driver instance as well as the
 corresponding CAN controller device.
 Parameters (inout):                                          None
 Parameters (out):                                             None
 Return value:                                                   None
 Description:
 This service confirms a previously successfully processed transmission of a CAN TxPDU.
 *******************************************************************************************************************************/

void CanIf_TxConfirmation(PduIdType CanTxPduId)
{
    CanIfTxPduCfg* txpduptr_0x13 = 0;
    Can_ReturnType transmitcheck_0x13;
    CanIf_ControllerModeType CanIfControllerModeLocal;
    Can_PduType PduInfoPtr;

    /* The CanIf must be initialized after Power ON. */
    if (CanIfState == CANIF_UNINIT)
    {
#if(CanIfPublicDevErrorDetect == true)
        Det_ReportError(MODULE_ID, INSTANCE_ID, 0x13, CANIF_E_UNINIT);
#endif
    }
    else
    {
        /*[SWS_CANIF_00410] d If parameter CanTxPduId of CanIf_TxConfirmation()
         has an invalid value, CanIf shall report development error code CANIF_E_PARAM_LPDU
         to the Det_ReportError service of the DET module, when CanIf_TxConfirmation()
         is called.*/
        if (CanTxPduId > CanIfMaxTxPduCfg)
        {
#if(CanIfPublicDevErrorDetect == true)
            Det_ReportError(MODULE_ID, INSTANCE_ID, 0x13, CANIF_E_INVALID_TXPDUID);
#endif
        }
        else
        {
            if (CanIf_GetTxPdu(CanTxPduId, txpduptr_0x13) == E_NOT_OK)
            {

            }
            else
            {
                /*[SWS_CANIF_00740] d If CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT (see ECUC_CanIf_00
                 is enabled, CanIf shall buffer the information about a received TxConfirmation per
                 CAN Controller, if the CCMSM of that controller is in state CANIF_CS_STARTED.*/
#if(CanIfPublicTxConfirmPollingSupport == true)
                if(CanIf_GetControllerMode(txpduptr_0x13->CanIfTxPduBufferRef->CanIfBufferHthRef->CanIfHthCanCtrlIdRef->CanIfCtrlId, &CanIfControllerModeLocal) == E_NOT_OK)
                {

                }
                else
                {
                    if(CanIfControllerModeLocal != CANIF_CS_STARTED)
                    {

                    }
                    else
                    {
                        CanIf_TxConfirmationInfo[CanTxPduId]= true;
                    }
                }
#endif
#if(CanIfPublicTxBuffering == true)
                if (CanIf_TxBufferPeek(txpduptr_0x13, &PduInfoPtr) == E_NOT_OK)
                {

                }
                else
                {
                    /* Attempt to transmit a PDU from the buffer */
                    transmitcheck_0x13 =
                            Can_Write(
                                    txpduptr_0x13->CanIfTxPduBufferRef->CanIfBufferHthRef->CanIfHthIdSymRef->CanObjectId,
                                    &PduInfoPtr);
                    if (transmitcheck_0x13 == CAN_OK)
                    {
                        CanIf_TxBufferDequeue(txpduptr_0x13, &PduInfoPtr);
                    }
                    else
                    {

                    }
                }
#endif
#if(CanIfPublicReadTxPduNotifyStatusApi == true)
                /*
                 * [SWS_CANIF_00391] d If configuration parameters CANIF_PUBLIC_READTXPDU_NOTIFY_STATUS
                 (ECUC_CanIf_00609) and CANIF_TXPDU_READ_NOTIFYSTATUS (ECUC_CanIf_00589)
                 for the Transmitted L-PDU are set to TRUE, and if CanIf_TxConfirmation() is
                 called, CanIf shall set the notification status for the Transmitted L-PDU.
                 */
#endif
                /*
                 * [SWS_CANIF_00414] d Configuration of CanIf_TxConfirmation(): Each Tx L-
                 PDU (see ECUC_CanIf_00248) has to be configured with a corresponding transmit
                 confirmation service of an upper layer module (see [SWS_CANIF_00011]) which is
                 called in CanIf_TxConfirmation().
                 */

                if (txpduptr_0x13->CanIfTxPduUserTxConfirmationUL == PDUR)
                {
                    /* PDUR_TxConfirmation(CanTxPduId);*/
                }
                else if (txpduptr_0x13->CanIfTxPduUserTxConfirmationUL
                        == CAN_TP)
                {
                    /*CAN_TP_TxConfirmation(CanTxPduId);*/
                }
                else
                {
                    /* misra */
                }
            }
        }
    }
}

/*********************************************************************************************************************************
 Service name:                                         CanIf_RxIndication
 Service ID[hex]:                                               0x14
 Sync/Async:                                                 Synchronous
 Reentrancy:                                                 Reentrant
 Parameters (in):                                           Mailbox      -->Identifies the HRH and its corresponding CAN Controller.
 PduInfoPtr  -->Pointer to the received L-PDU.
 Parameters (inout):                                          None
 Parameters (out):                                             None
 Return value:                                                   None
 Description:
 This service indicates a successful reception of a received CAN Rx L-PDU
 to the CanIf after passing all filters and validation checks.
 *******************************************************************************************************************************/

void CanIf_RxIndication(const Can_HwType* Mailbox,
                        const PduInfoType* PduInfoPtr)
{
    /*[SWS_CANIF_00415] Within the service CanIf_RxIndication() the CanIf routes
     this indication to the configured upper layer target service(s).*/

    uint8 CanObjectIdIteration;
    CanIfRxPduCfg* RxPduPtr = NULL;

    if (CanIfState == CAN_UNINIT)
    {
        /*[SWS_CANIF_00421] d If CanIf was not initialized before calling CanIf_RxIndication(),
         CanIf shall not execute Rx indication handling, when CanIf_RxIndication(), is called.*/
    }
    else
    {
        if ((PduInfoPtr != NULL) && (Mailbox != NULL))
        {
            if (PduInfoPtr->SduLength > 8)
            {
#if (CanIfPublicDevErrorDetect == true)
                Det_ReportError(MODULE_ID, INSTANCE_ID, 0x14,CANIF_E_INVALID_DLC);
#endif
            }
            else
            {
                if (Mailbox->Hoh < NUMBER_OF_HOH)
                {
                    for (CanObjectIdIteration = 0;
                            CanObjectIdIteration < NUMBER_OF_HOH ;
                            CanObjectIdIteration++)
                    {
                        if (CanIf.CanIfInitCfg.CanIfRxPduCfg[CanObjectIdIteration].CanIfRxPduHrhIdRef->CanIfHrhIdSymRef->CanObjectId
                                == Mailbox->Hoh)
                        {
                            RxPduPtr =
                                    &CanIf.CanIfInitCfg.CanIfRxPduCfg[CanObjectIdIteration];
                            break;
                        }
                        else
                        {
                            /*Misra*/
                        }
                    }
                    if (RxPduPtr == NULL)
                    {

                    }
                    else
                    {
                        if ((Mailbox->CanId
                                >= (RxPduPtr->CanIfRxPduCanIdRange.CanIfRxPduCanIdRangeLowerCanId))
                                && (Mailbox->CanId
                                        <= (RxPduPtr->CanIfRxPduCanIdRange.CanIfRxPduCanIdRangeUpperCanId)))
                        {
                            /*[SWS_CANIF_00389] d If the function CanIf_RxIndication() is called, the CanIf
                             shall process the Software Filtering on the received L-PDU as specified in 7.20, if
                             configured (see multiplicity of ECUC_CanIf_00628 equals 0::) If Software Filtering
                             rejects the received L-PDU, the CanIf shall end the receive indication for that call of
                             CanIf_RxIndication().*/

                            /*[SWS_CANIF_00390] If the CanIf accepts an L-PDU received via CanIf_RxIndication()
                             during Software Filtering (see [SWS_CANIF_00389]), the CanIf shall process the DLC
                             check afterwards, if configured (see ECUC_CanIf_00617).*/

#if (CanIfPrivateDlcCheck == true)
                            if (PduInfoPtr->SduLength
                                    >= RxPduPtr->CanIfRxPduDlc)
                            {

                                /*[SWS_CANIF_00026] CanIf shall accept all received L-PDUs (see [SWS_CANIF_00390])
                                 with a DLC value equal or greater then the configured DLC value (see ECUC_CanIf_00599).
                                 c(SRS_CAN_01005)*/
                                RxBuffer[RxPduPtr->CanIfRxPduId] = PduInfoPtr;

                                /*[SWS_CANIF_00392] d If configuration parameters CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_(ECUC_CanIf_00608)
                                 and CANIF_RXPDU_READ_NOTIFYSTATUS (ECUC_CanIf_00595) for the Received L-PDU are set to TRUE,
                                 and if CanIf_RxIndication() is called,the CanIf shall set the notification status for the Received L-PDU. c()*/

#if(CanIfPublicReadRxPduNotifyStatusApi == true)
                                if (RxPduPtr->CanIfRxPduReadNotifyStatus == true)
                                {
                                    RxPduState[RxPduPtr->CanIfRxPduId] =
                                            CANIF_TX_RX_NOTIFICATION;
                                }
#elif(CanIfPublicReadRxPduNotifyStatusApi == false)
                                if(RxPduPtr->CanIfRxPduReadNotifyStatus == false)
                                {
                                    /*[SWS_CANIF_00056] d If CanIf accepts a L-PDU received via CanIf_RxIndication()
                                     during DLC check (see [SWS_CANIF_00390], [SWS_CANIF_00026]), CanIf shall
                                     identify if a target upper layer module was configured (see configuration descrption
                                     of [SWS_CANIF_00012] and ECUC_CanIf_00529, ECUC_CanIf_00530) to be called
                                     with its providing receive indication service for the received L-SDU.*/

                                    switch (RxPduPtr->CanIfRxPduUserRxIndicationUL)
                                    {
                                        case PDUR:

                                        /*[SWS_CANIF_00135] d If a target upper layer module was configured to be called
                                         with its providing receive indication service (see [SWS_CANIF_00056]), the CanIf shall
                                         call this configured receive indication callback service (see ECUC_CanIf_00530) and
                                         shall provide the parameters required for upper layer notification callback functions
                                         (see [SWS_CANIF_00012]) based on the parameters of CanIf_RxIndication().
                                         c(SRS_BSW_00325)*/

                                        /*[SWS_CANIF_00442] d Configuration of <User_RxIndication>(): If CANIF_RXPDU_USERRXINDICATION_is set to PDUR,
                                         CANIF_RXPDU_USERRXINDICATION_NAME must be PduR_CanIfRxIndication.*/

                                        PduR_CanIfRxIndication(RxPduPtr->CanIfRxPduId , PduInfoPtr);

                                        break;
                                        case CAN_TP:

                                        /*[SWS_CANIF_00135] d If a target upper layer module was configured to be called
                                         with its providing receive indication service (see [SWS_CANIF_00056]), the CanIf shall
                                         call this configured receive indication callback service (see ECUC_CanIf_00530) and
                                         shall provide the parameters required for upper layer notification callback functions
                                         (see [SWS_CANIF_00012]) based on the parameters of CanIf_RxIndication().
                                         c(SRS_BSW_00325)*/

                                        /*[SWS_CANIF_00448] d Configuration of <User_RxIndication>(): If CANIF_RXPDU_USERRXINDICATION_is set to CAN_TP,
                                         CANIF_RXPDU_USERRXINDICATION_NAME must be CanTp_RxIndication.*/

                                        CanTp_RxIndication(PduInfoPtr->CanIfRxPduId , PduInfoPtr);

                                        break;
                                        default:
                                        /*error*/

                                        break;

                                    }
                                }
#endif

                            }
                            else
                            {
                                /*[SWS_CANIF_00168] If the DLC check rejects a received L-PDU (see [SWS_CANIF_00026]),
                                 CanIf shall report development error code CANIF_E_INVALID_DLC to the Det_ReportError()
                                 service of the DET module.*/
#if (CanIfPublicDevErrorDetect == true)
                                Det_ReportError(MODULE_ID,INSTANCE_ID,0x14,CANIF_E_INVALID_DLC);
#endif
                            }

#else

                            /*[SWS_CANIF_00392] If configuration parameters CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_(ECUC_CanIf_00608)
                             and CANIF_RXPDU_READ_NOTIFYSTATUS (ECUC_CanIf_00595) for the Received L-PDU are set to TRUE,
                             and if CanIf_RxIndication() is called,the CanIf shall set the notification status for the Received L-PDU. c()*/

                            RxBuffer[PduInfoPtr->CanIfRxPduId] = PduInfoPtr;

#if(CanIfPublicReadRxPduNotifyStatusApi == true)
                            if(PduInfoPtr->CanIfRxPduReadNotifyStatus == true)
                            {
                                RxPduState[PduInfoPtr->CanIfRxPduId] = CANIF_TX_RX_NOTIFICATION;
                            }
                            else
                            {

                            }
#elif(CanIfPublicReadRxPduNotifyStatusApi == false)
                            if(PduInfoPtr->CanIfRxPduReadNotifyStatus == false)
                            {
                                /*[SWS_CANIF_00056] d If CanIf accepts a L-PDU received via CanIf_RxIndication()
                                 during DLC check (see [SWS_CANIF_00390], [SWS_CANIF_00026]), CanIf shall
                                 identify if a target upper layer module was configured (see configuration descrption
                                 of [SWS_CANIF_00012] and ECUC_CanIf_00529, ECUC_CanIf_00530) to be called
                                 with its providing receive indication service for the received L-SDU.*/

                                switch (RxPduPtr->CanIfRxPduUserRxIndicationUL)
                                {
                                    case PDUR:
                                    /*[SWS_CANIF_00135] d If a target upper layer module was configured to be called
                                     with its providing receive indication service (see [SWS_CANIF_00056]), the CanIf shall
                                     call this configured receive indication callback service (see ECUC_CanIf_00530) and
                                     shall provide the parameters required for upper layer notification callback functions
                                     (see [SWS_CANIF_00012]) based on the parameters of CanIf_RxIndication().
                                     c(SRS_BSW_00325)*/

                                    /*[SWS_CANIF_00442] d Configuration of <User_RxIndication>(): If CANIF_RXPDU_USERRXINDICATION_is set to PDUR,
                                     CANIF_RXPDU_USERRXINDICATION_NAME must be PduR_CanIfRxIndication.*/

                                    PduR_CanIfRxIndication(PduInfoPtr->CanIfRxPduId , PduInfoPtr);

                                    break;

                                    case CAN_TP:
                                    /*[SWS_CANIF_00135] d If a target upper layer module was configured to be called
                                     with its providing receive indication service (see [SWS_CANIF_00056]), the CanIf shall
                                     call this configured receive indication callback service (see ECUC_CanIf_00530) and
                                     shall provide the parameters required for upper layer notification callback functions
                                     (see [SWS_CANIF_00012]) based on the parameters of CanIf_RxIndication().
                                     c(SRS_BSW_00325)*/

                                    /*[SWS_CANIF_00448] d Configuration of <User_RxIndication>(): If CANIF_RXPDU_USERRXINDICATION_is set to CAN_TP,
                                     CANIF_RXPDU_USERRXINDICATION_NAME must be CanTp_RxIndication.*/

                                    CanTp_RxIndication(PduInfoPtr->CanIfRxPduId , PduInfoPtr);
                                    break;

                                    default:
                                    /*error*/

                                    break;

                                }
                            }
#endif

#endif
                        }
                        else
                        {
                            /*[SWS_CANIF_00417] d If parameter Mailbox->CanId of CanIf_RxIndication()
                             has an invalid value, CanIf shall report development error code CANIF_E_PARAM_CANID
                             to the Det_ReportError service of the DET module, when CanIf_RxIndication()
                             is called. c(SRS_BSW_00323)*/
#if (CanIfPublicDevErrorDetect == true)
                            Det_ReportError(MODULE_ID,INSTANCE_ID,0x14,CANIF_E_PARAM_CANID);
#endif
                        }
                    }
                }
                else
                {
                    /*[SWS_CANIF_00416] d If parameter Mailbox->Hoh of CanIf_RxIndication()
                     has an invalid value, CanIf shall report development error code CANIF_E_PARAM_HOH
                     to the Det_ReportError service of the DET module, when CanIf_RxIndication()
                     is called. c(SRS_BSW_00323)*/
#if (CanIfPublicDevErrorDetect == true)
                    Det_ReportError(MODULE_ID,INSTANCE_ID,0x14,CANIF_E_PARAM_HOH);
#endif
                }
            }
        }
        else
        {
            /*[SWS_CANIF_00419] d If parameter PduInfoPtr or Mailbox of CanIf_RxIndication()
             has an invalid value, CanIf shall report development error code CANIF_E_PARAM_POINTER
             to the Det_ReportError service of the DET module, when CanIf_RxIndication()
             is called. c(SRS_BSW_00323)*/
#if (CanIfPublicDevErrorDetect == true)
            Det_ReportError(MODULE_ID,INSTANCE_ID,0x14,CANIF_E_PARAM_POINTER);
#endif
        }
    }
}

/*********************************************************************************************************************************
 Service name:                                         CanIf_ControllerBusOff
 Service ID[hex]:                                               0x14
 Sync/Async:                                                 Synchronous
 Reentrancy:                                                 Reentrant
 Parameters (in):                                            ControllerId      -->Abstracted CanIf ControllerId which is assigned to a
 CAN controller, which is requested for mode transition.
 Parameters (inout):                                          None
 Parameters (out):                                             None
 Return value:                                                   None
 Description:
 This service indicates a Controller BusOff event referring to the corresponding
 CAN Controller with the abstract CanIf ControllerId.
 *******************************************************************************************************************************/

void CanIf_ControllerBusOff(uint8 ControllerId)
{
    CanIf_ControllerModeType ControllerModeStateLocal;
    /*
     [SWS_CANIF_00432] Caveats of CanIf_ControllerBusOff():
     The call context is either on interrupt level (interrupt mode) or on task level (polling
     mode).
     The CanIf must be initialized after Power ON.
     */

    /* [SWS_CANIF_00431] If CanIf was not initialized before calling CanIf_ControllerBusOff(),
     CanIf shall not execute BusOff notification, when CanIf_ControllerBusOff(), is called. */

    if (CanIfState == CANIF_READY)
    {
        /*[SWS_CANIF_00429]  If parameter ControllerId of CanIf_ControllerBusOff()
         has an invalid value, CanIf shall report development error code CANIF_E_PARAM_CONTROLLER
         to the Det_ReportError service of the DET module, when CanIf_ControllerBusOff()
         is called. (SRS_BSW_00323)*/

        if (ControllerId >= NUMBER_OF_CONTROLLERS)
        {
#if(CanIfPublicDevErrorDetect == true)
            Det_ReportError(MODULE_ID, INSTANCE_ID, 0x14, CANIF_E_PARAM_CONTROLLER);
#endif
        }
        else
        {

            /* [SWS_CANIF_00433]  Configuration of CanIf_ControllerBusOff(): ID of the
             CAN Controller is published inside the configuration description of the CanIf (see ECUC_CanIf_ */
            /*
             [SWS_CANIF_00739] If CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT (see ECUC_CanIf_00
             is enabled, CanIf shall clear the information about a TxConfirmation (see [SWS_CANIF_00740])
             when callback CanIf_ControllerBusOff(ControllerId) is called. c()
             */

#if CanIfPublicTxConfirmPollingSupport
            CanIf_ClearTxConfirmationInfoBuffer();
#endif

            /*
             [SWS_CANIF_00298] If a CCMSM is in state CANIF_CS_INIT when CanIf_ControllerBusOff(
             is called with parameter ControllerId referencing that CCMSM, then the CCMSM shall
             be changed to CANIF_CS_STOPPED.
             */
            /*
             [SWS_CANIF_00488] If a CCMSM is in state CANIF_CS_STARTED when CanIf_ControllerBusO
             is called with parameter ControllerId referencing that CCMSM, then the CCMSM shall
             be changed to CANIF_CS_STOPPED.
             */
            if (CanIf_GetControllerMode(ControllerId,
                                        &ControllerModeStateLocal) == E_OK)
            {
                if (ControllerModeStateLocal == CANIF_CS_STARTED
                        || ControllerModeStateLocal == CANIF_CS_UNINIT)
                {
                    CanIf_SetControllerMode(ControllerId, CANIF_CS_STOPPED);
                }

                /*
                 [SWS_CANIF_00724] When callback CanIf_ControllerBusOff( ControllerId)
                 is called, the CanIf shall call CanSM_ControllerBusOff(ControllerId) of the
                 CanSm (see subsubsection 8.6.3.9 or a CDD (see [SWS_CANIF_00559], [SWS_CANIF_00560]).
                 */

                CanSM_ControllerBusOff(ControllerId);
            }
            else
            {
                //nothing
            }
        }
    }
    else
    {
        //nothing
    }

    //

}

/*********************************************************************************************************************************
 Service name:                                         CanIf_ControllerModeIndication
 Service ID[hex]:                                               0x17
 Sync/Async:                                                 Synchronous
 Reentrancy:                                                 Reentrant
 Parameters (in):                                            ControllerId      -->Abstracted CanIf ControllerId which is assigned to a
 CAN controller, which is requested for mode transition.
 ControllerMode  -->Mode to which the CAN controller transitioned
 Parameters (inout):                                          None
 Parameters (out):                                             None
 Return value:                                                   None
 Description:
 This service indicates a controller state transition referring to the corresponding
 CAN controller with the abstract CanIf ControllerId.
 *******************************************************************************************************************************/

void CanIf_ControllerModeIndication(uint8 ControllerId,
                                    CanIf_ControllerModeType ControllerMode)
{
    /*[SWS_CANIF_00702] d If CanIf was not initialized before calling CanIf_ControllerModeIndication(),
     CanIf shall not execute state transition notification, when CanIf_ControllerModeIndication()
     is called.()*/
    if (CanIfState == CANIF_UNINIT)
    {
#if (CanIfPublicDevErrorDetect == true)
        Det_ReportError(MODULE_ID, INSTANCE_ID, 0x17, CANIF_E_UNINIT);
#endif
    }
    else
    {
        /*[SWS_CANIF_00700] d If parameter ControllerId of CanIf_ControllerModeIndication()
         has an invalid value, CanIf shall report development error code CANIF_E_PARAM_CONTROLLER
         to the Det_ReportError service of the DET module, when CanIf_ControllerModeIndication()
         is called. */
        if (ControllerId >= NUMBER_OF_CONTROLLERS)
        {
#if (CanIfPublicDevErrorDetect == true)
            Det_ReportError(MODULE_ID, INSTANCE_ID, 0x17, CANIF_E_PARAM_CONTROLLERID);
#endif
        }
        else
        {
            /* Note: The CanIf calls this callback service. It is implemented by the configured upper
             layer module. It is called in case of a state transition notification via CanIf_ControllerModeIndic
             of the CanDrv. The delivered parameter ControllerId of the service CanIf_ControllerModeIn
             is passed to the upper layer module. The delivered parameter ControllerMode of
             the service CanIf_ControllerModeIndication() is mapped to the appropriate
             parameter ControllerMode of <User_ControllerModeIndication>(). */
            if (ControllerMode != CANIF_CS_SLEEP
                    && ControllerMode != CANIF_CS_STARTED
                    && ControllerMode != CANIF_CS_STOPPED)
            {
#if (CanIfPublicDevErrorDetect == true)
                Det_ReportError(MODULE_ID, INSTANCE_ID, 0x17, CANIF_E_PARAM_CTRLMODE);
#endif
            }
            else
            {
                switch(ControllerMode)
                {
                    case CANIF_CS_STARTED:
                        CanIfControllerMode[ControllerId] = CANIF_CS_STARTED;
                        break;
                    case CANIF_CS_STOPPED:
                        CanIfControllerMode[ControllerId] = CANIF_CS_STOPPED;
                        break;
                    case CANIF_CS_SLEEP:
                        CanIfControllerMode[ControllerId] = CANIF_CS_SLEEP;
                        break;
                    default:
                        break;
                }
                /*CanSM_ControllerModeIndication(ControllerId, ControllerMode);*/
            }
        }
    }
}



