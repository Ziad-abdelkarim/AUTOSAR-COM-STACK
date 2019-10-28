/********************************************************************************************************************************
 **
 FileName:                   CanIf.c
 AUTOSAR Version:            4.2.2
                                                                                                                                                                          **
 ********************************************************************************************************************************/
/********************************************************************************************************************************
 **                                                          Includes                                                                                                **
 ********************************************************************************************************************************/
#include "CanIf.h"
#include "Can.h"
/********************************************************************************************************************************
 **                                                       Global Variables                                                                                       **
 ********************************************************************************************************************************/
 extern CanIf_ConfigType CanIf;

typedef enum {CANIF_UNINIT, CANIF_READY} CanIfStateType ;
typedef struct{
    CanIfStateType CM_stateMachine;
    CanIf_ControllerModeType CM_SubstateMachine;
}ControllerModeStateMachine;
ControllerModeStateMachine CCMSM;

extern CanDriverStateType CanDriverState;

 
 
 
 
 
 
 
 
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
    Std_ReturnType Can_SetControllerModeReturn = E_OK;


	/*
	 * [SWS_CANIF_00311] d If parameter ControllerId of CanIf_SetControllerMode()
       has an invalid value, the CanIf shall report development error code CANIF_E_PARAM_CONTROLLERID
       to the Det_ReportError service of the DET module, when CanIf_SetControllerMode()
       is called
	 */
    #if(ControllerId > NUMBER_OF_CONTROLLERS)
    {

        return Det_ReportError(Canif_ModuleID, , Canif_SetControllerModeId, CANIF_E_PARAM_CONTROLLERID);
    }
    #endif

    /*
     * [SWS_CANIF_00311] d If parameter ControllerId of CanIf_SetControllerMode()
       has an invalid value, the CanIf shall report development error code CANIF_E_PARAM_CONTROLLERID
       to the Det_ReportError service of the DET module, when CanIf_SetControllerMode()
       is called.
     */
    #if((ControllerMode != CANIF_CS_STARTED)||(ControllerMode != CANIF_CS_STOPPED)||(ControllerMode != CANIF_CS_SLEEP))
    {
        return Det_ReportError(Canif_ModuleID, 0, Canif_SetControllerModeId, CANIF_E_PARAM_CTRLMODE);
    }
    #endif
	
    /*
     * [SWS_CANIF_00312]  Caveats of CanIf_SetControllerMode():
        • The CAN Driver module must be initialized after Power ON.
        • The CAN Interface module must be initialized after Power ON.
     */
    #if((CCMSM.CM_stateMachine==CANIF_READY) && (CanDriverState ==CAN_READY))
    {
        Switch(ControllerMode)
        {
            /*
             * [SWS_CANIF_00480] d If a CCMSM is in state CANIF_CS_STOPPED or CANIF_CS_STARTED
               when CanIf_SetControllerMode(ControllerId, CANIF_CS_STOPPED) is called
               with parameter ControllerId referencing that CCMSM, then CanIf shall call Can_SetControllerMode(CAN_T_STOP).
             */
            case CANIF_CS_STOPPED :
            {
                CanIf_SetPduMode(ControllerId, CANIF_OFFLINE);
                /*
                 * [SWS_CANIF_00487] If a CCMSM is in state CANIF_CS_SLEEP when CanIf_SetControllerMode(CANIF_CS_STOPPED)
                 * is called with parameter ControllerId referencing that CCMSM,
                   then CanIf shall call Can_SetControllerMode(Controller, CAN_T_WAKEUP).
                 */
                if(CCMSM.CM_SubstateMachine == CANIF_CS_SLEEP)
                {
                    Can_SetControllerMode(ControllerId, CAN_T_WAKEUP);
                }
                else
                {
                    Can_SetControllerModeReturn = Can_SetControllerMode(ControllerId,CAN_T_STOP);
                }
                /*
                 * [SWS_CANIF_00475] d If during function CanIf_SetControllerMode() the call of
                   Can_SetControllerMode() returns with CAN_NOT_OK, CanIf_SetControllerMode()
                   returns E_NOT_OK.
                 */
                if(Can_SetControllerModeReturn == E_NOT_OK)
                {
                    return E_NOT_OK;
                }
                else
                {
                    return E_OK;

                }
                break;
            }
            /*
             * [SWS_CANIF_00481] d When CanIf_SetControllerMode(ControllerId, CANIF_CS_STARTED)
               is called with parameter ControllerId referencing that CCMSM, then CanIf shall call
               Can_SetControllerMode(Controller, CAN_T_START).
             */
            case CANIF_CS_STARTED :
            {
                CanIf_SetPduMode(ControllerId, CANIF_ONLINE);
                Can_SetControllerMode(ControllerId, CAN_T_START);
                /*
                 * [SWS_CANIF_00475] If during function CanIf_SetControllerMode() the call of
                   Can_SetControllerMode() returns with CAN_NOT_OK, CanIf_SetControllerMode()
                   returns E_NOT_OK.
                 */
                if(Can_SetControllerModeReturn == E_NOT_OK)
                {
                    return E_NOT_OK;
                }
                else
                {
                    return E_OK;
                }
                break;
            }
            /*
             * [SWS_CANIF_00482] d When CanIf_SetControllerMode(ControllerId, CANIF_CS_SLEEP)
               is called with parameter ControllerId referencing that CCMSM, then CanIf shall call
               Can_SetControllerMode(Controller, CAN_T_SLEEP).
             */
            case CANIF_CS_SLEEP :
            {
                CanIf_SetPduMode(ControllerId, CANIF_OFFLINE);
                Can_SetControllerModeReturn = Can_SetControllerMode(ControllerId, CAN_T_SLEEP);
                /*
                 * [SWS_CANIF_00475] If during function CanIf_SetControllerMode() the call of
                   Can_SetControllerMode() returns with CAN_NOT_OK, CanIf_SetControllerMode()
                   returns E_NOT_OK.
                 */
                if(Can_SetControllerModeReturn == E_NOT_OK)
                {
                    return E_NOT_OK;
                }
                else
                {
                    return E_OK;
                }
                break;
            }
        }
    }
    #endif
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
	
    /*
     * [SWS_CANIF_00313] d If parameter ControllerId of CanIf_GetControllerMode()
       has an invalid, the CanIf shall report development error code CANIF_E_PARAM_CONTROLLERID
       to the Det_ReportError service of the DET, when CanIf_GetControllerMode()
       is called.
     */
    #if(ControllerId > NUMBER_OF_CONTROLLERS)
    {

        return Det_ReportError(Canif_ModuleID, , Canif_SetControllerModeId, CANIF_E_PARAM_CONTROLLERID);
    }
    #endif

    /*
     * [SWS_CANIF_00656] d If parameter ControllerModePtr of CanIf_GetControllerMode()
       has an invalid value, the CanIf shall report development error code CANIF_E_PARAM_POINTER
       to the Det_ReportError service of the DET, when CanIf_GetControllerMode()
       is called.
     */
    #if(ControllerModePtr == NULL)
    {
        return Det_ReportError(Canif_ModuleID, 0, Canif_GetControllerModeID, CANIF_E_PARAM_POINTER);
    }
    #endif

    /*
     * [SWS_CANIF_00316] d Caveats of CanIf_GetControllerMode:
        • The CanDrv must be initialized after Power ON.
        • The CanIf must be initialized after Power ON.
     */
    #if((CCMSM.CM_stateMachine==CANIF_READY) && (CanDriverState ==CAN_READY))
    {
        if((CCMSM.CM_stateMachine= CANIF_UNINIT) || (CCMSM.CM_stateMachine = CANIF_READY))
        {
            *ControllerModePtr = CCMSM.CM_stateMachine;
        }
        else if ((CCMSM.CM_SubstateMachine = CANIF_CS_STARTED) ||(CCMSM.CM_SubstateMachine = CANIF_CS_STOPPED) ||(CCMSM.CM_SubstateMachine = CANIF_CS_SLEEP))
        {
            *ControllerModePtr = CCMSM.CM_SubstateMachine ;
        }
        else
        {
            /* Controller mode request has not been accepted. */
            return E_NOT_OK;
        }
        /* Controller mode request has been accepted.*/
            return E_OK;
    }
    #endif
    return E_NOT_OK;
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

CanIf_NotifStatusType CanIf_ReadTxNotifStatus(PduIdType CanIfTxSduId){
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	






























}


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

CanIf_NotifStatusType CanIf_ReadRxNotifStatus(PduIdType CanIfRxSduId){
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}

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





