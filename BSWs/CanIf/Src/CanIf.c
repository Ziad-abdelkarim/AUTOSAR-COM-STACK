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

void CanIf_ControllerModeIndication(uint8 ControllerId,CanIf_ControllerModeType ControllerMode)
{
	
    uint16 ModuleId=0x12;                                     /*Module ID of calling module.*/
    uint8 ApiId=0x17;                                         /*ID of API service in which error is detected (defined in SWS of calling module)*/
    uint8 InstanceId=0;                                       /*The identifier of the index based instance of a module, starting from 0,
                                                                If the module is a single instance module it shall pass 0 as the InstanceId.*/

    /*
     * [SWS_CANIF_00661] d If the switch CANIF_PUBLIC_DEV_ERROR_DETECT is enabled,
       all CanIf API services shall:
       - not execute their normal operation
       - report to the DET (using CANIF_E_UNINIT)
       unless the CanIf has been initialized with a preceding call of CanIf_Init().
     */

        #if (CANIF_PUBLIC_DEV_ERROR_DETECT==1)            /*Development Error in case of UN_INT of CanIf or invalid parameters*/


        /*
         * [SWS_CANIF_00702] d If CanIf was not initialized before calling CanIf_ControllerModeIndication(),
           CanIf shall not execute state transition notification, when CanIf_ControllerModeIndication()
           is called. c()
         */
           if(CanIfState == CANIF_UNINIT)
            {
              // Det_ReportError(ModuleId,InstanceId,ApiId,CANIF_E_UNINIT);

            }

         /*
          * [SWS_CANIF_00700] d If parameter ControllerId of CanIf_ControllerModeIndication()
            has an invalid value, CanIf shall report development error code CANIF_E_PARAM_CONTROLLER
            to the Det_ReportError service of the DET module, when CanIf_ControllerModeIndication()
            is called. c
          */
           if(ControllerId >=NUMBER_OF_CONTROLLERS)
             {
               //Det_ReportError(ModuleId,InstanceId,ApiId,CANIF_E_PARAM_CONTROLLERID);
             }


        #else

                 switch (ControllerMode)
                    {
                        /*
                         * Indication to the new state setted by CanDrv
                         */

                     case CANIF_CS_SLEEP:
                         CanIfControllerMode[ControllerId] =CANIF_CS_SLEEP;
                            break;

                     case CANIF_CS_STARTED:
                         CanIfControllerMode[ControllerId] =CANIF_CS_STARTED;
                            break;

                     case CANIF_CS_STOPPED:
                         CanIfControllerMode[ControllerId] =CANIF_CS_STOPPED;
                            break;

                     default:
                         //Det_ReportError(ModuleId,InstanceId,ApiId,CANIF_E_PARAM_CTRLMODE);
                            break;
                    }


    #endif

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}





