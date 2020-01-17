/********************************************************************************************************************************
 **
 FileName:                   Com.c
 AUTOSAR Version:            4.2.2
 **
 ********************************************************************************************************************************/
/********************************************************************************************************************************
 **                                                          Includes                                                                                                **
 ********************************************************************************************************************************/
#include "PduR.h"
#include "Com.h"
#include "ComTeam_Types.h"
#include "Det.h"
#include "Std_Types.h"

//#include "CanSM.h"
/********************************************************************************************************************************
 **                                                       Global Variables                                                                                       **
 ********************************************************************************************************************************/
extern Can_ConfigType Can;
extern CanIf_ConfigType CanIf;
extern PduR_PBConfigType PduR;
extern Com_Type Com;

static ComState_Type ComState = COM_UNINIT;

ComTeamConfig_Type ComTeamConfig;

/********************************************************************************************************************************
 **                                                        Local Functions                                                                                        **
 *********************************************************************************************************************************/
static boolean Com_GroupSignalDataCmp(uint8 ComIPduIndex,uint8 ComGroupSignalIndex);

boolean Com_GroupSignalDataCmp(uint8 ComIPduIndex,uint8 ComGroupSignalIndex)
{
    uint8 BitIndex, ComGroupSignalBufferBit, ComIPduBufferBit;
    Com_GroupSignalType* ComGroupSignalLocal = &Com.ComConfig.ComGroupSignal[ComGroupSignalIndex];
    Com_IPduType* ComIPduLocal = &Com.ComConfig.ComIPdu[ComIPduIndex];

    for(BitIndex = 0; BitIndex < ComGroupSignalLocal->ComBitSize; BitIndex++)
    {
        /*Get the bits from GroupSignalBuffer and IPduBuffer*/
        ComGroupSignalBufferBit = (ComGroupSignalLocal->ComBufferRef[BitIndex / 8] >> (BitIndex % 8)) & 1;
        ComIPduBufferBit = (ComIPduLocal->ComBufferRef[(ComGroupSignalLocal->ComBitPosition + BitIndex) / 8] >> ((ComGroupSignalLocal->ComBitPosition + BitIndex) % 8)) & 1;
        if(ComGroupSignalBufferBit != ComIPduBufferBit)
        {
            return true;
        }
        else
        {

        }
    }
    return false;
}
/********************************************************************************************************************************
 **                                                        Local Functions                                                                                        **
 *********************************************************************************************************************************/
/*********************************************************************************************************************************
 Service name:                  Com_Init
 Service ID:                      0x01
 Parameters (in):           config--> Pointer to the AUTOSAR COM module's configuration data
 Parameters (inout):              None
 Parameters (out):                None
 Return value:                    None
 Description:                  This service initializes internal and external interfaces and variables of the AUTOSAR COM module
                               layer for the further processing.After calling this function the inter-ECU communication is still
                               disabled.
*******************************************************************************************************************************/
void Com_Init(const Com_ConfigType* config)
{
    /*Local Variables For indexing Used Mainly In Loops*/
    uint8 ComInitIPduIndex;
    uint8 ComInitByteIndex;
    uint8 ComInitGroupSignalIndex;
    uint8 ComInitSignalGroupIndex;
    uint8 ComInitSignalIndex;
    uint8 ComInitBitIndex;

    Com_SignalType *ComSignalLocal;
    Com_IPduType *ComIPduLocal;
    Com_GroupSignalType* ComGroupSignalLocal;
    Com_SignalGroupType* ComSignalGroupLocal;

    /*Initializing Signals Buffer By Init Values Of The Signal*/
    for (ComInitSignalIndex = 0; ComInitSignalIndex < ComMaxSignalCnt ;
            ComInitSignalIndex++)
    {
        for (ComInitByteIndex = 0;
                ComInitByteIndex
                        < Com.ComConfig.ComSignal[ComInitSignalIndex].ComSignalLength;
                ComInitByteIndex++)
        {
            Com.ComConfig.ComSignal[ComInitSignalIndex].ComBufferRef[ComInitByteIndex] =
                    Com.ComConfig.ComSignal[ComInitSignalIndex].ComSignalInitValue;
        }
    }

    /*Initializing Group Signals Buffer By Init Values Of The Group Signal*/

    for (ComInitGroupSignalIndex = 0;
            ComInitGroupSignalIndex < ComMaxGroupSignalCnt ;
            ComInitGroupSignalIndex++)
    {
        for (ComInitByteIndex = 0;
                ComInitByteIndex
                        < Com.ComConfig.ComGroupSignal[ComInitGroupSignalIndex].ComSignalLength;
                ComInitByteIndex++)
        {
            Com.ComConfig.ComGroupSignal[ComInitGroupSignalIndex].ComBufferRef[ComInitByteIndex] =
                    Com.ComConfig.ComGroupSignal[ComInitGroupSignalIndex].ComSignalInitValue;
        }
    }

    /* Initializing IPDUs And Copying Signals And Group Signals To IPDUs  */
    for (ComInitIPduIndex = 0; ComInitIPduIndex < ComMaxIPduCnt ;
            ComInitIPduIndex++)
    {

        /* Get Pdu */
        ComIPduLocal = &Com.ComConfig.ComIPdu[ComInitIPduIndex];

        ComTeamConfig.ComTeamIPdu[ComInitIPduIndex].ComTeamTxMode.ComTeamMinimumDelayTimer = Com.ComConfig.ComIPdu[ComInitIPduIndex].ComTxIPdu.ComMinimumDelayTime;
        ComTeamConfig.ComTeamIPdu[ComInitIPduIndex].ComTeamTxMode.ComTeamTxModeTimePeriod = Com.ComConfig.ComIPdu[ComInitIPduIndex].ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeTimePeriod;
        /* Initialize Signals and GroupSignals Update and Confirmation State*/
        for (ComInitSignalIndex = 0; ComInitSignalIndex < ComMaxSignalCnt ;
                ComInitSignalIndex++)
        {
            ComTeamConfig.ComTeamSignal[ComInitSignalIndex].ComTeamSignalUpdated =
                    false;
            ComTeamConfig.ComTeamSignal[ComInitSignalIndex].ComTeamSignalConfirmed =
                    false;
        }
        for (ComInitSignalGroupIndex = 0;
                ComInitSignalGroupIndex < ComMaxSignalGroupCnt ;
                ComInitSignalGroupIndex++)
        {
            ComTeamConfig.ComTeamSignalGroup[ComInitSignalGroupIndex].ComTeamSignalGroupUpdated =
                    false;
            ComTeamConfig.ComTeamSignalGroup[ComInitSignalGroupIndex].ComTeamSignalGroupUpdated =
                    false;
        }

        /*Fill IPDUs With Default Unused Area */
        for (ComInitByteIndex = 0;
                ComInitByteIndex
                        < Com.ComConfig.ComIPdu[ComInitIPduIndex].ComIPduLength;
                ComInitByteIndex++)
        {
            Com.ComConfig.ComIPdu[ComInitIPduIndex].ComBufferRef[ComInitByteIndex] =
                    COM_TX_IPDU_UNUSED_AREAS_DEFAULT;
        }

        /*Copy Signals Referenced In IPDUs to IPDUs Buffers */
        for (ComInitSignalIndex = 0;
                Com.ComConfig.ComIPdu[ComInitIPduIndex].ComIPduSignalRef[ComInitSignalIndex]
                        != NULL; ComInitSignalIndex++)
        {
            /*Get Signal*/
            ComSignalLocal =
                    Com.ComConfig.ComIPdu[ComInitIPduIndex].ComIPduSignalRef[ComInitSignalIndex];

            /* Write data from signal buffer to IPdu*/
            for (ComInitBitIndex = 0;
                    ComInitBitIndex < ComSignalLocal->ComBitSize;
                    ComInitBitIndex++)
            {
                if ((ComSignalLocal->ComBufferRef[ComInitBitIndex / 8]
                        >> (ComInitBitIndex % 8)) & 1)
                {
                    ComIPduLocal->ComBufferRef[(ComInitBitIndex
                            + ComSignalLocal->ComBitPosition) / 8] |= 1
                            << ((ComInitBitIndex
                                    + ComSignalLocal->ComBitPosition) % 8);
                }
                else
                {
                    ComIPduLocal->ComBufferRef[(ComInitBitIndex
                            + ComSignalLocal->ComBitPosition) / 8] &= ~(1
                            << ((ComInitBitIndex
                                    + ComSignalLocal->ComBitPosition) % 8));
                }
            }
            /*Clear update bit*/
            ComIPduLocal->ComBufferRef[ComSignalLocal->ComUpdateBitPosition / 8] &=
                    ~(1 << (ComSignalLocal->ComUpdateBitPosition % 8));
        }

        /*Copy Group Signals Referenced In SignalGroups Referenced In IPDUs to IPDUs Buffers */

        for (ComInitSignalGroupIndex = 0;
                Com.ComConfig.ComIPdu[ComInitIPduIndex].ComIPduSignalGroupRef[ComInitSignalGroupIndex]
                        != NULL; ComInitSignalGroupIndex++)
        {
            /* Get SignalGroup */
            ComSignalGroupLocal =
                    Com.ComConfig.ComIPdu[ComInitIPduIndex].ComIPduSignalGroupRef[ComInitSignalGroupIndex];
            for (ComInitGroupSignalIndex = 0;
                    Com.ComConfig.ComSignalGroup[ComInitSignalGroupIndex].ComGroupSignalRef[ComInitGroupSignalIndex]
                            != NULL; ComInitGroupSignalIndex++)
            {
                /*Get Group Signal*/
                ComGroupSignalLocal =
                        Com.ComConfig.ComSignalGroup[ComInitSignalGroupIndex].ComGroupSignalRef[ComInitGroupSignalIndex];

                /* Write data from signal buffer to IPdu*/
                for (ComInitBitIndex = 0;
                        ComInitBitIndex < ComGroupSignalLocal->ComBitSize;
                        ComInitBitIndex++)
                {
                    if ((ComGroupSignalLocal->ComBufferRef[ComInitBitIndex / 8]
                            >> (ComInitBitIndex % 8)) & 1)
                    {
                        ComIPduLocal->ComBufferRef[(ComInitBitIndex
                                + ComGroupSignalLocal->ComBitPosition) / 8] |= 1
                                << ((ComInitBitIndex
                                        + ComGroupSignalLocal->ComBitPosition)
                                        % 8);
                    }
                    else
                    {
                        ComIPduLocal->ComBufferRef[(ComInitBitIndex
                                + ComGroupSignalLocal->ComBitPosition) / 8] &=
                                ~(1
                                        << ((ComInitBitIndex
                                                + ComGroupSignalLocal->ComBitPosition)
                                                % 8));
                    }
                }
            }
            /*Clear update bit*/
            ComIPduLocal->ComBufferRef[ComSignalGroupLocal->ComUpdateBitPosition
                    / 8] &= ~(1
                    << (ComSignalGroupLocal->ComUpdateBitPosition % 8));
        }

    }
    ComState = COM_READY;

    return;
}




/*********************************************************************************************************************************
 Service name:               Com_SendSignal
 Service ID:                    0x0a
 Parameters (in):           SignalId--> Id of signal to be sent.
                            SignalDataPtr --> Reference to the signal data to be transmitted.
 Parameters (inout):            None
 Parameters (out):              None
 Return value:              uint8 --->
                                 E_OK: service has been accepted
                                 COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group
                                     was stopped (or service failed due to development error)
                                 COM_BUSY: in case the TP-Buffer is locked for large data types
                                     handling
 Description:        The service Com_SendSignal updates the signal object identified by SignalId with the signal
                     referenced by the SignalDataPtr parameter
 *******************************************************************************************************************************/
uint8 Com_SendSignal(Com_SignalIdType SignalId,const void* SignalDataPtr)
{
    uint8 ComIPduIndex, ComSignalIndex, BitIndex;
    boolean ComCopySignal = false;
    /*
     [SWS_Com_00804] ⌈Error code if any other API service, except Com_GetStatus, is called before the AUTOSAR COM module was initialized with Com_Init
     or after a call to Com_Deinit:
     error code: COM_E_UNINIT
     value [hex]: 0x02
     (SRS_BSW_00337)
   */
    if(ComState==COM_UNINIT)
    {
        #if ComConfigurationUseDet == true
            Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, 0x0A, COM_E_UNINIT);
        #endif
        return COM_SERVICE_NOT_AVAILABLE;
    }

    /*
      [SWS_Com_00803] ⌈API service called with wrong parameter:
      error code: COM_E_PARAM
      value [hex]: 0x01
      (SRS_BSW_00337)
   */
    else if(SignalId >= ComMaxSignalCnt)
    {
        #if ComConfigurationUseDet == true
            Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, 0x0A, COM_E_PARAM);
        #endif
        return COM_SERVICE_NOT_AVAILABLE;
    }

    /*
      [SWS_Com_00805] ⌈NULL pointer checking:
      error code: COM_E_PARAM_POINTER
      value [hex]: 0x03
      (SRS_BSW_00414)
    */
    else if(SignalDataPtr == NULL)
    {
        #if ComConfigurationUseDet == true
            Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, 0x0A, COM_E_PARAM_POINTER);
        #endif
        return COM_SERVICE_NOT_AVAILABLE;
    }

    else
    {
        Com_SignalType* ComSignalLocal;
        Com_IPduType* ComIPduLocal;
        ComTeamIPdu_Type* ComTeamIPduLocal;
        /*
         * [SWS_Com_00625] ⌈If the updated signal has the ComTransferProperty TRIG-GERED and it is assigned to an I-PDU with ComTxModeMode DIRECT or MIXED,
           then Com_SendSignal shall perform an immediate transmission (within the next main function at the latest)
           of that I-PDU, unless the sending is delayed or prevented by other COM mechanisms.⌋ (SRS_Com_02037)
         */

        for (ComIPduIndex=0; ComIPduIndex < ComMaxIPduCnt; ComIPduIndex++)
        {
            for (ComSignalIndex=0; Com.ComConfig.ComIPdu[ComIPduIndex].ComIPduSignalRef[ComSignalIndex] != NULL; ComSignalIndex++)
            {
                if (Com.ComConfig.ComIPdu[ComIPduIndex].ComIPduSignalRef[ComSignalIndex]->ComHandleId == SignalId)
                {
                    ComIPduLocal = &Com.ComConfig.ComIPdu[ComIPduIndex];
                    ComSignalLocal = Com.ComConfig.ComIPdu[ComIPduIndex].ComIPduSignalRef[ComSignalIndex];
                    ComTeamIPduLocal = &ComTeamConfig.ComTeamIPdu[ComIPduIndex];

                    ComTeamIPduLocal->ComTeamTxMode.ComTeamTxModeRepetitionPeriod = 0;

                    switch (ComSignalLocal->ComTransferProperty)
                    {
                         /*[SWS_Com_00330] At any send request of a signal with ComTransferProperty TRIGGERED assigned to an I-PDU with ComTxModeMode DIRECT or MIXED, the AUTOSAR COM module shall immediately
                            (within the next main function at the lat-est) initiate ComTxModeNumberOfRepetitions plus one transmissions
                            of the as-signed I-PDU.
                            (SRS_Com_02083)
                          */

                        case TRIGGERED:
                            ComTeamIPduLocal->ComTeamTxMode.ComTeamTxIPduNumberOfRepetitions = ComIPduLocal->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeNumberOfRepetitions+1;
                            ComCopySignal = true;
                            break;


                         /*
                          * [SWS_Com_00734] ⌈At a send request of a signal with ComTransferProperty TRIGGERED_ON_CHANGE assigned to an I-PDU with ComTxModeMode
                             DIRECT or MIXED, the AUTOSAR COM module shall immediately (within the next main func-tion at the latest)
                             initiate ComTxModeNumberOfRepetitions plus one transmissions of the assigned I-PDU,
                             if the new sent signal differs to the locally stored
                             (last sent or init) in length or value.
                             (SRS_Com_02083)
                          */

                        case TRIGGERED_ON_CHANGE:
                            if(memcmp(ComSignalLocal->ComBufferRef, SignalDataPtr, ComSignalLocal->ComSignalLength))
                            {
                                ComTeamIPduLocal->ComTeamTxMode.ComTeamTxIPduNumberOfRepetitions = ComIPduLocal->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeNumberOfRepetitions + 1;
                                ComCopySignal = true;
                            }
                            else
                            {

                            }
                            break;
                        /*
                         * SWS_Com_00768] ⌈At a send request of a signal with ComTransferProperty TRIG-GERED_ON_CHANGE_WITHOUT_REPETITION assigned to an I-PDU with ComTxModeMode DIRECT or MIXED,
                           the AUTOSAR COM module shall immediate-ly (within the next main function at the latest) initiate one transmission of the as-signed I-PDU,
                           if the new sent signal differs to the locally stored (last sent or init) in length or value.
                           (SRS_Com_02083)
                         */
                        case TRIGGERED_ON_CHANGE_WITHOUT_REPETITION:
                            if(memcmp(ComSignalLocal->ComBufferRef, SignalDataPtr, ComSignalLocal->ComSignalLength))
                            {
                                ComTeamIPduLocal->ComTeamTxMode.ComTeamTxIPduNumberOfRepetitions = 1;
                                ComCopySignal = true;
                            }
                            else
                            {

                            }
                            break;

                        /*
                         * SWS_Com_00767] ⌈At any send request of a signal with ComTransferProperty TRIGGERED_WITHOUT_REPETITION assigned to an I-PDU with ComTx-ModeMode DIRECT or MIXED,
                           the AUTOSAR COM module shall immediately (with-in the next main function at the latest) initiate one transmission of the assigned I-PDU.
                           (SRS_Com_02083)
                         */

                        case TRIGGERED_WITHOUT_REPETITION:
                            ComTeamIPduLocal->ComTeamTxMode.ComTeamTxIPduNumberOfRepetitions = 1;
                            ComCopySignal = true;
                            break;

                        default:
                            return COM_SERVICE_NOT_AVAILABLE;
                    }

                    if(ComCopySignal)
                    {
                        /*Copy signal to signal buffer*/
                        memcpy(ComSignalLocal->ComBufferRef, SignalDataPtr, ComSignalLocal->ComSignalLength);

                        /* Write data from signal buffer to IPdu*/
                        for(BitIndex = 0; BitIndex < ComSignalLocal->ComBitSize; BitIndex++)
                        {
                           if((ComSignalLocal->ComBufferRef[BitIndex / 8] >> (BitIndex % 8)) & 1)
                           {
                               ComIPduLocal->ComBufferRef[(BitIndex + ComSignalLocal->ComBitPosition) / 8] |= 1 << ((BitIndex + ComSignalLocal->ComBitPosition)%8);
                           }
                           else
                           {
                               ComIPduLocal->ComBufferRef[(BitIndex + ComSignalLocal->ComBitPosition) / 8] &= ~(1 << ((BitIndex + ComSignalLocal->ComBitPosition)%8));
                           }
                        }
                        /*Set update bit*/
                        ComIPduLocal->ComBufferRef[ComSignalLocal->ComUpdateBitPosition / 8] |= 1 << (ComSignalLocal->ComUpdateBitPosition%8);

                        ComTeamConfig.ComTeamSignal[ComSignalLocal->ComHandleId].ComTeamSignalUpdated = true;
                    }
                    else
                    {

                    }

                    return E_OK;
                }
                else
                {

                }
            }
        }
    }

    return COM_SERVICE_NOT_AVAILABLE;
}


/*********************************************************************************************************************************
 Service name:               Com_ReceiveSignal
 Service ID:                    0x0b
 Parameters (in):           SignalId--> Id of signal to be sent.
 Parameters (inout):            None
 Parameters (out):          SignalDataPtr --> Reference to the location where the received signal data shall be
                                              stored
 Return value:              uint8 --->
                                 E_OK: service has been accepted
                                 COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group
                                     was stopped (or service failed due to development error)
                                 COM_BUSY: in case the TP-Buffer is locked for large data types
                                     handling
 Description:        Com_ReceiveSignal copies the data of the signal identified by SignalId to the
                     location specified by SignalDataPtr.
 *******************************************************************************************************************************/
uint8 Com_ReceiveSignal(Com_SignalIdType SignalId,void* SignalDataPtr)
{
    Com_SignalType *ComSignalLocal;
    Com_IPduType *ComIPduLocal;
    uint8 ComIPduIndex, ComSignalIndex, BitIndex;

    if(ComState == COM_UNINIT)
    {
        /*[SWS_Com_00804] ⌈Error code if any other API service, except Com_GetStatus, is called before the AUTOSAR COM module was initialized with Com_Init
        or after a call to Com_Deinit:
        error code: COM_E_UNINIT
        value [hex]: 0x02
        (SRS_BSW_00337)
        */
        #if ComConfigurationUseDet == true
            Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, 0x0B, COM_E_UNINIT);
        #endif
  
    }
    else
    {
        if(SignalDataPtr==NULL)
        {
            /*
              [SWS_Com_00805] ⌈NULL pointer checking:
              error code: COM_E_PARAM_POINTER
              value [hex]: 0x03
              (SRS_BSW_00414)
            */
            #if ComConfigurationUseDet == true
                Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, 0x0B, COM_E_PARAM_POINTER);
            #endif
        }
        else
        {
            if(SignalId >= ComMaxSignalCnt)
            {
                /*
                [SWS_Com_00803] ⌈API service called with wrong parameter:
                error code: COM_E_PARAM
                value [hex]: 0x01
                (SRS_BSW_00337)
                */
                #if ComConfigurationUseDet == true
                    Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, 0x0B, COM_E_PARAM);
                #endif
            }
            else
            {
                /* Find the IPdu which contains this signal */
                for(ComIPduIndex = 0; ComIPduIndex < ComMaxIPduCnt; ComIPduIndex++)
                {
                    for(ComSignalIndex = 0; Com.ComConfig.ComIPdu[ComIPduIndex].ComIPduSignalRef[ComSignalIndex] != NULL; ComSignalIndex++)
                    {
                        if(Com.ComConfig.ComIPdu[ComIPduIndex].ComIPduSignalRef[ComSignalIndex]->ComHandleId == SignalId)
                        {
                            /* Get Pdu */
                            ComIPduLocal = &Com.ComConfig.ComIPdu[ComIPduIndex];
                            /*Get Signal*/
                            ComSignalLocal = Com.ComConfig.ComIPdu[ComIPduIndex].ComIPduSignalRef[ComSignalIndex];

                            /* Write data from IPdu buffer to Signal buffer*/
                            for(BitIndex = ComSignalLocal->ComBitPosition; BitIndex < ComSignalLocal->ComBitPosition + ComSignalLocal->ComBitSize; BitIndex++)
                            {
                               if((ComIPduLocal->ComBufferRef[BitIndex / 8] >> (BitIndex % 8)) & 1)
                               {
                                   ComSignalLocal->ComBufferRef[(BitIndex - ComSignalLocal->ComBitPosition) / 8] |= 1 << ((BitIndex - ComSignalLocal->ComBitPosition) % 8);
                               }
                               else
                               {
                                   ComSignalLocal->ComBufferRef[(BitIndex - ComSignalLocal->ComBitPosition) / 8] &= ~(1 << ((BitIndex - ComSignalLocal->ComBitPosition) % 8));
                               }
                            }
                        }
                        else
                        {

                        }
                    }

                    SignalDataPtr = ComSignalLocal->ComBufferRef;
                }
                return E_OK;
            }
        }
    }

    return COM_SERVICE_NOT_AVAILABLE;
}



/*********************************************************************************************************************************
 Service name:               Com_UpdateShadowSignal
 Service ID:                    0x0c
 Parameters (in):           SignalId--> Id of group signal to be updated.
                            SignalDataPtr --> Reference to the group signal data to be updated.
 Parameters (inout):            None
 Parameters (out):              None
 Return value:                  None
 Description:        The service Com_UpdateShadowSignal updates a group signal with the data referenced by SignalDataPtr.
 *******************************************************************************************************************************/
void Com_UpdateShadowSignal(Com_SignalIdType SignalId,const void* SignalDataPtr)
{
    uint8 ComGroupSignalIndex;

    /* Check that the group signal ID is a valid ID*/
    if(SignalId < ComMaxGroupSignalCnt)
    {
        /*Find GroupSignal with such ID*/
        for(ComGroupSignalIndex = 0; ComGroupSignalIndex < ComMaxGroupSignalCnt; ComGroupSignalIndex++)
        {
            if(Com.ComConfig.ComGroupSignal[ComGroupSignalIndex].ComHandleId == SignalId)
            {
                /* Copy group signal to group signal buffer */
                memcpy(Com.ComConfig.ComGroupSignal[SignalId].ComBufferRef, SignalDataPtr, Com.ComConfig.ComGroupSignal[SignalId].ComSignalLength);
                return;
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


/*********************************************************************************************************************************
 Service name:               Com_ReceiveShadowSignal
 Service ID:                    0x0f
 Parameters (in):           SignalId--> Id of group signal to be received.
                            SignalDataPtr --> Reference to the group signal data in which to store the received data.
 Parameters (inout):            None
 Parameters (out):              None
 Return value:                  None
 Description:        The service Com_ReceiveShadowSignal updates the group signal which is referenced by SignalDataPtr with the data in the shadow buffer.
 *******************************************************************************************************************************/
void Com_ReceiveShadowSignal(Com_SignalIdType SignalId, void* SignalDataPtr)
{
    uint8 ComGroupSignalIndex;

    /* Check that the group signal ID is a valid ID*/
    if(SignalId < ComMaxGroupSignalCnt)
    {
        /*Find GroupSignal with such ID*/
        for(ComGroupSignalIndex = 0; ComGroupSignalIndex < ComMaxGroupSignalCnt; ComGroupSignalIndex++)
        {
            if(Com.ComConfig.ComGroupSignal[ComGroupSignalIndex].ComHandleId == SignalId)
            {
                SignalDataPtr = Com.ComConfig.ComGroupSignal[ComGroupSignalIndex].ComBufferRef;
                return;
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





/*********************************************************************************************************************************
 Service name:               Com_RxIndication
 Service ID:                    0x42
 Parameters (in):           RxPduId--> ID of the received I-PDU.
                            PduInfoPtr --> Contains the length (SduLength) of the received I-PDU and a pointer to
                                           a buffer (SduDataPtr) containing the I-PDU.
 Parameters (inout):            None
 Parameters (out):              None
 Return value:                  None
 Description:        Indication of a received I-PDU from a lower layer communication interface module.
*******************************************************************************************************************************/
void Com_RxIndication(PduIdType RxPduId,const PduInfoType* PduInfoPtr)
{
    uint8 ComSignalIndex,
    ComSignalGroupIndex,
    ComUpdateBitPositionLocal;

    if(ComState == COM_READY)
    {
        if(RxPduId < ComMaxIPduCnt)
        {
            if(PduInfoPtr != NULL)
            {
                if (Com.ComConfig.ComIPdu[RxPduId].ComIPduDirection == Receive)
                {
                    /*[SWS_Com_00574] ?When unpacking an I-PDU,
                      the AUTOSAR COM module shall check the received data length (PduInfoPtr->SduLength) and unpack
                      and notify only completely received signals via ComNotification. (SRS_Com_02046)*/

                    if(PduInfoPtr -> SduLength <= 8)
                    {
                        memcpy(Com.ComConfig.ComIPdu[RxPduId].ComBufferRef , PduInfoPtr -> SduDataPtr , PduInfoPtr -> SduLength);
                    }
                    else
                    {
                        /*Misra*/
                    }

                    if(Com.ComConfig.ComIPdu[RxPduId].ComIPduSignalProcessing == IMMEDIATE)
                    {
                        /*[SWS_Com_00300] If ComIPduSignalProcessing for an I-PDU is configured to IM-MEDIATE,
                          the AUTOSAR COM module shall invoke the configured ComNotifications for the included signals
                          and signal groups within the Com_RxIndication,or Com_TpRxIndication function respectively.(SRS_Com_02046)*/

                        /* Loop over all Signals in this IPDU */
                        for(ComSignalIndex = 0 ; Com.ComConfig.ComIPdu[RxPduId].ComIPduSignalRef[ComSignalIndex] != NULL ; ComSignalIndex++)
                        {
                            ComUpdateBitPositionLocal = Com.ComConfig.ComIPdu[RxPduId].ComIPduSignalRef[ComSignalIndex] -> ComUpdateBitPosition;

                            /* Check if UpdateBit is set*/
                            if((Com.ComConfig.ComIPdu[RxPduId].ComBufferRef[ComUpdateBitPositionLocal / 8]) & (1 << (ComUpdateBitPositionLocal % 8)))
                            {
                                /*invoke the configured ComNotifications for the included signals to RTE*/
                                Com.ComConfig.ComIPdu[RxPduId].ComIPduSignalRef[ComSignalIndex] -> ComNotification();
                                /* Clear UpdateBit */
                                Com.ComConfig.ComIPdu[RxPduId].ComBufferRef[ComUpdateBitPositionLocal  / 8] &= ~(1 << (ComUpdateBitPositionLocal % 8));
                            }
                            else
                            {
                                /*Misra*/
                            }
                        }

                        /* Loop over all Signal groups in the current IPDU */
                        for(ComSignalGroupIndex=0; Com.ComConfig.ComIPdu[RxPduId].ComIPduSignalGroupRef[ComSignalGroupIndex] != NULL; ComSignalGroupIndex++)
                        {
                            ComUpdateBitPositionLocal = Com.ComConfig.ComIPdu[RxPduId].ComIPduSignalGroupRef[ComSignalGroupIndex]->ComUpdateBitPosition;

                            /* Check if UpdateBit is set*/
                            if(Com.ComConfig.ComIPdu[RxPduId].ComBufferRef[ComUpdateBitPositionLocal / 8]  &  (1 << (ComUpdateBitPositionLocal % 8)))
                            {
                                /*invoke the configured ComNotifications for the included signal groups to RTE*/
                                Com.ComConfig.ComIPdu[RxPduId].ComIPduSignalGroupRef[ComSignalGroupIndex]->ComNotification();
                                /* Clear UpdateBit */
                                Com.ComConfig.ComIPdu[RxPduId].ComBufferRef[ComUpdateBitPositionLocal / 8] &= ~(1 << (ComUpdateBitPositionLocal % 8));
                            }
                            else
                            {
                                /*Misra*/
                            }

                        }
                    }
                    else
                    {
                        /*[SWS_Com_00301] ?If ComIPduSignalProcessing for an I-PDU is configured to DE-FERRED,
                          the AUTOSAR COM module shall first copy the I-PDU\92s data within the Com_RxIndication function
                          or the related TP reception functions respectively from the PduR into COM.
                          Then the AUTOSAR COM module shall invoke the configured ComNotifications for the included signals
                          and signal groups asynchronously during the next call to Com_MainFunctionRx.(SRS_Com_02046)*/
                    }
                }
                else
                {
                    /*Com.ComConfig.ComIPdu[RxPduId].ComIPduDirection != Receive*/
                }
            }
            else
            {
                /*PduInfoPtr == NULL*/
            }

        }
        else
        {
            /* RxPduId > PduIdMax */
        }
    }
    else
    {
        /*ComState != COM_READY*/
    }
}



/*********************************************************************************************************************************
 Service name:               Com_TxConfirmation
 Service ID:                    0x40
 Parameters (in):           TxPduId--> ID of the I-PDU that has been transmitted.
 Parameters (inout):            None
 Parameters (out):              None
 Return value:                  None
 Description:        The lower layer communication interface module confirms the transmission of an IPDU.
 *******************************************************************************************************************************/
void Com_TxConfirmation(PduIdType TxPduId)
{
    uint8 ComIPduIndex,
    ComSignalIndex,
    ComSignalGroupIndex;

    Com_IPduType *ComIPduLocal;
    Com_SignalType *ComSignalLocal;
    Com_SignalGroupType *ComSignalGroupLocal;
    if(TxPduId < ComMaxIPduCnt)
    {
        /* Loop over all IPdus*/
        for(ComIPduIndex = 0; ComIPduIndex < ComMaxIPduCnt ; ComIPduIndex++)
        {
            ComIPduLocal = &Com.ComConfig.ComIPdu[ComIPduIndex];

            /* Check if the IPdu has the desired ID */
            if(ComIPduLocal->ComIPduHandleId == TxPduId)
            {
                /* Check if the IPdu direction is send */
                if(ComIPduLocal->ComIPduDirection == Send)
                {
                    /* Loop over all signals which belong to this IPdu */
                    for (ComSignalIndex = 0; ComIPduLocal->ComIPduSignalRef[ComSignalIndex] != NULL; ComSignalIndex++)
                    {
                        ComSignalLocal = Com.ComConfig.ComIPdu[ComIPduIndex].ComIPduSignalRef[ComSignalIndex];

                        /* Check if the signal was updated */
                        if(ComTeamConfig.ComTeamSignal[ComSignalIndex].ComTeamSignalUpdated)
                        {
                            /* Clear that the signal was updated */
                            ComTeamConfig.ComTeamSignal[ComSignalIndex].ComTeamSignalUpdated = false;

                            if(ComIPduLocal->ComTxIPdu.ComTxIPduClearUpdateBit == Confirmation)
                            {
                                /* Clear update bit */
                                ComIPduLocal->ComBufferRef[ComSignalLocal->ComUpdateBitPosition / 8] &= ~(1<< (ComSignalLocal->ComUpdateBitPosition % 8));
                            }
                            else
                            {

                            }
                            if(ComIPduLocal->ComIPduSignalProcessing == IMMEDIATE)
                            {
                                /* Notify RTE */
                                if(ComSignalLocal->ComNotification != NULL)
                                {
                                    ComSignalLocal->ComNotification();
                                }
                                else
                                {

                                }
                            }
                            else if(ComIPduLocal->ComIPduSignalProcessing == DEFERRED)
                            {
                                /* Set Flag so Com_MainFunctionTx can check it at a later cyclic call */
                                ComTeamConfig.ComTeamSignal[ComSignalIndex].ComTeamSignalConfirmed = true;
                            }
                            else
                            {

                            }
                        }
                        else
                        {

                        }
                    }
                    /* Loop over all signalgroups which belong to this IPdu */
                    for (ComSignalGroupIndex = 0; ComIPduLocal->ComIPduSignalGroupRef[ComSignalGroupIndex] != NULL; ComSignalGroupIndex++)
                    {
                        ComSignalGroupLocal = Com.ComConfig.ComIPdu[ComIPduIndex].ComIPduSignalGroupRef[ComSignalGroupIndex];

                        /* Check if the signalgroup was updated */
                        if(ComTeamConfig.ComTeamSignalGroup[ComSignalGroupIndex].ComTeamSignalGroupUpdated)
                        {
                            ComTeamConfig.ComTeamSignalGroup[ComSignalGroupIndex].ComTeamSignalGroupUpdated = false;

                            if(ComIPduLocal->ComTxIPdu.ComTxIPduClearUpdateBit == Confirmation)
                            {
                                /* Clear update bit */
                                ComIPduLocal->ComBufferRef[ComSignalGroupLocal->ComUpdateBitPosition / 8] &= ~(1<< (ComSignalGroupLocal->ComUpdateBitPosition % 8));
                            }
                            else
                            {

                            }
                            if(ComIPduLocal->ComIPduSignalProcessing == IMMEDIATE)
                            {
                                /* Notify RTE */
                                if(ComSignalGroupLocal->ComNotification != NULL)
                                {
                                    ComSignalGroupLocal->ComNotification();
                                }
                                else
                                {

                                }
                            }
                            else if(ComIPduLocal->ComIPduSignalProcessing == DEFERRED)
                            {
                                /* Set Flag so Com_MainFunctionTx can check it at a later cyclic call */
                                ComTeamConfig.ComTeamSignalGroup[ComSignalGroupIndex].ComTeamSignalGroupConfirmed = true;
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

                break;
            }
        }
    }
    else
    {

    }

    return;
}


/*********************************************************************************************************************************
 Service name:               Com_MainFunctionTx
 Service ID:                    0x19
 Parameters (in):               None
 Parameters (inout):            None
 Parameters (out):              None
 Return value:                  None
 Description:        This function performs the processing of the AUTOSAR COM module's transmission activities
                     that are not directly handled within the COM's function invoked by the RTE,
                     for example Com_SendSignal
 *******************************************************************************************************************************/
void Com_MainFunctionTx(void)
{
    uint8 IPduIdIndex;
    uint8 SignalIdIndex;
    uint8 SignalGroupIdIndex;
    Com_IPduType *ComIPduLoc;
    ComTeamIPdu_Type *ComTeamIPduLoc;
    boolean MinimumDelayTimerLoc=true;

    if(ComState == COM_READY)
    {
        for ( IPduIdIndex = 0; IPduIdIndex < ComMaxIPduCnt; IPduIdIndex++)
        {
            ComIPduLoc =&Com.ComConfig.ComIPdu[IPduIdIndex];
            ComTeamIPduLoc = &ComTeamConfig.ComTeamIPdu[IPduIdIndex];

            /* check if  IPdu Direction should be transmitted */
            if (ComIPduLoc->ComIPduDirection == Send)
            {
                #if(ComEnableMDTForCyclicTransmission == true)

                    if(ComTeamIPduLoc->ComTeamTxMode.ComTeamMinimumDelayTimer> 0)
                    {
                        ComTeamIPduLoc->ComTeamTxMode.ComTeamMinimumDelayTimer -= ComIPduLoc->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeRepetitionPeriod;
                    }
                    else
                    {
                        /*misra*/
                    }

                    if(ComTeamIPduLoc->ComTeamTxMode.ComTeamMinimumDelayTimer<=0)
                    {
                        MinimumDelayTimerLoc=true;
                    }
                    else
                    {
                        MinimumDelayTimerLoc=false;
                    }
                #endif

                /* check If IPDU has periodic transmission mode*/
                if (ComIPduLoc->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeMode == PERIODIC)
                {
                    if(ComTeamIPduLoc->ComTeamTxMode.ComTeamTxModeTimePeriod> 0)
                    {
                        ComTeamIPduLoc->ComTeamTxMode.ComTeamTxModeTimePeriod -= ComIPduLoc->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeRepetitionPeriod;
                    }
                    else if((ComTeamIPduLoc->ComTeamTxMode.ComTeamTxModeTimePeriod<= 0)&&(MinimumDelayTimerLoc))
                    {
                        if(Com_TriggerIPDUSend(IPduIdIndex) == E_OK)
                        {
                            /*Reset periodic timer.*/
                            ComTeamIPduLoc->ComTeamTxMode.ComTeamTxModeTimePeriod= ComIPduLoc->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeTimePeriod;
                            #if(ComEnableMDTForCyclicTransmission == true)
                                ComTeamMinimumDelayTimerLoc=ComIPduLoc->ComTxIPdu.ComMinimumDelayTime;
                            #endif
                        }
                        else
                        {
                            /*misra*/
                        }
                    }
                    else
                    {
                        /*misra*/
                    }
                }
                /* check If IPDU has DIRECT transmission mode*/
                else if (ComIPduLoc->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeMode == DIRECT)
                {
                    if (ComTeamIPduLoc->ComTeamTxMode.ComTeamTxIPduNumberOfRepetitions > 0)
                    {
                        if(ComTeamIPduLoc->ComTeamTxMode.ComTeamTxModeRepetitionPeriod > 0)
                        {
                            ComTeamIPduLoc->ComTeamTxMode.ComTeamTxModeRepetitionPeriod -= ComIPduLoc->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeRepetitionPeriod;
                        }
                        else if((ComTeamIPduLoc->ComTeamTxMode.ComTeamTxModeRepetitionPeriod <= 0)&&(MinimumDelayTimerLoc))
                        {
                            if(Com_TriggerIPDUSend(IPduIdIndex) == E_OK)
                            {   /*Reset periodic timer.*/
                                UARTprintf("Com_TriggerIPDUSend\n");
                                ComTeamIPduLoc->ComTeamTxMode.ComTeamTxModeRepetitionPeriod = ComIPduLoc->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeTimePeriod;
                                ComTeamIPduLoc->ComTeamTxMode.ComTeamTxIPduNumberOfRepetitions--;
                                #if(ComEnableMDTForCyclicTransmission == true)
                                    ComTeamMinimumDelayTimerLoc=ComIPduLoc->ComTxIPdu.ComMinimumDelayTime;
                                #endif
                            }
                            else
                            {
                                /*misra*/
                            }
                        }
                        else
                        {
                            /*misra*/
                        }
                    }
                    else
                    {
                        /*misra*/
                    }
                }
                /* check If IPDU has MIXED transmission mode*/
                else if (ComIPduLoc->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeMode == MIXED)
                {
                    /*it time for a direct transmission*/
                    if (ComTeamIPduLoc->ComTeamTxMode.ComTeamTxIPduNumberOfRepetitions > 0)
                    {
                        if(ComTeamIPduLoc->ComTeamTxMode.ComTeamTxModeRepetitionPeriod > 0)
                        {
                        ComTeamIPduLoc->ComTeamTxMode.ComTeamTxModeRepetitionPeriod    -= ComTxTimeBase;
                        }
                        else if((ComTeamIPduLoc->ComTeamTxMode.ComTeamTxModeRepetitionPeriod <= 0)&&(MinimumDelayTimerLoc))
                        {
                            if(Com_TriggerIPDUSend(IPduIdIndex) == E_OK)
                            {   /*Reset periodic timer.*/
                                ComTeamIPduLoc->ComTeamTxMode.ComTeamTxModeRepetitionPeriod = ComIPduLoc->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeRepetitionPeriod;
                                ComTeamIPduLoc->ComTeamTxMode.ComTeamTxIPduNumberOfRepetitions--;
                                #if(ComEnableMDTForCyclicTransmission == true)
                                    ComTeamMinimumDelayTimerLoc=ComIPduLoc->ComTxIPdu.ComMinimumDelayTime;
                                #endif
                            }
                            else
                            {
                                /*misra*/
                            }
                        }
                        else
                        {
                            /*misra*/
                        }
                    } /*it time for a periodic transmission*/
                    else
                    {
                        if(ComTeamIPduLoc->ComTeamTxMode.ComTeamTxModeTimePeriod> 0)
                        {
                            ComTeamIPduLoc->ComTeamTxMode.ComTeamTxModeTimePeriod -= ComTxTimeBase;
                        }
                        else if((ComTeamIPduLoc->ComTeamTxMode.ComTeamTxModeTimePeriod<= 0)&&(MinimumDelayTimerLoc))
                        {
                            if(Com_TriggerIPDUSend(IPduIdIndex) == E_OK)
                            {   /*Reset periodic timer.*/
                                ComTeamIPduLoc->ComTeamTxMode.ComTeamTxModeTimePeriod= ComIPduLoc->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeTimePeriod;
                                #if(ComEnableMDTForCyclicTransmission == true)
                                    ComTeamMinimumDelayTimerLoc=ComIPduLoc->ComTxIPdu.ComMinimumDelayTime;
                                #endif
                            }
                            else
                            {
                                /*misra*/
                            }
                        }
                        else
                        {
                            /*misra*/
                        }

                    }
                }
                /* The IDPU has NONE transmission mode*/
                else
                {
                    /* Don't send!*/
                }
                if(Com.ComConfig.ComIPdu[IPduIdIndex].ComIPduSignalProcessing==DEFERRED)
                {
                    for ( SignalIdIndex = 0; Com.ComConfig.ComIPdu[IPduIdIndex]. ComIPduSignalRef[SignalIdIndex] !=NULL; SignalIdIndex++)
                    {
                        if(ComTeamConfig.ComTeamSignal[SignalIdIndex].ComTeamSignalConfirmed)
                        {
                            if(Com.ComConfig.ComIPdu[IPduIdIndex]. ComIPduSignalRef[SignalIdIndex]->ComNotification !=NULL)
                            {
                                Com.ComConfig.ComIPdu[IPduIdIndex]. ComIPduSignalRef[SignalIdIndex]->ComNotification();
                            }
                            else
                            {
                                /*misra*/
                            }
                            ComTeamConfig.ComTeamSignal[SignalIdIndex].ComTeamSignalConfirmed=false;
                        }
                        else
                        {
                             /*misra*/
                        }
                    }
                    for ( SignalGroupIdIndex = 0; Com.ComConfig.ComIPdu[IPduIdIndex]. ComIPduSignalGroupRef[SignalGroupIdIndex] !=NULL; SignalGroupIdIndex++)
                    {
                        if(ComTeamConfig.ComTeamSignalGroup[SignalGroupIdIndex].ComTeamSignalGroupConfirmed)
                        {
                            if(Com.ComConfig.ComIPdu[IPduIdIndex]. ComIPduSignalGroupRef[SignalGroupIdIndex]->ComNotification !=NULL)
                            {
                                Com.ComConfig.ComIPdu[IPduIdIndex]. ComIPduSignalGroupRef[SignalGroupIdIndex]->ComNotification();
                            }
                            else
                            {
                                /*misra*/
                            }
                            ComTeamConfig.ComTeamSignalGroup[SignalGroupIdIndex].ComTeamSignalGroupConfirmed=false;
                        }
                        else
                        {
                            /*misra*/
                        }
                    }
                }
                else
                {
                    /*misra*/
                }
            }
            else
            {
             /*IPdu Direction is not send*/
            }
        }
    }
    else
    {
        /*misra*/
    }

    return;
}


/*********** **********************************************************************************************************************
 Service name:               Com_MainFunctionRx
 Service ID:                    0x18
 Parameters (in):               None
 Parameters (inout):            None
 Parameters (out):              None
 Return value:                  None
 Description:        This function performs the processing of the AUTOSAR COM module's receive
                     processing that are not directly handled within the COM's functions invoked by the
                     PDU-R, for example Com_RxIndication
 *******************************************************************************************************************************/
void Com_MainFunctionRx(void)
{
    if(ComState == COM_READY)
    {
        uint8 ComIPduIndex, ComSignalIndex, ComSignalGroupIndex, ComUpdateBitPositionLocal;
        /* Loop over all IPdus */
        for(ComIPduIndex = 0; ComIPduIndex < ComMaxIPduCnt; ComIPduIndex++)
        {
            /* Check if IPdu direction is Receive */
            if(Com.ComConfig.ComIPdu[ComIPduIndex].ComIPduDirection == Receive)
            {
                /* Check if IPdu Signal Processing is DEFERRED*/
                if(Com.ComConfig.ComIPdu[ComIPduIndex].ComIPduSignalProcessing == DEFERRED)
                {
                    /* Loop over all Signals in this IPDU */
                    for(ComSignalIndex=0; Com.ComConfig.ComIPdu[ComIPduIndex].ComIPduSignalRef[ComSignalIndex] != NULL; ComSignalIndex++)
                    {
                        ComUpdateBitPositionLocal = Com.ComConfig.ComIPdu[ComIPduIndex].ComIPduSignalRef[ComSignalIndex]->ComUpdateBitPosition;

                        /* Check if update bit is set*/
                        if(Com.ComConfig.ComIPdu[ComIPduIndex].ComBufferRef[ComUpdateBitPositionLocal / 8] & (1 << (ComUpdateBitPositionLocal % 8)))
                        {
                            /* Notify RTE */
                            if(Com.ComConfig.ComIPdu[ComIPduIndex].ComIPduSignalRef[ComSignalIndex]->ComNotification != NULL)
                            {
                                Com.ComConfig.ComIPdu[ComIPduIndex].ComIPduSignalRef[ComSignalIndex]->ComNotification();
                            }
                            else
                            {

                            }
                            /* Clear update bit */
                            Com.ComConfig.ComIPdu[ComIPduIndex].ComBufferRef[ComUpdateBitPositionLocal / 8] &= ~(1 << (ComUpdateBitPositionLocal % 8));
                        }
                        else
                        {

                        }
                    }
                    /* Loop over all Signal groups in this IPDU */
                    for(ComSignalGroupIndex=0; Com.ComConfig.ComIPdu[ComIPduIndex].ComIPduSignalGroupRef[ComSignalGroupIndex] != NULL; ComSignalGroupIndex++)
                    {
                        ComUpdateBitPositionLocal = Com.ComConfig.ComIPdu[ComIPduIndex].ComIPduSignalGroupRef[ComSignalGroupIndex]->ComUpdateBitPosition;

                        /* Check if update bit is set*/
                        if(Com.ComConfig.ComIPdu[ComIPduIndex].ComBufferRef[ComUpdateBitPositionLocal / 8] & (1 << (ComUpdateBitPositionLocal % 8)))
                        {
                            /* Notify RTE */
                            if(Com.ComConfig.ComIPdu[ComIPduIndex].ComIPduSignalGroupRef[ComSignalGroupIndex]->ComNotification != NULL)
                            {
                                Com.ComConfig.ComIPdu[ComIPduIndex].ComIPduSignalGroupRef[ComSignalGroupIndex]->ComNotification();
                            }
                            else
                            {

                            }
                            /* Clear update bit */
                            Com.ComConfig.ComIPdu[ComIPduIndex].ComBufferRef[ComUpdateBitPositionLocal / 8] &= ~(1 << (ComUpdateBitPositionLocal % 8));
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

    return ;
}


/*********************************************************************************************************************************
 Service name:               Com_SendSignalGroup
 Service ID:                    0x0d
 Parameters (in):               SignalGroupId -->  Id of signal group to be sent.
 Parameters (inout):            None
 Parameters (out):              None
 Return value:                  uint8 --->
                                 ->E_OK: service has been accepted
                                 ->COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group
                                     was stopped (or service failed due to development error)
                                 ->COM_BUSY: in case the TP-Buffer is locked for large data types
                                     handling
 Description:        The service Com_SendSignalGroup copies the content of the associated shadow
                     buffer to the associated I-PDU
 *******************************************************************************************************************************/
uint8 Com_SendSignalGroup(Com_SignalGroupIdType SignalGroupId)
{
    uint8 ComIPduIndex,
        ComSignalGroupIndex,
        ComGroupSignalIndex,
        BitIndex;

    boolean ComCopySignalGroup = false;

    Com_GroupSignalType* ComGroupSignalLocal;
    Com_SignalGroupType* ComSignalGroupLocal;
    Com_IPduType* ComIPduLocal;

    /*[SWS_Com_00804] Error code if any other API service, except Com_GetStatus, is called before the AUTOSAR COM module was initialized with Com_Init
     or after a call to Com_Deinit:
     error code: COM_E_UNINIT
     value [hex]: 0x02
     (SRS_BSW_00337)*/
    if(ComState==COM_UNINIT)
    {
        #if ComConfigurationUseDet == true
            Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, 0x0D, COM_E_UNINIT);
        #endif
        return COM_SERVICE_NOT_AVAILABLE;
    }

    /*[SWS_Com_00803] API service called with wrong parameter:
      error code: COM_E_PARAM
      value [hex]: 0x01
      (SRS_BSW_00337)*/
    else if(SignalGroupId >= ComMaxSignalGroupCnt)
    {
        #if ComConfigurationUseDet == true
            Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, 0x0D, COM_E_PARAM);
        #endif
        return COM_SERVICE_NOT_AVAILABLE;
    }
    else if(SignalGroupId < ComMaxSignalGroupCnt)
    {
        /* Find the IPdu which contains this signal */
        for(ComIPduIndex = 0; ComIPduIndex < ComMaxIPduCnt; ComIPduIndex++)
        {
            for(ComSignalGroupIndex = 0; Com.ComConfig.ComIPdu[ComIPduIndex].ComIPduSignalGroupRef[ComSignalGroupIndex] != NULL; ComSignalGroupIndex++)
            {
                if(Com.ComConfig.ComIPdu[ComIPduIndex].ComIPduSignalGroupRef[ComSignalGroupIndex]->ComHandleId == SignalGroupId)
                {
                    /* Get SignalGroup */
                    ComSignalGroupLocal = Com.ComConfig.ComIPdu[ComIPduIndex].ComIPduSignalGroupRef[ComSignalGroupIndex];

                    /* Get IPdu */
                    ComIPduLocal = &Com.ComConfig.ComIPdu[ComIPduIndex];

                    if((Com.ComConfig.ComIPdu[ComIPduIndex].ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeMode == DIRECT)||\
                        (Com.ComConfig.ComIPdu[ComIPduIndex].ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeMode == MIXED))
                    {
                        switch(ComSignalGroupLocal->ComTransferProperty)
                        {
                            /*At any send request of a signal group with ComTransferProperty TRIGGERED assigned to an I-PDU with
                             *ComTxModeMode DIRECT or MIXED, the AUTOSAR COM module shall immediately (within the next main function at the latest)
                             *initiate ComTxModeNumberOfRepetitions plus one transmissions of the as-signed I-PDU.*/
                            case TRIGGERED:
                            {
                                ComTeamConfig.ComTeamIPdu[ComIPduIndex].ComTeamTxMode.ComTeamTxIPduNumberOfRepetitions = Com.ComConfig.ComIPdu[ComIPduIndex].ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeNumberOfRepetitions +1 ;
                                ComCopySignalGroup = true;
                                break;
                            }

                            /*At any send request of a signal group with ComTransferProperty TRIGGERED_WITHOUT_REPETITION assigned to an I-PDU
                             *with ComTx-ModeMode DIRECT or MIXED, the AUTOSAR COM module shall initiate one transmission of the assigned I-PDU
                             *within the next main function at the latest.*/
                            case TRIGGERED_WITHOUT_REPETITION:
                            {
                                ComTeamConfig.ComTeamIPdu[ComIPduIndex].ComTeamTxMode.ComTeamTxIPduNumberOfRepetitions = 1;
                                ComCopySignalGroup = true;
                                break;
                            }

                            /*At a send request of a signal group with ComTransferProperty TRIGGERED_ON_CHANGE_WITHOUT_REPETITION assigned to an I-PDU
                             *with ComTxModeMode DIRECT or MIXED, the AUTOSAR COM module shall immediately (within the next main function at the latest)
                             *initiate one transmission of the as-signed I-PDU, if at least one new sent group signal differs to the locally
                             *stored (last sent or init) in length or value.*/
                            case TRIGGERED_ON_CHANGE_WITHOUT_REPETITION:
                            {
                                for(ComGroupSignalIndex=0;Com.ComConfig.ComSignalGroup[ComSignalGroupIndex].ComGroupSignalRef[ComGroupSignalIndex]!= NULL; ComGroupSignalIndex++)
                                {
                                    /* Compare data value between ComIPduBuffer and ComGroupSignalBuffer */
                                    if(Com_GroupSignalDataCmp(ComIPduIndex, ComGroupSignalIndex))
                                    {
                                        ComTeamConfig.ComTeamIPdu[ComIPduIndex].ComTeamTxMode.ComTeamTxIPduNumberOfRepetitions = 1;
                                        ComCopySignalGroup = true;
                                        break;
                                    }
                                    else
                                    {

                                    }

                                }
                                break;
                            }
                            /*Regarding signal groups with ComTransferProperty TRIG-GERED_ON_CHANGE which do not contain any signals
                             *that have an own ComTransferProperty configured: At any send request of such a signal group assigned to an I-PDU
                             *with ComTx-ModeMode DIRECT or MIXED, the AUTOSAR COM module shall immediately (with-in the next main function at the latest)
                             *initiate ComTxModeNumberOfRepetitions plus one transmissions of the assigned I-PDU, if at least one new sent group signal
                             *differs to the locally stored (last sent or init) in length or value.*/
                            case TRIGGERED_ON_CHANGE:
                            {
                                for(ComGroupSignalIndex=0;Com.ComConfig.ComSignalGroup[ComSignalGroupIndex].ComGroupSignalRef[ComGroupSignalIndex]!= NULL; ComGroupSignalIndex++)
                                {
                                    /* Compare data value between ComIPduBuffer and ComGroupSignalBuffer */
                                    if(Com_GroupSignalDataCmp(ComIPduIndex, ComGroupSignalIndex))
                                    {
                                        ComTeamConfig.ComTeamIPdu[ComIPduIndex].ComTeamTxMode.ComTeamTxIPduNumberOfRepetitions = Com.ComConfig.ComIPdu[ComIPduIndex].ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeNumberOfRepetitions + 1;
                                        ComCopySignalGroup = true;
                                        break;
                                    }
                                    else
                                    {

                                    }
                                }
                                break;
                            }

                            default:
                                break;
                            }
                        }
                        else
                        {

                        }

                        if(ComCopySignalGroup)
                        {
                            for(ComGroupSignalIndex = 0; Com.ComConfig.ComSignalGroup[ComSignalGroupIndex].ComGroupSignalRef[ComGroupSignalIndex] != NULL; ComGroupSignalIndex++)
                            {
                                /*Get Group Signal*/
                                ComGroupSignalLocal = Com.ComConfig.ComSignalGroup[ComSignalGroupIndex].ComGroupSignalRef[ComGroupSignalIndex];
                                /*The service Com_SendSignalGroup shall copy the content of the shadow buffer referenced by parameter SignalGroupId
                                 *to the associated I-PDU.*/
                                for(BitIndex = 0; BitIndex < ComGroupSignalLocal->ComBitSize; BitIndex++)
                                {
                                    if((ComGroupSignalLocal->ComBufferRef[BitIndex / 8] >> (BitIndex % 8)) & 1)
                                    {
                                        ComIPduLocal->ComBufferRef[(BitIndex + ComGroupSignalLocal->ComBitPosition) / 8] |= 1 << ((BitIndex + ComGroupSignalLocal->ComBitPosition)%8);
                                    }
                                    else
                                    {
                                        ComIPduLocal->ComBufferRef[(BitIndex + ComGroupSignalLocal->ComBitPosition) / 8] &= ~(1 << ((BitIndex + ComGroupSignalLocal->ComBitPosition)%8));
                                    }

                                }
                            }
                            /*Set update bit*/
                            ComIPduLocal->ComBufferRef[ComSignalGroupLocal->ComUpdateBitPosition / 8] |= 1 << (ComSignalGroupLocal->ComUpdateBitPosition % 8);
                        }
                        else
                        {

                        }
                        return E_OK;
                    }
                    else
                    {
                    }

                }
            }
        }
        else
        {

        }
        return COM_SERVICE_NOT_AVAILABLE;
}
/*********************************************************************************************************************************
 Service name:               Com_ReceiveSignalGroup
 Service ID:                    0x0e
 Parameters (in):               SignalGroupId -->  Id of signal group to be sent.
 Parameters (inout):            None
 Parameters (out):              None
 Return value:                  uint8 --->
                                 ->E_OK: service has been accepted
                                 ->COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group
                                     was stopped (or service failed due to development error)
                                 ->COM_BUSY: in case the TP-Buffer is locked for large data types
                                     handling
 Description:        The service Com_ReceiveSignalGroup copies the received signal group from the
                     I-PDU to the shadow buffer.
 *******************************************************************************************************************************/
uint8 Com_ReceiveSignalGroup(Com_SignalGroupIdType SignalGroupId)
{
    uint8 ComIPduIndex, ComSignalGroupIndex, ComGroupSignalIndex, BitIndex;
    Com_GroupSignalType* ComGroupSignalLocal;
    Com_IPduType* ComIPduLocal;

    /*[SWS_Com_00638] âŒˆThe service Com_ReceiveSignalGroup shall copy the received
    signal group from the I-PDU to the shadow buffer.âŒ‹ (SRS_Com_02041)*/
    if(SignalGroupId < ComMaxSignalGroupCnt)
    {
        /*After this call, the group signals could be copied from the shadow buffer to the RTE
        by calling Com_ReceiveSignal.*/
        /* Find the IPdu which contains this signal */
        for(ComIPduIndex = 0; ComIPduIndex < ComMaxIPduCnt; ComIPduIndex++)
        {
            for(ComSignalGroupIndex = 0; Com.ComConfig.ComIPdu[ComIPduIndex].ComIPduSignalGroupRef[ComSignalGroupIndex] != NULL; ComSignalGroupIndex++)
            {
                if(Com.ComConfig.ComIPdu[ComIPduIndex].ComIPduSignalGroupRef[ComSignalGroupIndex]->ComHandleId == SignalGroupId)
                {
                    /* Get IPdu */
                    ComIPduLocal = &Com.ComConfig.ComIPdu[ComIPduIndex];

                    for(ComGroupSignalIndex = 0; Com.ComConfig.ComSignalGroup[ComSignalGroupIndex].ComGroupSignalRef[ComGroupSignalIndex] != NULL; ComGroupSignalIndex++)
                    {
                        /*Get Group Signal*/
                        ComGroupSignalLocal = Com.ComConfig.ComSignalGroup[ComSignalGroupIndex].ComGroupSignalRef[ComGroupSignalIndex];

                        /* Write data from signal buffer to IPdu*/
                        for(BitIndex = ComGroupSignalLocal->ComBitPosition; BitIndex < ComGroupSignalLocal->ComBitPosition + ComGroupSignalLocal->ComBitSize; BitIndex++)
                        {
                            if((ComIPduLocal->ComBufferRef[BitIndex / 8] >> (BitIndex % 8)) & 1)
                            {
                               ComGroupSignalLocal->ComBufferRef[(BitIndex - ComGroupSignalLocal->ComBitPosition) / 8] |= 1 << ((BitIndex - ComGroupSignalLocal->ComBitPosition) % 8);
                            }
                           else
                            {
                               ComGroupSignalLocal->ComBufferRef[(BitIndex - ComGroupSignalLocal->ComBitPosition) / 8] &= ~(1 << ((BitIndex - ComGroupSignalLocal->ComBitPosition) % 8));
                            }
                        }
                    }
                }
                else
                {
                }
            }
        }
        return E_OK;
    }
    else
    {
        return COM_SERVICE_NOT_AVAILABLE;
    }
}
/*********************************************************************************************************************************
 Service name:               Com_MainFunctionTx
 Service ID:                    0x17
 Parameters (in):               None
 Parameters (inout):            PduId
 Parameters (out):              None
 Return value:                  Std_ReturnType
 Description:      
                        By a call to Com_TriggerIPDUSend the I-PDU with the given ID is triggered for transmission.
*******************************************************************************************************************************/
Std_ReturnType  Com_TriggerIPDUSend(PduIdType PduId)
{
    PduInfoType* pduinfo = &(PduInfoType){
                            .SduDataPtr = Com.ComConfig.ComIPdu[PduId].ComBufferRef,
                            .SduLength = Com.ComConfig.ComIPdu[PduId].ComIPduLength
    };
    uint8 ComSignalIndex, ComSignalGroupIndex, ComUpdateBitPositionLocal;

    if ( PduId > ComMaxIPduCnt)
    {
#if (ComConfigurationUseDet == true )
        Det_ReportError(MODULE_ID, INSTANCE_ID, 0x17, COM_E_PARAM);
#endif
        return E_NOT_OK ;
    }
    else if (ComState == COM_UNINIT)
    {
#if (ComConfigurationUseDet == true )
        Det_ReportError(MODULE_ID, INSTANCE_ID, 0x17, COM_E_UNINIT);
#endif
        return E_NOT_OK ;
    }
    else {
#if (ComEnableMDTForCyclicTransmission == true)
        if(MinimumDelayTime == 0){
#endif

        if (PduR_ComTransmit(PduId, pduinfo) == E_OK)
        {

            if (Com.ComConfig.ComIPdu[PduId].ComTxIPdu.ComTxIPduClearUpdateBit
                    == Transmit)
            {
                /* Loop over all Signals in this IPDU */
                for (ComSignalIndex = 0;
                        Com.ComConfig.ComIPdu[PduId].ComIPduSignalRef[ComSignalIndex]
                                != NULL; ComSignalIndex++)
                {
                    ComUpdateBitPositionLocal =
                            Com.ComConfig.ComIPdu[PduId].ComIPduSignalRef[ComSignalIndex]->ComUpdateBitPosition;

                    /* Check if update bit is set*/
                    if (Com.ComConfig.ComIPdu[PduId].ComBufferRef[ComUpdateBitPositionLocal
                            / 8] & (1 << (ComUpdateBitPositionLocal % 8)))
                    {

                        /* Clear update bit */
                        Com.ComConfig.ComIPdu[PduId].ComBufferRef[ComUpdateBitPositionLocal
                                / 8] &= ~(1 << (ComUpdateBitPositionLocal % 8));
                    }
                    else
                    {

                    }
                }
                /* Loop over all Signal groups in this IPDU */
                for (ComSignalGroupIndex = 0;
                        Com.ComConfig.ComIPdu[PduId].ComIPduSignalGroupRef[ComSignalGroupIndex]
                                != NULL; ComSignalGroupIndex++)
                {
                    ComUpdateBitPositionLocal =
                            Com.ComConfig.ComIPdu[PduId].ComIPduSignalGroupRef[ComSignalGroupIndex]->ComUpdateBitPosition;

                    /* Check if update bit is set*/
                    if (Com.ComConfig.ComIPdu[PduId].ComBufferRef[ComUpdateBitPositionLocal
                            / 8] & (1 << (ComUpdateBitPositionLocal % 8)))
                    {
                        /* Clear update bit */
                        Com.ComConfig.ComIPdu[PduId].ComBufferRef[ComUpdateBitPositionLocal
                                / 8] &= ~(1 << (ComUpdateBitPositionLocal % 8));
                    }
                    else
                    {

                    }
                }
            }
            #if ComEnableMDTForCyclicTransmission
            ComTeamConfig.ComTeamIPdu[PduId]ComTeamTxMode.ComTeamMinimumDelayTimer = Com.ComConfig.ComIPdu[PduId].ComTxIPdu.ComMinimumDelayTime;
            #endif
            return E_OK;
        }
        else
        {

            return E_NOT_OK;

        }

#if (ComEnableMDTForCyclicTransmission == true)
        }
        else {
            return E_NOT_OK;
        }
#endif

    }

}

#define CONTROLLER_ID   (uint8)0
#define SIGNAL_ID       (uint8)1
#define GROUPSIGNAL_ID  (uint8)0
#define SIGNALGROUP_ID  (uint8)0

void Timer0IntHandler(void)
{

}

void main()
{
    uint32 Period;
    Com_IPduType *ComIPduLoc = &Com.ComConfig.ComIPdu[0];
    uint8 SignalData = 0x00;
    uint16 GroupSignalData = 0xFFFF;
    const void* SignalDataPtr = &SignalData;
    const void* GroupSignalDataPtr = &GroupSignalData;

    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    /******************Setup UART for debugging*********************/
    // Enable UART0 and GPIOA peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    // Configure pins A0 and A1 as Rx and Tx
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Set baudrate to 115200
    UARTStdioConfig(0, 115200, SysCtlClockGet());
    /***************************************************************/

    /**************************Setup CAN0***************************/
    // Enable CAN0 and GPIOB peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    // Configure pins B4 and B5 as Rx and Tx
    GPIOPinConfigure(GPIO_PB4_CAN0RX);
    GPIOPinConfigure(GPIO_PB5_CAN0TX);
    GPIOPinTypeCAN(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    /***************************************************************/


    Can_Init(&Can);
    CanIf_Init(&CanIf);
    PduR_Init(&PduR);
    Com_Init(&(Com.ComConfig));

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    /**********************Setup TIMER0*****************************/
    /*while (!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0));
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    Period = (SysCtlClockGet() / 2);
    TimerLoadSet(TIMER0_BASE, TIMER_A, Period - 1);
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntMasterEnable();
    TimerEnable(TIMER0_BASE, TIMER_A);
    */
    /***************************************************************/

    CanIf_SetControllerMode(CONTROLLER_ID, CANIF_CS_STARTED);
    //SysCtlDelay(SysCtlClockGet() / 3);
    uint8 Counter = 0;

    if(CanIf_SetControllerMode(CONTROLLER_ID, CANIF_CS_STARTED) == E_OK)
    {
        UARTprintf("CanIf_SetControllerMode E_OK\n");
        Can_MainFunction_Mode();
        while(1)
        {
            Com_MainFunctionRx();
            /*Counter++;
            if(Counter == 11)
            {
                if(Com_SendSignal(SIGNAL_ID, SignalDataPtr) == E_OK)
                {
                    SignalData ++;
                    UARTprintf("Com_SendSignal\n");
                }
                Com_UpdateShadowSignal(GROUPSIGNAL_ID, GroupSignalDataPtr);
                if(Com_SendSignalGroup(SIGNALGROUP_ID) == E_OK)
                {
                    UARTprintf("Com_SendSignalGroup\n");
                }
                GroupSignalData++;
                Counter = 0;

            }
            SysCtlDelay(SysCtlClockGet() / 30);
            */
        }
    }
    else
    {

    }
}

