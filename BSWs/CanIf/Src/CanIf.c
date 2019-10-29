/********************************************************************************************************************************
 **
 FileName:                   CanIf.c
 AUTOSAR Version:            4.2.2
                                                                                                                                                                          **
 ********************************************************************************************************************************/
/********************************************************************************************************************************
 **                                                          Includes                                                                                                **
 ********************************************************************************************************************************/
#include <CanIf.h>

/********************************************************************************************************************************
 **                                                       Global Variables                                                                                       **
 ********************************************************************************************************************************/
  CanIf_NotifStatusType TxPduState  [CanIfMaxTxPduCfg];
 CanIf_NotifStatusType RxPduState[CanIfMaxRxPduCfg];
 bool CanIfState=false;
 
 
 
 
 
 
 
 
 
 /********************************************************************************************************************************
**                                                        Local Functions                                                                                        **
*********************************************************************************************************************************/











/********************************************************************************************************************************
**                                                      Functions  defination                                                                        **
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
void CanIf_Init(const CanIf_ConfigType* ConfigPtr){
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
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

Std_ReturnType CanIf_SetControllerMode(uint8 ControllerId,CanIf_ControllerModeType ControllerMode){
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
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
Std_ReturnType CanIf_GetControllerMode(uint8 ControllerId,CanIf_ControllerModeType* ControllerModePtr){
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
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

Std_ReturnType CanIf_Transmit(PduIdType CanIfTxSduId,const PduInfoType* CanIfTxInfoPtr){
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
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
Std_ReturnType CanIf_ReadRxPduData(PduIdType CanIfRxSduId,PduInfoType* CanIfRxInfoPtr){
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}


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

Std_ReturnType CanIf_SetPduMode(uint8 ControllerId, CanIf_PduModeType PduModeRequest){
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
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

Std_ReturnType CanIf_GetPduMode(uint8 ControllerId,CanIf_PduModeType* PduModePtr);


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

Std_ReturnType CanIf_SetBaudrate(uint8 ControllerId,uint16 BaudRateConfigID) {
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}



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

void CanIf_TxConfirmation(PduIdType CanTxPduId){
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
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

void CanIf_RxIndication(const Can_HwType* Mailbox,const PduInfoType* PduInfoPtr){
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
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

void CanIf_ControllerBusOff(uint8 ControllerId){
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
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

void CanIf_ControllerModeIndication(uint8 ControllerId,CanIf_ControllerModeType ControllerMode){
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}





