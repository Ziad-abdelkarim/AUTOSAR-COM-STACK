#ifndef CANIF_H
#define CANIF_H
#include "Can_GeneralTypes.h"
#include "Canif_CLBK_STUB.h"
CanIf_ControllerModeType CurrentCanState[NUMBER_OF_CONTROLLERS];
void CanIf_ControllerModeIndication(uint8 ControllerId,CanIf_ControllerModeType ControllerMode);
void CanIf_ControllerBusOff(uint8 ControllerId);
void CanIf_TxConfirmation(PduIdType CanTxPduId);
void CanIf_RxIndication(const Can_HwType* Mailbox,const PduInfoType* PduInfoPtr);

#endif /*CANIF_H*/
