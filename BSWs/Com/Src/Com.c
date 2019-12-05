/********************************************************************************************************************************
 **
 FileName:                   Com.c
 AUTOSAR Version:            4.2.2
 **
 ********************************************************************************************************************************/
/********************************************************************************************************************************
 **                                                          Includes                                                                                                **
 ********************************************************************************************************************************/
#include "Com.h"
#include "Det.h"
#include "PduR.h"

//#include "CanSM.h"
/********************************************************************************************************************************
 **                                                       Global Variables                                                                                       **
 ********************************************************************************************************************************/
extern Com_Type Com;

static Com_StateType ComState = COM_UNINIT;

uint8 ComDevelopmentError;

ComTeamConfig_type ComTeamConfig;

/********************************************************************************************************************************
 **                                                        Local Functions                                                                                        **
 *********************************************************************************************************************************/

static void Com_WriteSignalToIPdu(Com_SignalIdType SignalId,const void *SignalDataPtr);
static void Com_ReadSignalFromIPdu(Com_SignalIdType SignalId, void* SignalDataPtr);
static void Com_CopyShadowBufferToIPdu(Com_SignalIdType SignalGroupId);
static void Com_CopySignalGroupToShadowBuffer(Com_SignalGroupIdType SignalGroupId);

void Com_WriteSignalToIPdu(Com_SignalIdType SignalId,const void *SignalDataPtr)
{
	Com_SignalType *ComSignalLocal;
	Com_IPduType *ComIPduLocal;
	uint8 ComIPduIndex, ComSignalIndex, BitIndex;

	/* Check that the signal ID is a valid ID*/
	if(SignalId < ComMaxSignalCnt)
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
					
					return;
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
}

void Com_ReadSignalFromIPdu(Com_SignalIdType SignalId, void* SignalDataPtr)
{
	Com_SignalType *ComSignalLocal;
	Com_IPduType *ComIPduLocal;
	uint8 ComIPduIndex, ComSignalIndex, BitIndex;

	/* Check that the signal ID is a valid ID*/
	if(SignalId < ComMaxSignalCnt)
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

					return;

				}
				else
				{

				}
			}

			SignalDataPtr = ComSignalLocal->ComBufferRef;
		}
	}
	else
	{

	}
}

void Com_CopyShadowBufferToIPdu(Com_SignalIdType SignalGroupId)
{
	uint8 ComIPduIndex, ComSignalGroupIndex, ComGroupSignalIndex, BitIndex;
	Com_GroupSignalType* ComGroupSignalLocal;
	Com_SignalGroupType* ComSignalGroupLocal;
	Com_IPduType* ComIPduLocal;

	if(SignalGroupId < ComMaxSignalGroupCnt)
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

					for(ComGroupSignalIndex = 0; Com.ComConfig.ComSignalGroup[ComSignalGroupIndex].ComGroupSignalRef[ComGroupSignalIndex] != NULL; ComGroupSignalIndex++)
					{
						/*Get Group Signal*/
						ComGroupSignalLocal = Com.ComConfig.ComSignalGroup[ComSignalGroupIndex].ComGroupSignalRef[ComGroupSignalIndex];

						/* Write data from signal buffer to IPdu*/
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
				}
				else
				{
				}

				/*Set update bit*/
                ComIPduLocal->ComBufferRef[ComSignalGroupLocal->ComUpdateBitPosition / 8] |= 1 << (ComSignalGroupLocal->ComUpdateBitPosition % 8);
                return;
			}
		}
	}
	else
	{

	}
}

void Com_CopySignalGroupToShadowBuffer(Com_SignalGroupIdType SignalGroupId)
{
	uint8 ComIPduIndex, ComSignalGroupIndex, ComGroupSignalIndex, BitIndex;
	Com_GroupSignalType* ComGroupSignalLocal;
	Com_IPduType* ComIPduLocal;

	if(SignalGroupId < ComMaxSignalGroupCnt)
	{
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
	}
	else
	{
	}
}








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
void Com_Init( const Com_ConfigType* config)
{
















































   return ;
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
	uint8 ComIPduIndex, ComSignalIndex;
    /*
     [SWS_Com_00804] ⌈Error code if any other API service, except Com_GetStatus, is called before the AUTOSAR COM module was initialized with Com_Init
     or after a call to Com_Deinit:
     error code: COM_E_UNINIT
     value [hex]: 0x02
     (SRS_BSW_00337)
   */
    if(ComState == COM_UNINIT)
    {
        Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, 0x0A, COM_E_UNINIT);
        return COM_SERVICE_NOT_AVAILABLE;
    }

  uint8 ApiId=0x0a;
    
    /*
     [SWS_Com_00804] ⌈Error code if any other API service, except Com_GetStatus, is called before the AUTOSAR COM module was initialized with Com_Init
     or after a call to Com_Deinit:
     error code: COM_E_UNINIT
     value [hex]: 0x02
     (SRS_BSW_00337)
   */
    if(Com_StateType==COM_UNINIT)
    {
        Det_ReportError(ModuleId,InstanceId,ApiId,COM_E_UNINIT);
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
        Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, 0x0A, COM_E_PARAM);
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
        Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, 0x0A, COM_E_PARAM_POINTER);
        return COM_SERVICE_NOT_AVAILABLE;
    }

    else
    {
        /*
         *  [SWS_Com_00624] ⌈The service Com_SendSignal shall update the signal object identified by SignalId with the signal referenced by
           the SignalDataPtr parameter.
           (SRS_Com_02037)
         */

    /*
      [SWS_Com_00805] ⌈NULL pointer checking:
      error code: COM_E_PARAM_POINTER
      value [hex]: 0x03
      (SRS_BSW_00414)
    */
    else if(SignalDataPtr==NULL)
    {
        Det_ReportError(ModuleId,InstanceId,ApiId,COM_E_PARAM_POINTER);
        return COM_SERVICE_NOT_AVAILABLE;
    }

		Com_SignalType* ComSignalLocal;
		Com_IPduType* ComIPduLocal;
		ComTeamIPdu_type* ComTeamIPduLocal;

    else
    {

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
							}
							else
							{
							
							}
							break;

                switch (Signal -> ComTransferProperty)
                {

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
							break;
					
						default:
							return COM_SERVICE_NOT_AVAILABLE;
					}
					
					Com_WriteSignalToIPdu(SignalId,SignalDataPtr);
					
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
















































   return ;
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
















































   return ;
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
                if(ComIPduLocal->ComIPduDirection == SEND)
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
    ComTeamIPdu_type *ComTeamIPduLoc;
    boolean MinimumDelayTimerLoc=true;

    if(ComState == COM_READY)
    {
		for ( IPduIdIndex = 0; IPduIdIndex < ComMaxIPduCnt; IPduIdIndex++)
		{
			ComIPduLoc =&Com.ComConfig.ComIPdu[IPduIdIndex];
			ComTeamIPduLoc = &ComTeamConfig.ComTeamIPdu[IPduIdIndex];

			/* check if  IPdu Direction should be transmitted */
			if (ComIPduLoc->ComIPduDirection == SEND)
			{
				#if(ComEnableMDTForCyclicTransmission == true)
				
					if(ComTeamIPduLoc->ComTeamTxMode.ComTeamMinimumDelayTimer> 0)
					{
						ComTeamIPduLoc->ComTeamTxMode.ComTeamMinimumDelayTimer -= ComTxTimeBase;
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
						ComTeamIPduLoc->ComTeamTxMode.ComTeamTxModeTimePeriod -= ComTxTimeBase;
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
			/* Check if IPdu direction is RECEIVE */
			if(Com.ComConfig.ComIPdu[ComIPduIndex].ComIPduDirection == RECEIVE)
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
















































   return ;
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
	/*[SWS_Com_00638] âŒˆThe service Com_ReceiveSignalGroup shall copy the received
	signal group from the I-PDU to the shadow buffer.âŒ‹ (SRS_Com_02041)*/
	if(SignalGroupId < ComMaxSignalGroupCnt)
	{	
	 /*After this call, the group signals could be copied from the shadow buffer to the RTE
		by calling Com_ReceiveSignal.*/
		Com_CopySignalGroupToShadowBuffer(SignalGroupId);
		return E_OK;
	}
	else
	{
		return COM_SERVICE_NOT_AVAILABLE;
	}
}

/*********************************************************************************************************************************
 Service name:               Com_TriggerIPduSend
 Service ID:                    0x17
 Parameters (in):               PduId -->  The I-PDU-ID of the I-PDU that shall be triggered for sending.
 Parameters (inout):            None
 Parameters (out):              None
 Return value:                  Std_ReturnType  --->
									E_OK: I-PDU was triggered for transmission
									E_NOT_OK: I-PDU is stopped, the transmission could not be triggered
 Description:        			By a call to Com_TriggerIPDUSend the I-PDU with the given ID is triggered for transmission.
*******************************************************************************************************************************/
Std_ReturnType Com_TriggerIPDUSend(PduIdType PduId)
{
	return;
}

void main(void)
{

}
