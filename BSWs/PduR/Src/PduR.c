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
extern PduR_Type PduR;
static PduR_StateType PduRState;



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
	
}

/*********************************************************************************************************************************
 Service name:                  PduR_ComTransmit
 Service ID:                       0x<UpModId>9
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
	
}

/*********************************************************************************************************************************
 Service name:                  PduR_CanIfRxIndication
 Service ID:                      0x<LoModId>1
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
	UARTprintf("PduR_CanIfRxIndication PDU= %d Data= %d\n", RxPduId, *(PduInfoPtr->SduDataPtr));
}

/*********************************************************************************************************************************
 Service name:                  PduR_CanIfTxConfirmation
 Service ID:                      0x<LoModId>2
 Parameters (in):           TxPduId--> ID of the I-PDU that has been transmitted.
 Parameters (inout):              None
 Parameters (out):                None
 Return value:                    None
 Description:               The lower layer communication interface module confirms the transmission of an IPDU.
*******************************************************************************************************************************/
void PduR_CanIfTxConfirmation(PduIdType TxPduId)
{
    UARTprintf("PduR_TxConfirmation PDU= %d\n", TxPduId);
}
