/*******************************************************************************************************************************
FileName:                    								    Com.h
AUTOSAR Version:          								  4.2.2
******************************************************************************************************************************/
#ifndef COM_H
#define COM_H

/******************************************************************************************************************************
 **                                                                        Includes                                                                             **
 ******************************************************************************************************************************/
#include "Com_Cfg.h"
#include "Com_Types.h"
#include "ComTeam_Types.h"
#include "ComStack_Types.h"
/*******************************************************************************************************************************
**                                               						  Defines                                                                                **
********************************************************************************************************************************/

/********************************************************************************************************************************
*                                           						   Variables                                                                             **
********************************************************************************************************************************/

/*******************************************************************************************************************************
                                             				    Function prototypes                                                                       **
********************************************************************************************************************************/

void Com_Init( const Com_ConfigType* config);
uint8 Com_SendSignal(Com_SignalIdType SignalId,const void* SignalDataPtr);
uint8 Com_ReceiveSignal(Com_SignalIdType SignalId,void* SignalDataPtr);
void Com_RxIndication(PduIdType RxPduId,const PduInfoType* PduInfoPtr);
void Com_TxConfirmation(PduIdType TxPduId);
void Com_MainFunctionTx(void);
void Com_MainFunctionRx(void);
uint8 Com_SendSignalGroup(Com_SignalGroupIdType SignalGroupId);
uint8 Com_ReceiveSignalGroup(Com_SignalGroupIdType SignalGroupId);
void Com_UpdateShadowSignal(Com_SignalIdType SignalId,const void* SignalDataPtr);
void Com_ReceiveShadowSignal(Com_SignalIdType SignalId, void* SignalDataPtr);
Std_ReturnType Com_TriggerIPDUSend(PduIdType PduId);
void main(void);

#endif /*COM_H*/
