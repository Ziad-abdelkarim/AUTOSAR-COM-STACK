/*******************************************************************************************************************************
FileName:                    								    CanIf.h                                      
AUTOSAR Version:          								  4.2.2
******************************************************************************************************************************/
#ifndef CANIF_H
#define CANIF_H

/******************************************************************************************************************************
 **                                                                        Includes                                                                             **
 ******************************************************************************************************************************/
#include "CanIf_Cfg.h"
#include "CanIf_Types.h"
/*******************************************************************************************************************************
**                                               						  Defines                                                                                **
********************************************************************************************************************************/
 #define NUMBER_OF_CHANNELS   2


/*******************************************************************************************************************************
**                                               						   Variables                                                                             **
********************************************************************************************************************************/
CanIf_ControllerModeType CurrentCanState[NUMBER_OF_CONTROLLERS];


/*******************************************************************************************************************************
**                                               				    Function prototypes                                                                       **
********************************************************************************************************************************/

void CanIf_Init(const CanIf_ConfigType* ConfigPtr);
Std_ReturnType CanIf_SetControllerMode(uint8 ControllerId,CanIf_ControllerModeType ControllerMode);
Std_ReturnType CanIf_GetControllerMode(uint8 ControllerId,CanIf_ControllerModeType* ControllerModePtr);
Std_ReturnType CanIf_Transmit(PduIdType CanIfTxSduId,const PduInfoType* CanIfTxInfoPtr);
Std_ReturnType CanIf_ReadRxPduData(PduIdType CanIfRxSduId,PduInfoType* CanIfRxInfoPtr);
CanIf_NotifStatusType CanIf_ReadTxNotifStatus(PduIdType CanIfTxSduId);
CanIf_NotifStatusType CanIf_ReadRxNotifStatus(PduIdType CanIfRxSduId);
Std_ReturnType CanIf_SetPduMode(uint8 ControllerId, CanIf_PduModeType PduModeRequest);
Std_ReturnType CanIf_GetPduMode(uint8 ControllerId,CanIf_PduModeType* PduModePtr);
Std_ReturnType CanIf_SetBaudrate(uint8 ControllerId,uint16 BaudRateConfigID);
Std_ReturnType Det_ReportError(uint16 ModuleId,uint8 InstanceId,uint8 ApiId,uint8 ErrorId);
void CanIf_ControllerModeIndication(uint8 ControllerId,CanIf_ControllerModeType ControllerMode);
void CanIf_ControllerBusOff(uint8 ControllerId);
void CanIf_TxConfirmation(PduIdType CanTxPduId);
void CanIf_RxIndication(const Can_HwType* Mailbox,const PduInfoType* PduInfoPtr);
void main(void);

#endif /*CANIF_H*/
