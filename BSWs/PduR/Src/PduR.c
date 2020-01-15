/********************************************************************************************************************************
 **
 FileName:                   PduR.c
 AUTOSAR Version:            4.2.2
                                                                                                                                                                          **
 ********************************************************************************************************************************/
/********************************************************************************************************************************
 **                                                          Includes                                                                                                **
 ********************************************************************************************************************************/
#include "PduR.h"
#include "Com.h"
#include "CanIf.h"
#include "Det.h"
/********************************************************************************************************************************
 **                                                       Global Variables                                                                                       **
 ********************************************************************************************************************************/
extern PduR_PBConfigType PduR;
static PduR_StateType PduRState = PDUR_UNINIT;



/********************************************************************************************************************************
**                                                        Local Functions                                                       **
*********************************************************************************************************************************/


/********************************************************************************************************************************
**                                                      Functions  definition                                                  **
*********************************************************************************************************************************/
/*********************************************************************************************************************************
 Service name:                  PduR_Init
 Service ID:                      0xF0
 Parameters (in):           ConfigPtr--> Pointer to post build configuration
 Parameters (inout):              None
 Parameters (out):                None
 Return value:                    None
 Description:                  Initializes the PDU Router.
*******************************************************************************************************************************/
void PduR_Init(const PduR_PBConfigType* ConfigPtr)
{
	PduRState = PDUR_ONLINE;
}

/*********************************************************************************************************************************
 Service name:                  PduR_ComTransmit
 Service ID:                       0x89
 Parameters (in):           id--> Identification of the I-PDU.
							info-->Length and pointer to the buffer of the I-PDU.
 Parameters (inout):              None
 Parameters (out):                None
 Return value:              Std_ReturnType-->
							E_OK - request is accepted by the destination module; transmission is continued.
							E_NOT_OK - request is not accepted by the destination module;
							transmission is aborted.
 Description:               Requests transmission of an I-PDU.
*******************************************************************************************************************************/
Std_ReturnType PduR_ComTransmit(PduIdType id,const PduInfoType* info)
{
	uint8 PduRRoutingPathIndex;

	if (PduRState == PDUR_UNINIT) 
	{ 
		#if PduRDevErrorDetect
		Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, 0x89, PDUR_E_INVALID_REQUEST);
		#endif
	}
	else
	{
		for (PduRRoutingPathIndex = 0; PduRRoutingPathIndex < PduRMaxRoutingPathCnt; PduRRoutingPathIndex++)
		{
			if (PduR.PduRRoutingTables.PduRRoutingTable[0].PduRRoutingPath[PduRRoutingPathIndex].PduRSrcPdu.PduRSourcePduHandleId == id)
			{ 
				CanIf_Transmit(PduR.PduRRoutingTables.PduRRoutingTable[0].PduRRoutingPath[PduRRoutingPathIndex].PduRDestPdu[0].PduRDestPduHandleId, info);
				return E_OK;
			}
			else
			{

			}
		}
		
		#if PduRDevErrorDetect
			Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, 0x89, PDUR_E_PDU_ID_INVALID); 
		#endif
	}
	return E_NOT_OK;
}

/*********************************************************************************************************************************
 Service name:                  PduR_CanIfRxIndication
 Service ID:                      0x01
 Parameters (in):           RxPduId--> ID of the received I-PDU.
							PduInfoPtr-->Contains the length (SduLength) of the received I-PDU and a pointer to 
										a buffer (SduDataPtr) containing the I-PDU.
 Parameters (inout):              None
 Parameters (out):                None
 Return value:                    None
 Description:                Indication of a received I-PDU from a lower layer communication interface module.
*******************************************************************************************************************************/
void PduR_CanIfRxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr)
{
	 /*
     The reception of an I-PDU received from a communication interface module or from
     transport protocol module and forwarded to the COM module.
    */
	UARTprintf("PduR_CanIfRxIndication PDU= %d Data= %d\n", RxPduId, *(PduInfoPtr->SduDataPtr));
    uint8 PduRRoutingPathIndex;
	if (PduRState == PDUR_UNINIT) 
	{ 
		#if PduRDevErrorDetect
		Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, 0x01, PDUR_E_INVALID_REQUEST);
		#endif
	}
	else
	{
		for(PduRRoutingPathIndex=0; PduRRoutingPathIndex < PduRMaxRoutingPathCnt;PduRRoutingPathIndex++)
		{
			if(RxPduId==PduR.PduRRoutingTables.PduRRoutingTable[0].PduRRoutingPath[PduRRoutingPathIndex].PduRSrcPdu.PduRSourcePduHandleId)
			{
				Com_RxIndication(PduR.PduRRoutingTables.PduRRoutingTable[0].PduRRoutingPath[PduRRoutingPathIndex].PduRDestPdu[0].PduRDestPduHandleId,PduInfoPtr);
				return;
			}
			else
			{
			/* MISRA ERROR */
			}
		}
		#if PduRDevErrorDetect
		Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, 0x01, PDUR_E_PDU_ID_INVALID); 
		#endif
	}
	return;
}

/*********************************************************************************************************************************
 Service name:                  PduR_CanIfTxConfirmation
 Service ID:                      0x02
 Parameters (in):           TxPduId--> ID of the I-PDU that has been transmitted.
 Parameters (inout):              None
 Parameters (out):                None
 Return value:                    None
 Description:               The lower layer communication interface module confirms the transmission of an IPDU.
*******************************************************************************************************************************/
void PduR_CanIfTxConfirmation(PduIdType TxPduId) 
{
	UARTprintf("PduR_TxConfirmation PDU= %d\n", TxPduId);

	uint8 PduRRoutingPathIndex;

	if (PduRState == PDUR_UNINIT) 
	{ 
		#if PduRDevErrorDetect
		Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, 0x02, PDUR_E_INVALID_REQUEST);
		#endif
		return;
	}
	else
	{
		for (PduRRoutingPathIndex = 0;  PduRRoutingPathIndex < PduRMaxRoutingPathCnt; PduRRoutingPathIndex++)
		{
			if ((TxPduId == PduR.PduRRoutingTables.PduRRoutingTable[0].PduRRoutingPath[PduRRoutingPathIndex].PduRSrcPdu.PduRSourcePduHandleId)&&(PduR.PduRRoutingTables.PduRRoutingTable[0].PduRRoutingPath[PduRRoutingPathIndex].PduRSrcPdu.PduRSrcPduUpTxConf)) 
			{ 
				Com_TxConfirmation(PduR.PduRRoutingTables.PduRRoutingTable[0].PduRRoutingPath[PduRRoutingPathIndex].PduRDestPdu[0].PduRDestPduHandleId);
				return;
			}
			else
			{
				/*misra*/
			}				
		}
		#if PduRDevErrorDetect
			Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, 0x02, PDUR_E_PDU_ID_INVALID); 
		#endif
	}			
}
