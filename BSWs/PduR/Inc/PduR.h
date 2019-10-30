/*******************************************************************************************************************************
FileName:                    							  PduR.h                                      
AUTOSAR Version:          								  4.2.2
******************************************************************************************************************************/
#ifndef PDUR_H
#define PDUR_H

/******************************************************************************************************************************
 **                                                                        Includes                                                                             **
 ******************************************************************************************************************************/
#include "Platform_Types.h"
#include "ComStack_Types.h"
/*******************************************************************************************************************************
**                                               						  Defines                                                                                **
********************************************************************************************************************************/



/*******************************************************************************************************************************
**                                               						   Variables                                                                             **
********************************************************************************************************************************/



/*******************************************************************************************************************************
**                                               				    Function prototypes                                                                       **
********************************************************************************************************************************/
void PduR_CanIfRxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr);
void PduR_CanIfTxConfirmation(PduIdType TxPduId);


#endif /*PDUR_H*/
