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
extern Com_ConfigType Com;

static Com_StateType ComState = COM_UNINIT;

uint8 ComDevelopmentError;

/********************************************************************************************************************************
 **                                                        Local Functions                                                                                        **
 *********************************************************************************************************************************/

static void Com_WriteSignalToIPdu(Com_SignalIdType SignalId, void *SignalDataPtr);
static void Com_ReadSignalFromIPdu(Com_SignalIdType SignalId, void* SignalDataPtr);
static void Com_CopyShadowBufferToIPdu(Com_SignalIdType SignalGroupId);
static void Com_CopySignalGroupToShadowBuffer(Com_SignalGroupIdType SignalGroupId);

void Com_WriteSignalToIPdu(Com_SignalIdType SignalId, void *SignalDataPtr)
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
					   if((*(ComSignalLocal->ComBufferRef) >> BitIndex) & 1)
					   {
						   ComIPduLocal->ComBufferRef[(BitIndex + ComSignalLocal->ComBitPosition) / 8] |= 1 << ((BitIndex + ComSignalLocal->ComBitPosition)%8);
					   }
					   else
					   {
						   ComIPduLocal->ComBufferRef[(BitIndex + ComSignalLocal->ComBitPosition) / 8] &= ~(1 << ((BitIndex + ComSignalLocal->ComBitPosition)%8));
					   }
					}
					/*Set update bit*/ 
					ComIPduLocal->ComBufferRef[ComSignalLocal->UpdateBitPosition / 8] |= 1 << (ComSignalLocal->UpdateBitPosition%8);
					
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
						   *(ComSignalLocal->ComBufferRef) |= 1 << (BitIndex - ComSignalLocal->ComBitPosition);
					   }
					   else
					   {
						   *(ComSignalLocal->ComBufferRef) &= ~(1 << (BitIndex - ComSignalLocal->ComBitPosition));
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
	Com_IPduType* ComIPduLocal;
	
	if(SignalGroupId < ComMaxSignalGroupCnt)
	{
		/* Find the IPdu which contains this signal */
		for(ComIPduIndex = 0; ComIPduIndex < ComMaxIPduCnt; ComIPduIndex++)
		{
			for(ComSignalGroupIndex = 0; Com.ComConfig.ComIPdu[ComIPduIndex].ComSignalGroupRef[ComSignalGroupIndex] != NULL; ComSignalGroupIndex++)
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
						for(BitIndex = 0; BitIndex < ComGroupSignalLocal->ComBitSize; BitIndex++)
						{
							if((*(ComGroupSignalLocal->ComBufferRef) >> BitIndex) & 1)
							{
								ComIPduLocal->ComBufferRef[(BitIndex + ComGroupSignalLocal->ComBitPosition) / 8] |= 1 << ((BitIndex + ComGroupSignalLocal->ComBitPosition)%8);
							}
							else
							{
								ComIPduLocal->ComBufferRef[(BitIndex + ComGroupSignalLocal->ComBitPosition) / 8] &= ~(1 << ((BitIndex + ComGroupSignalLocal->ComBitPosition)%8));
							}
						}
						/*Set update bit*/ 
						ComIPduLocal->ComBufferRef[ComGroupSignalLocal->UpdateBitPosition / 8] |= 1 << (ComGroupSignalLocal->UpdateBitPosition % 8);
						
						return;
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
			for(ComSignalGroupIndex = 0; Com.ComConfig.ComIPdu[ComIPduIndex].ComSignalGroupRef[ComSignalGroupIndex] != NULL; ComSignalGroupIndex++)
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
							   *(ComGroupSignalLocal->ComBufferRef) |= 1 << (BitIndex - ComGroupSignalLocal->ComBitPosition);
						    }
						   else
						    {
							   *(ComGroupSignalLocal->ComBufferRef) &= ~(1 << (BitIndex - ComGroupSignalLocal->ComBitPosition));
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
















































   return ;
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
















































   return ;
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
















































   return ;
}


/*********************************************************************************************************************************
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
							Com.ComConfig.ComIPdu[ComIPduIndex].ComIPduSignalRef[ComSignalIndex]->ComNotification();
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
							Com.ComConfig.ComIPdu[ComIPduIndex].ComIPduSignalGroupRef[ComSignalGroupIndex]->ComNotification();
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
















































   return ;
}

void main(void)
{

}
